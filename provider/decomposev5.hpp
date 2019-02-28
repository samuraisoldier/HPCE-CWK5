#ifndef user_decompose_hppv5
#define user_decompose_hppv5

#include "puzzler/puzzles/decompose.hpp"
#include "tbb/parallel_for.h"
#include "tbb/atomic.h"

namespace puzzler{
class DecomposeProviderv5
  : public puzzler::DecomposePuzzle
{
public:
  DecomposeProviderv5()
  {}
  

    void decompose(ILog *log, unsigned rr, unsigned cc, unsigned p, uint32_t *matrix) const
    {
      auto at = [=](unsigned r, unsigned c) -> uint32_t &{
        assert(r<rr && c<cc);
        return matrix[rr*c+r];
      };

      dump(log, Log_Debug, rr, cc, matrix);

      unsigned rank=0;
	  //tbb::parallel_for(0u,cc,[&](unsigned c1){
      for(unsigned c1=0; c1<cc; c1++){
        unsigned r1=rank;
        while(r1<rr && at(r1,c1)==0){
          ++r1;
        }

        if(r1!=rr){
          unsigned pivot=at(r1,c1);
		  
		  tbb::parallel_for(0u,cc,[&](unsigned c2){
            std::swap( at(r1,c2), at(rank,c2) );
            at(rank,c2)=div( at(rank,c2) , pivot );
          });
			
		  tbb::parallel_for(unsigned(rank+1),rr,[&](unsigned r2){
            unsigned count=at(r2, c1);
			tbb::parallel_for(unsigned(0),cc,[&](unsigned c2){
              at(r2,c2) = sub( at(r2,c2) , mul( count, at(rank,c2)) );
            });
          });

          ++rank;
        }

        dump(log, Log_Debug, rr, cc, matrix);
      }//);
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
	  
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
	  
      log->LogInfo("Doing the decomposition");
	  
      decompose(log, rr, cc, p, &matrix[0]);
      
      log->LogInfo("Collecting decomposed hash.");
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
	
      tbb::atomic<uint64_t> hash=0;
	  tbb::parallel_for(0u,unsigned(matrix.size()),[&](unsigned i){
        hash += uint64_t(matrix[i])*i;
      });
      pOutput->hash=hash;
      
      log->LogInfo("Finished");
    }


};
};
#endif

