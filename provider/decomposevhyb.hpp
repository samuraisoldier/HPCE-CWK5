#ifndef user_decompose_hppvhyb
#define user_decompose_hppvhyb
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS 
#include "CL/cl.hpp"
#include "tbb/atomic.h"

#include <fstream>
#include <streambuf>
#include "puzzler/puzzles/decompose.hpp"

namespace puzzler{
class DecomposeProvidervhyb
  : public puzzler::DecomposePuzzle
{
private:
	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
	cl::Device device;
public:
  DecomposeProvidervhyb()
  {	  		cl::Platform::get(&platforms);
		if(platforms.size()==0){
			throw std::runtime_error("No OpenCL platforms found.");
		}
		
		//std::cerr<<"Found "<<platforms.size()<<" platforms\n";
		//for(unsigned i=0;i<platforms.size();i++){
		//	std::string vendor=platforms[i].getInfo<CL_PLATFORM_VENDOR>();
		//	std::cerr<<"  Platform "<<i<<" : "<<vendor<<"\n";
		//}
		
		int selectedPlatform=0;
		if(getenv("HPCE_SELECT_PLATFORM")){
			selectedPlatform=atoi(getenv("HPCE_SELECT_PLATFORM"));
		}
//std::cerr<<"Choosing platform "<<selectedPlatform<<"\n";
		cl::Platform platform=platforms.at(selectedPlatform);   
		

		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);	
		if(devices.size()==0){
			throw std::runtime_error("No opencl devices found.\n");
		}
			
		//std::cerr<<"Found "<<devices.size()<<" devices\n";
		//for(unsigned i=0;i<devices.size();i++){
		//	std::string name=devices[i].getInfo<CL_DEVICE_NAME>();
		//	std::cerr<<"  Device "<<i<<" : "<<name<<"\n";
		//}
		
		int selectedDevice=0;
		if(getenv("HPCE_SELECT_DEVICE")){
			selectedDevice=atoi(getenv("HPCE_SELECT_DEVICE"));
		}
		//std::cerr<<"Choosing device "<<selectedDevice<<"\n";
		device=devices.at(selectedDevice);}
  
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
  
  

    void decompose(ILog *log, unsigned rr, unsigned cc, uint32_t *matrix) const
    {
      auto at = [=](unsigned r, unsigned c) -> uint32_t &{
        assert(r<rr && c<cc);
        return matrix[rr*c+r];
      };
	//unsigned p=7;

		
		cl::Context context(devices);
		
		std::string kernelSource=LoadSource("decomposel_kernel3.cl");
		
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
		
		//std::cerr<<"Chosen device and platform"<<"\n\n";
		size_t mBuffer=rr*cc*4;
		cl::Buffer buffM(context, CL_MEM_READ_WRITE, mBuffer);

		//cl::Kernel kernel(program, "decompose_kernel");
		cl::Kernel kernel_2(program, "decompose_kernel_2");
		kernel_2.setArg(0, rr);
		kernel_2.setArg(1, cc);
		kernel_2.setArg(2, buffM);

		cl::CommandQueue queue(context, device);

		//kernel.setArg(0, rr);
		//kernel.setArg(1, cc);
		//kernel.setArg(2, buffM);
		

	    dump(log, Log_Debug, rr, cc, matrix);
        unsigned rank=0;
		
		for(unsigned c1=0; c1<cc; c1++){
			unsigned r1=rank;
			while(r1<rr && at(r1,c1)==0){
				++r1;
			}
		
        if(r1!=rr){
          unsigned pivot=at(r1,c1);
			kernel_2.setArg(3, rank);
			kernel_2.setArg(4, pivot);
			kernel_2.setArg(5, r1);
			//kernel.setArg(4, c1);
			//kernel.setArg(3, rank);		
			
			
			cl::Event ev2CopiedState;
			queue.enqueueWriteBuffer(buffM, CL_FALSE, 0, mBuffer, &matrix[0], NULL, &ev2CopiedState);

			cl::NDRange offset2(0);				// Always start iterations at x=0, y=0
			cl::NDRange globalSize2(cc);	// Global size must match the original loops
			cl::NDRange localSize2=cl::NullRange;	// We don't care about local size
			
			std::vector<cl::Event> kernelDependencies2(1, ev2CopiedState);
			cl::Event ev2ExecutedKernel;
			queue.enqueueNDRangeKernel(kernel_2, offset2, globalSize2, localSize2, &kernelDependencies2, &ev2ExecutedKernel);
			
			std::vector<cl::Event> copyBackDependencies2(1, ev2ExecutedKernel);
			queue.enqueueReadBuffer(buffM, CL_TRUE, 0, mBuffer, &matrix[0], &copyBackDependencies2);
          
		tbb::parallel_for(unsigned(rank+1),rr,[&](unsigned r2){	//THIS ONE SPEEDS UP
          //for(unsigned r2=rank+1; r2<rr; r2++){
            unsigned count=at(r2, c1);
			//tbb::parallel_for(0u,cc,[&](unsigned c2){
            for(unsigned c2=0; c2<cc; c2++){
              at(r2,c2) = sub( at(r2,c2) , mul( count, at(rank,c2)) );
            }//);
          });
        ++rank;
        }

        dump(log, Log_Debug, rr, cc, matrix);
      }
    }

	uint32_t make_bit(uint32_t seed, uint32_t input) const
    {
      const uint32_t PRIME32_1  = 2654435761U;
      const uint32_t PRIME32_2 = 2246822519U;
      seed += input * PRIME32_2;
      seed  = (seed<<13) | (seed>>(32-13));
      seed *= PRIME32_1;
      return seed % P;
    }

    void Execute(
			  ILog *log,
			  const DecomposeInput *pInput,
			  DecomposeOutput *pOutput
			  ) const
    {
      unsigned n=pInput->n;
      unsigned rr=n;
      unsigned cc=n;
      unsigned p=7;
	  
	  
      log->LogInfo("Building random matrix");
      std::vector<uint32_t> matrix(rr*cc);
	  
	  tbb::parallel_for(0u,unsigned(matrix.size()),[&](unsigned i){
        matrix[i]=make_bit(pInput->seed, i);
      });
	  /*
      for(unsigned i=0; i<matrix.size(); i++){
        matrix[i]=make_bit(pInput->seed, i);
      }
	  */
	  
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
      
      log->LogInfo("Doing the decomposition");		

	  
	  
      decompose(log, rr, cc, &matrix[0]); //make it gpuified
      
      log->LogInfo("Collecting decomposed hash.");
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
      uint64_t hash=0;
      for(unsigned i=0; i<matrix.size(); i++){
        hash += uint64_t(matrix[i])*i;
      }
	  /*tbb::atomic<uint64_t> hash=0;
	  tbb::parallel_for(0u,unsigned(matrix.size()),[&](unsigned i){
        hash += uint64_t(matrix[i])*i;
      });*/
	  
      pOutput->hash=hash;
      
      log->LogInfo("Finished");
    }

};
};

#endif


      
