#ifndef user_rank_hpp_v2
#define user_rank_hpp_v2

#include "puzzler/puzzles/rank.hpp"

namespace puzzler{
class RankProviderv2
  : public puzzler::RankPuzzle
{
public:
  RankProviderv2()
  {}
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
			  const RankInput *pInput,
			  RankOutput *pOutput
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

	std::string kernelSource=LoadSource("rank2.cl");

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
		
			
	const std::vector<std::vector<uint32_t> > &edges=pInput->edges;	
	float tol=pInput->tol;
	unsigned n=edges.size();

	log->LogInfo("Starting iterations.");
	std::vector<float> curr(n, 0.0f);
	curr[0]=1.0;
	std::vector<float> next(n, 0.0f);
	float dist=norm(curr,next);
	
	/*int max_i = edges.size();
	
	int edgesSizes[max_i] = {0};
	
	/*for(unsigned i=0; i<edges.size(); i++){
		edgesSizes[i] = edges[i].size();
	}
	*/
	size_t eBuffer=4*n;
	size_t sBuffer = 20 * n;

	
	cl::Buffer buffnext(context, CL_MEM_READ_WRITE, eBuffer);
	cl::Buffer buffcurr(context, CL_MEM_READ_ONLY, eBuffer);

	cl::Buffer buffedges(context, CL_MEM_READ_ONLY, sBuffer);
	cl::Kernel kernel(program, "iteration");
			

	kernel.setArg(0, buffedges);
	kernel.setArg(1, buffcurr);
	kernel.setArg(2, buffnext);


	cl::CommandQueue queue(context, device);
 

	queue.enqueueWriteBuffer(buffedges, CL_TRUE, 0, sBuffer, &edges[0]);
	//queue.enqueueWriteBuffer(buffcurr, CL_TRUE, 0, eBuffer, &curr[0]);	  
	  

    while( tol < dist ){

        log->LogVerbose("dist=%g", dist);
		
		for(unsigned i=0; i<n; i++){
			next[i]=0;
		}

		
		cl::Event evCopiedState;
		queue.enqueueWriteBuffer(buffcurr, CL_TRUE, 0, eBuffer, &curr[0],NULL, &evCopiedState);	
		queue.enqueueWriteBuffer(buffnext, CL_FALSE, 0, eBuffer, &next[0], NULL, &evCopiedState);

		
		cl::NDRange offset(0);				// Always start iterations at x=0, y=0
		cl::NDRange globalSize(n);	// Global size must match the original loops
		cl::NDRange localSize=cl::NullRange;	// We don't care about local size
		std::cerr<<"log6."<<"\n";
		std::vector<cl::Event> kernelDependencies(1, evCopiedState);
		cl::Event evExecutedKernel;
		queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize, &kernelDependencies, &evExecutedKernel);
		std::cerr<<"log7."<<"\n";
		std::vector<cl::Event> copyBackDependencies(1, evExecutedKernel);
		std::cerr<<"log9."<<"\n";
		queue.enqueueReadBuffer(buffnext, CL_TRUE, 0, eBuffer, &next[0], &copyBackDependencies);		
		std::cerr<<"log8."<<"\n";
		
		
		double total=0;
		for(unsigned i=0; i<n; i++){
			next[i] = (curr[i] * 0.3  + next[i] * 0.7 );
			total += next[i];
		}

		log->LogVerbose("  total=%g", total);

		for(unsigned i=0; i<n; i++){
			next[i] /= total;
			log->LogVerbose("    c[%u] = %g", i, next[i]);
		}
		
        std::swap(curr, next);
        dist=norm(curr, next);
    }
      
    pOutput->ranks=curr;
      
    log->LogInfo("Finished");
    }

};
};
#endif