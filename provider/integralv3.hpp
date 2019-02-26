#ifndef user_integral_hpp_v3
#define user_integral_hpp_v3

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS 
#include "CL/cl.hpp"

#include <fstream>
#include <streambuf>

#include "puzzler/puzzles/integral.hpp"

namespace puzzler{
class IntegralProviderv3
  : public puzzler::IntegralPuzzle
{
public:
  IntegralProviderv3()
  {}
	void integral_kernel(int r, float range, const float M[D*D], const float C[D], const float bounds[D], double *sum_total) const
	{
	  static const int D=3;
		for(int i1=0; i1<r; i1++){
			for(int i2=0; i2<r; i2++){
				for(int i3=0; i3<r; i3++){
				  float x1= -range/2 + range * (i1/(float)r);
				  float x2= -range/2 + range * (i2/(float)r);
				  float x3= -range/2 + range * (i3/(float)r);

				  float x[3]={x1,x2,x3};
				  float dx=range/r;

				  float acc=1.0f;
				  for(unsigned i=0; i<D; i++){
					float xt=C[i];
					for(unsigned j=0; j<D; j++){
					  xt += M[i*D+j] * x[j];
					}
					acc *= (exp(-xt*xt/2) / sqrt(2*3.1415926535897932384626433832795)) * dx;
				  }

				  for(unsigned i=0; i<D;i++){
					if(x[i] > bounds[i]){
					  acc=0;
					}
				  }
				  sum_total[0] += acc;
				}
			}
		}
	}
	
	
	std::string LoadSource(const char *fileName)const
	{
		// TODO : Don't forget to change your_login here
		std::string baseDir="provider";
		if(getenv("HPCE_CL_SRC_DIR")){
			baseDir=getenv("HPCE_CL_SRC_DIR");
		}
		
		std::string fullName=baseDir+"/"+fileName;
		
		// Open a read-only binary stream over the file
		std::ifstream provider(fullName, std::ios::in | std::ios::binary);
		if(!provider.is_open())
			throw std::runtime_error("LoadSource : Couldn't load cl file from '"+fullName+"'.");
		
		// Read all characters of the file into a string
		return std::string(
			(std::istreambuf_iterator<char>(provider)), // Node the extra brackets.
			std::istreambuf_iterator<char>()
		);
	}
	
	
	
    void Execute(
			  ILog *log,
			  const IntegralInput *pInput,
			  IntegralOutput *pOutput
			  ) const
    {
		std::vector<cl::Platform> platforms;
	
		cl::Platform::get(&platforms);
		if(platforms.size()==0){
			throw std::runtime_error("No OpenCL platforms found.");
		}
		
		std::cerr<<"Found "<<platforms.size()<<" platforms\n";
		for(unsigned i=0;i<platforms.size();i++){
			std::string vendor=platforms[i].getInfo<CL_PLATFORM_VENDOR>();
			std::cerr<<"  Platform "<<i<<" : "<<vendor<<"\n";
		}
		
		int selectedPlatform=1;
		if(getenv("HPCE_SELECT_PLATFORM")){
			selectedPlatform=atoi(getenv("HPCE_SELECT_PLATFORM"));
		}
		std::cerr<<"Choosing platform "<<selectedPlatform<<"\n";
		cl::Platform platform=platforms.at(selectedPlatform);   
		
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);	
		if(devices.size()==0){
			throw std::runtime_error("No opencl devices found.\n");
		}
			
		std::cerr<<"Found "<<devices.size()<<" devices\n";
		for(unsigned i=0;i<devices.size();i++){
			std::string name=devices[i].getInfo<CL_DEVICE_NAME>();
			std::cerr<<"  Device "<<i<<" : "<<name<<"\n";
		}
		
		int selectedDevice=0;
		if(getenv("HPCE_SELECT_DEVICE")){
			selectedDevice=atoi(getenv("HPCE_SELECT_DEVICE"));
		}
		std::cerr<<"Choosing device "<<selectedDevice<<"\n";
		cl::Device device=devices.at(selectedDevice);
		
		cl::Context context(devices);
		
		std::string kernelSource=LoadSource("integral_kernel.cl");

		cl::Program::Sources sources;	// A vector of (data,length) pairs
		sources.push_back(std::make_pair(kernelSource.c_str(), kernelSource.size()+1));	// push on our single string

		cl::Program program(context, sources);
		try{
			program.build(devices);
		}catch(...){
			for(unsigned i=0;i<devices.size();i++){
				std::cerr<<"Log for device "<<devices[i].getInfo<CL_DEVICE_NAME>()<<":\n\n";
				std::cerr<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i])<<"\n\n";
			}
			throw;
		}
		std::cerr<<"Chosen device and platform"<<"\n\n";
		
		size_t ddBuffer=8*D*D;
		size_t dBuffer=8*D;
		size_t writeBuffer=16;
		cl::Buffer buffM(context, CL_MEM_READ_ONLY, ddBuffer);
		cl::Buffer buffC(context, CL_MEM_READ_ONLY, dBuffer);
		cl::Buffer buffD(context, CL_MEM_READ_ONLY, dBuffer);
		cl::Buffer buffSum(context, CL_MEM_READ_WRITE, writeBuffer);
				
				
		cl::Kernel kernel(program, "integral_kernel");
				
		unsigned r=pInput->resolution;
		double acc=0;
		
		kernel.setArg(0, r);
		kernel.setArg(1, buffM);
		kernel.setArg(2, buffC);
		kernel.setArg(3, buffD);
		kernel.setArg(4, buffSum);
		
		cl::CommandQueue queue(context, device);
		
		queue.enqueueWriteBuffer(buffM, CL_TRUE, 0, ddBuffer, &pInput->M[0]);
		queue.enqueueWriteBuffer(buffC, CL_TRUE, 0, dBuffer, &pInput->C[0]);
		queue.enqueueWriteBuffer(buffD, CL_TRUE, 0, dBuffer, &pInput->bounds[0]);
		
		cl::Event evCopiedState;
		queue.enqueueWriteBuffer(buffSum, CL_FALSE, 0, writeBuffer, &acc, NULL, &evCopiedState);
		
		
		cl::NDRange offset(0);				// Always start iterations at x=0, y=0
		cl::NDRange globalSize(1);	// Global size must match the original loops
		cl::NDRange localSize=cl::NullRange;	// We don't care about local size
		
		std::vector<cl::Event> kernelDependencies(1, evCopiedState);
		cl::Event evExecutedKernel;
		queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize, &kernelDependencies, &evExecutedKernel);
		
		std::vector<cl::Event> copyBackDependencies(1, evExecutedKernel);
		queue.enqueueReadBuffer(buffSum, CL_TRUE, 0, writeBuffer, &acc, &copyBackDependencies);

      log->LogInfo("Integral = %g", acc);
      pOutput->value=acc;
    }

};
};
#endif