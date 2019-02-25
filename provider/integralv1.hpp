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
  public:
    static const int D=3;  
  protected:
    float mpdf(int r, float range, float x[D], const float M[D*D], const float C[D], const float bounds[D]) const
    {
      float dx=range/r;

	  float acc[3];
	  float acc_sum = 1.0f;
	  tbb::parallel_for(0u,unsigned(D),[&](unsigned i){

        float xt[3];
		float xt_sum = C[i];
        tbb::parallel_for(0u, unsigned (D), [&](unsigned j){

          xt[j] = M[i*D+j] * x[j];
        });
		for(unsigned j=0; j<D; j++){ //we introduced an extra loop as we had a floating point error leading to a slight difference in the answer	
		  xt_sum += xt[j];
        }//);
        acc[i] = updf(xt_sum) * dx;
      });
	  for(unsigned i = 0; i < D; i++){
		  acc_sum *= acc[i];
	  }

	  tbb::parallel_for(0u,unsigned(D),[&](unsigned i){
        if(x[i] > bounds[i]){
          acc_sum=0;
        }
      });

      return acc_sum;
    }

    void Execute(
			  ILog *log,
			  const IntegralInput *pInput,
			  IntegralOutput *pOutput
			  ) const
    {
      unsigned r=pInput->resolution;

      const float range=12;
	
	  double acctot = 0;
      double acci1[r]={0};

	  
	  tbb::parallel_for(unsigned(0),r,[&](unsigned i1){  
		double acci2[r] = {0};

        tbb::parallel_for(unsigned(0),r,[&](unsigned i2){
			double acci3[r] = {0};

		    tbb::parallel_for(unsigned(0),r,[&](unsigned i3){

            float x1= -range/2 + range * (i1/(float)r);
            float x2= -range/2 + range * (i2/(float)r);
            float x3= -range/2 + range * (i3/(float)r);

            float x[3]={x1,x2,x3};
			acci3[i3] = mpdf(r, range, x, &pInput->M[0], &pInput->C[0], &pInput->bounds[0]);
            
        });

		for(unsigned i3=0; i3<r; i3++){
			acci2[i2] += acci3[i3];			
		}

        });
		  for(unsigned i2=0; i2<r; i2++){
			acci1[i1] += acci2[i2];
		  }

      });
	
	  for(unsigned i1=0; i1<r; i1++){
		acctot += acci1[i1];
	  }
	  
	
      log->LogInfo("Integral = %g", acctot);
	  
      pOutput->value=acctot;
    }

};
};
#endif
