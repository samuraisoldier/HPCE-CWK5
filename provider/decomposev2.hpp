#ifndef user_decompose_hppv2
#define user_decompose_hppv2

#include "puzzler/puzzles/decompose.hpp"

namespace puzzler{
class DecomposeProviderv2
  : public puzzler::DecomposePuzzle
{
public:
  DecomposeProviderv2()
  {}
  /*
    void decompose(ILog *log, unsigned rr, unsigned cc, unsigned p, uint32_t *matrix) const
    {
      auto at = [=](unsigned r, unsigned c) -> uint32_t &{
        assert(r<rr && c<cc);
        return matrix[rr*c+r];
      };

      dump(log, Log_Debug, rr, cc, matrix);

      unsigned rank=0;
      for(unsigned c1=0; c1<cc; c1++){
        unsigned r1=rank;
        while(r1<rr && at(r1,c1)==0){
          ++r1;
        }

        if(r1!=rr){
          unsigned pivot=at(r1,c1);
          for(unsigned c2=0; c2<cc; c2++){
            std::swap( at(r1,c2), at(rank,c2) );
            at(rank,c2)=div( at(rank,c2) , pivot );
          }

          for(unsigned r2=rank+1; r2<rr; r2++){
            unsigned count=at(r2, c1);
            for(unsigned c2=0; c2<cc; c2++){
              at(r2,c2) = sub( at(r2,c2) , mul( count, at(rank,c2)) );
            }
          }

          ++rank;
        }

        dump(log, Log_Debug, rr, cc, matrix);
      }
    }*/
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
			  const DecomposeInput *pInput,
			  DecomposeOutput *pOutput
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
				
		
      unsigned n=pInput->n;
      unsigned rr=n;
      unsigned cc=n;
      unsigned p=7;
      
      log->LogInfo("Building random matrix");
      std::vector<uint32_t> matrix(rr*cc);
      for(unsigned i=0; i<matrix.size(); i++){
        matrix[i]=make_bit(pInput->seed, i);
      }
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
	   
      log->LogInfo("Doing the decomposition");
	  
	  
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
	  
	  
      //decompose(log, rr, cc, p, &matrix[0]); //make it gpuified
      
      log->LogInfo("Collecting decomposed hash.");
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
      uint64_t hash=0;
      for(unsigned i=0; i<matrix.size(); i++){
        hash += uint64_t(matrix[i])*i;
      }
      pOutput->hash=hash;
      
      log->LogInfo("Finished");
    }

};
};

#endif
