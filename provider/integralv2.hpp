#ifndef user_integral_hppv2
#define user_integral_hppv2

#include "puzzler/puzzles/integral.hpp"

namespace puzzler{
class IntegralProviderv2
  : public puzzler::IntegralPuzzle
{
public:
  IntegralProviderv2()
  {}

    void Execute(
			  ILog *log,
			  const IntegralInput *pInput,
			  IntegralOutput *pOutput
			  ) const
    {
      unsigned r=pInput->resolution;

      const float range=12;

      double acc=0;
      for(unsigned i1=0; i1<r; i1++){
        for(unsigned i2=0; i2<r; i2++){
          for(unsigned i3=0; i3<r; i3++){
            float x1= -range/2 + range * (i1/(float)r);
            float x2= -range/2 + range * (i2/(float)r);
            float x3= -range/2 + range * (i3/(float)r);

            float x[3]={x1,x2,x3};
            acc += mpdf(r, range, x, &pInput->M[0], &pInput->C[0], &pInput->bounds[0]);
          }
        }
      }

      log->LogInfo("Integral = %g", acc);
      pOutput->value=acc;
    }

};
};
#endif
