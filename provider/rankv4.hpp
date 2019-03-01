#ifndef user_rank_hppv4
#define user_rank_hppv4

#include "puzzler/puzzles/rank.hpp"
#include "tbb/parallel_for.h"

namespace puzzler{
class RankProviderv4
  : public puzzler::RankPuzzle
{
public:
  RankProviderv4()
  {}
    float norm(const std::vector<float> &a, const std::vector<float> &b) const
    {
      double acc=0;
	  unsigned sze = a.size();
	  //tbb::parallel_for(0u,sze,[&](unsigned i){
      for(unsigned i=0; i<a.size(); i++){
		float subres = a[i]-b[i];
        acc += (subres*subres);
      }//);
      return acc;
    }

    void iteration(ILog *log, unsigned n, const std::vector<std::vector<uint32_t> > &edges, const float *current, float *next) const
    {
	  std::memset(&next[0], 0, n*sizeof(next[0]));
      //for(unsigned i=0; i<n; i++){
        //next[i]=0;
      //}
      for(unsigned i=0; i<n; i++){
	  
		//tbb::parallel_for(0u,sze,[&](unsigned j){
        for(unsigned j=0; j<edges[i].size(); j++){
          int dst=edges[i][j];
          next[dst] += current[i] / edges[i].size();
        }//);
      }//);

      /*double total=0;
	  //tbb::parallel_for(0u,n,[&](unsigned i){
      for(unsigned i=0; i<n; i++){
        next[i] = (current[i] * 0.3  + next[i] * 0.7 );
        total += next[i];
      }//);
      //log->LogVerbose("  total=%g", total);
      for(unsigned i=0; i<n; i++){
	  //tbb::parallel_for(0u,n,[&](unsigned i){
        next[i] /= total;
        //log->LogVerbose("    c[%u] = %g", i, next[i]);
      }//);*/
    }
	
    void Execute(
			  ILog *log,
			  const RankInput *pInput,
			  RankOutput *pOutput
			  ) const
    {
      const std::vector<std::vector<uint32_t> > &edges=pInput->edges;
	  float tol=pInput->tol;
      tol*=tol;
      unsigned n=edges.size();

      log->LogInfo("Starting iterations.");
      std::vector<float> curr(n, 0.0f);
      curr[0]=1.0;
      std::vector<float> next(n, 0.0f);
      float dist=norm(curr,next);
	  

	   while( tol < dist ){
        log->LogVerbose("dist=%g", dist);
        iteration(log, n, edges, &curr[0], &next[0]);
		if(tol >= norm(curr, next)){
			pOutput->ranks=next;
			break;
		}
		iteration(log, n, edges, &next[0], &curr[0]);
        //std::swap(curr, next);
        dist=norm(curr, next);
      }
	  
	  pOutput->ranks=curr;
      log->LogInfo("Finished");
    }

};
};
#endif
