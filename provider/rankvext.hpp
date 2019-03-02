#ifndef user_rank_hppvext
#define user_rank_hppvext

#include "puzzler/puzzles/rank.hpp"
#include "tbb/parallel_for.h"

namespace puzzler{
class RankProvidervext
  : public puzzler::RankPuzzle
{
public:
  RankProvidervext()
  {}

    void Execute(
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
        
		  tbb::parallel_for(0u,n,[&](unsigned i){
			next[i]=0;
		  });
		  
		  for(unsigned i=0; i<n; i++){
		  //tbb::parallel_for(0u,n,[&](unsigned i){
			for(unsigned j=0; j<edges[i].size(); j++){
			  int dst=edges[i][j];
			  next[dst] += curr[i] / edges[i].size();
			}
		  }//);
		  
		  double total=0;
		  
		  //for(unsigned i; i<n; i++){
		  tbb::parallel_for(0u,n,[&](unsigned i){
			next[i] = (curr[i] * 0.3  + next[i] * 0.7 );
			total = total + next[i];
		  });
		  
		  log->LogVerbose("  total=%g", total);
		  
		  for(unsigned i; i<n; i++){
		  //tbb::parallel_for(0u,n,[&](unsigned i){
			next[i] /= total;
			log->LogVerbose("    c[%u] = %g", i, next[i]);
		  }//);
		  
		  
        std::swap(curr, next);
        dist=norm(curr, next);
      }
      
      pOutput->ranks=curr;
      
      log->LogInfo("Finished");
    }

};
};
#endif
