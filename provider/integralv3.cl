__kernel void integral_kernel(int r, // 0
							__global const float M[D*D], // 1
							__global const float C[D], // 2
							__global const float bounds[D], //3 
							double *sum_total // 4
							);
{
  static const int D=3;
  const float range= 12;
	for(int i1=0; i1<r; i1++){
		for(int i2=0; i2<r; i2++){
			for(int i3=0; i3<r; i3++){
			  float x1= -range/2 + range * (i1/(float)r);
			  float x2= -range/2 + range * (i2/(float)r);
			  float x3= -range/2 + range * (i3/(float)r);

			  float x[3]={x1,x2,x3};
			  float dx=range/r;

			  float acc=1.0f;
			  for(unsigned i=0; i<D; i++){
				float xt=C[i];
				for(unsigned j=0; j<D; j++){
				  xt += M[i*D+j] * x[j];
				}
				acc *= (exp(-xt*xt/2) / sqrt(2*3.1415926535897932384626433832795)) * dx;
			  }

			  for(unsigned i=0; i<D;i++){
				if(x[i] > bounds[i]){
				  acc=0;
				}
			  }
			  sum_total[0] += acc;
			}
		}
	}
}