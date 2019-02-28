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
private:

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
cl::Device device;

public:
  IntegralProviderv3()
  {
	  		cl::Platform::get(&platforms);
		if(platforms.size()==0){
			throw std::runtime_error("No OpenCL platforms found.");
		}
		
		//std::cerr<<"Found "<<platforms.size()<<" platforms\n";
		//for(unsigned i=0;i<platforms.size();i++){
		//	std::string vendor=platforms[i].getInfo<CL_PLATFORM_VENDOR>();
		//	std::cerr<<"  Platform "<<i<<" : "<<vendor<<"\n";
		//}
		
		int selectedPlatform=1;
		if(getenv("HPCE_SELECT_PLATFORM")){
			selectedPlatform=atoi(getenv("HPCE_SELECT_PLATFORM"));
		}
		//std::cerr<<"Choosing platform "<<selectedPlatform<<"\n";
		cl::Platform platform=platforms.at(selectedPlatform);   
		

		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);	
		if(devices.size()==0){
			throw std::runtime_error("No opencl devices found.\n");
		}
			
		//td::cerr<<"Found "<<devices.size()<<" devices\n";
		//for(unsigned i=0;i<devices.size();i++){
		//	std::string name=devices[i].getInfo<CL_DEVICE_NAME>();
		//	std::cerr<<"  Device "<<i<<" : "<<name<<"\n";
		//}
		
		int selectedDevice=0;
		if(getenv("HPCE_SELECT_DEVICE")){
			selectedDevice=atoi(getenv("HPCE_SELECT_DEVICE"));
		}
		//std::cerr<<"Choosing device "<<selectedDevice<<"\n";
		device=devices.at(selectedDevice);

	  
	  
  }
	
	
	std::string LoadSource(const char *fileName)const
	{
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
		unsigned r=pInput->resolution;
		size_t ddBuffer=4*D*D;
		size_t dBuffer=4*D;
		size_t writeBuffer=8*r;
		cl::Buffer buffM(context, CL_MEM_READ_ONLY, ddBuffer);
		cl::Buffer buffC(context, CL_MEM_READ_ONLY, dBuffer);
		cl::Buffer buffD(context, CL_MEM_READ_ONLY, dBuffer);
		cl::Buffer buffSum(context, CL_MEM_READ_WRITE, writeBuffer);
				
				
		cl::Kernel kernel(program, "integral_kernel");
				

		double acc[r]={0};
		
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
		cl::NDRange globalSize(r);	// Global size must match the original loops
		cl::NDRange localSize=cl::NullRange;	// We don't care about local size
		
		std::vector<cl::Event> kernelDependencies(1, evCopiedState);
		cl::Event evExecutedKernel;
		queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize, &kernelDependencies, &evExecutedKernel);
		
		std::vector<cl::Event> copyBackDependencies(1, evExecutedKernel);
		queue.enqueueReadBuffer(buffSum, CL_TRUE, 0, writeBuffer, &acc[0], &copyBackDependencies);

	  double total = 0;
	  for(unsigned i=0; i<r; i++){
		  total += acc[i];
	  }
      log->LogInfo("Integral = %g", total);
      pOutput->value=total;
    }

};
};
#endif