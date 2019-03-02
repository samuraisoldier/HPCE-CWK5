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
  {}/*
    float norm(ILog *log, const std::vector<float> &a, const std::vector<float> &b) const
    {
      double acc=0;
	  log->LogInfo("%u", a.size());
      for(unsigned i=0; i<a.size(); i++){
		float subres = a[i]-b[i];
        acc += (subres*subres);
      }
      return acc;
    }*/
	
	
    void Execute(
			  ILog *log,
			  const RankInput *pInput,
			  RankOutput *pOutput
			  ) const
    {
      const std::vector<std::vector<uint32_t> > &edges=pInput->edges;
      float tol=pInput->tol;
	  tol *=tol;
      unsigned n=edges.size();

      log->LogInfo("Starting iterations.");
      
	  std::vector<float> curr(n, 0.0f);
      curr[0]=1.0;
      std::vector<tbb::atomic<float> > next(n, 0.0f);
      float dist = 0;
	  tbb::parallel_for(0u, unsigned(curr.size()),[&](unsigned i){
	  //for(unsigned i=0; i<curr.size(); i++){
		float subres = curr[i]-next[i];
		dist += (subres*subres);
	  });
	  //float dist=norm(curr,next);
	  
      while( tol < dist ){
        log->LogVerbose("dist=%g", dist);
        
		  tbb::parallel_for(0u,n,[&](unsigned i){
			next[i]=0;
		  });
		  std::vector<float> tmp(n, 0.0f);
		  tbb::parallel_for(0u,n,[&](unsigned i){
			tmp[i] = curr[i]/edges[i].size();
		  });
		  for(unsigned i=0; i<n; i++){
			//tbb::parallel_for(0u,unsigned(edges[i].size()),[&](unsigned j){
			for(unsigned j=0; j<edges[i].size(); j++){
			  int dst=edges[i][j];
			  next[dst] = next[dst] + tmp[i];
			}//);
		  }//);
		  
		  tbb::atomic<double> total=0;
		  
		  //for(unsigned i; i<n; i++){
		  tbb::parallel_for(0u,n,[&](unsigned i){
			next[i] = (curr[i] * 0.3  + next[i] * 0.7 );
			total = total + next[i];
		  });
		  
		  log->LogVerbose("  total=%g", total);
		  /*
		  for(unsigned i; i<n; i++){
		  //tbb::parallel_for(0u,n,[&](unsigned i){
			next[i] /= total;
			log->LogVerbose("    c[%u] = %g", i, next[i]);
		  }//);
		  */
		tbb::parallel_for(0u, n,[&](unsigned i){
		//for(unsigned i; i<n; i++){
			float tmp = curr[i];
			curr[i] = next[i];
			next[i] = tmp;
		});
        //std::swap(curr, next);
        //dist = norm(log,curr,next);
		dist = 0;
		tbb::parallel_for(0u, unsigned(curr.size()),[&](unsigned i){
		//for(unsigned i=0; i<curr.size(); i++){
			float subres = curr[i]-next[i];
			dist += (subres*subres);
		});
      }
      
      pOutput->ranks=curr;
      
      log->LogInfo("Finished");
    }

};
};
#endif
