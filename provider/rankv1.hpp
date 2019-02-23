#ifndef user_rank_hppv1
#define user_rank_hppv1

#include "puzzler/puzzles/rank.hpp"

namespace puzzler{
class RankProviderv1
  : public puzzler::RankPuzzle
{
public:
  RankProviderv1()
  {}

    void ReferenceExecute(
			  ILog *log,
			  const RankInput *pInput,
			  RankOutput *pOutput
			  ) const
    {
      const std::vector<std::vector<uint32_t> > &edges=pInput->edges;
      float tol=pInput->tol;
      unsigned n=edges.size();

      log->LogInfo("Starting iterations.");
      std::vector<float> curr(n, 0.0f);
      curr[0]=1.0;
      std::vector<float> next(n, 0.0f);
      float dist=norm(curr,next);
      while( tol < dist ){
        log->LogVerbose("dist=%g", dist);
        iteration(log, n, edges, &curr[0], &next[0]);
        std::swap(curr, next);
        dist=norm(curr, next);
      }
      
      pOutput->ranks=curr;
      
      log->LogInfo("Finished");
    }

};
};
#endif
