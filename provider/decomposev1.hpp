#ifndef user_decompose_hppv1
#define user_decompose_hppv1

#include "puzzler/puzzles/decompose.hpp"

namespace puzzler{
class DecomposeProviderv1
  : public puzzler::DecomposePuzzle
{
public:
  DecomposeProviderv1()
  {}

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
      for(unsigned i=0; i<matrix.size(); i++){
        matrix[i]=make_bit(pInput->seed, i);
      }
      dump(log, Log_Verbose, rr, cc, &matrix[0]);
      
      log->LogInfo("Doing the decomposition");
      decompose(log, rr, cc, p, &matrix[0]);
      
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
