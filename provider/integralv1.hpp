#ifndef user_integral_hppv1
#define user_integral_hppv1

#include "puzzler/puzzles/integral.hpp"
#include "tbb/parallel_for.h"

namespace puzzler{
class IntegralProviderv1
  : public puzzler::IntegralPuzzle
{
public:
  IntegralProviderv1()
  {}
    float mpdf(int r, float range, float x[D], const float M[D*D], const float C[D], const float bounds[D]) const
    {
      float dx=range/r;
	  //tbb::atomic<float> acc=1.0f;
      float acc=1.0f;
	  //tbb::parallel_for(0u,unsigned(D),[&](unsigned i){
      for(unsigned i=0; i<D; i++){
		tbb::atomic<float> xt=C[i];
        //float xt=C[i];
		tbb::parallel_for(0u,unsigned(D),[&](unsigned j){
        //for(unsigned j=0; j<D; j++){
          xt =xt + M[i*D+j] * x[j];
        });
        acc *= updf(xt) * dx;
      }//);

	  tbb::parallel_for(0u,unsigned(D),[&](unsigned i){
        if(x[i] > bounds[i]){
          acc=0;
        }
      });

      return acc;
    }

    void Execute(
			  ILog *log,
			  const IntegralInput *pInput,
			  IntegralOutput *pOutput
			  ) const
    {
      unsigned r=pInput->resolution;

      const float range=12;
	
	  //tbb::atomic<double> acc=0;
      double acc=0;
	  //tbb::parallel_for(unsigned(0),r,[&](unsigned i1){
      for(unsigned i1=0; i1<r; i1++){
        for(unsigned i2=0; i2<r; i2++){
		  //tbb::parallel_for(unsigned(0),r,[&](unsigned i3){
          for(unsigned i3=0; i3<r; i3++){
            float x1= -range/2 + range * (i1/(float)r);
            float x2= -range/2 + range * (i2/(float)r);
            float x3= -range/2 + range * (i3/(float)r);

            float x[3]={x1,x2,x3};
            acc += mpdf(r, range, x, &pInput->M[0], &pInput->C[0], &pInput->bounds[0]);
          }//);
        }
      }//);

      log->LogInfo("Integral = %g", acc);
      pOutput->value=acc;
    }

};
};
#endif
