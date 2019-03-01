__kernel void integral_kernel(
				int r,
				__global const float *M,
				__global const float *C,
				__global const float *bounds,
				__global double *sum_total)
	{
	  const float range=12;
	  const int D=3;
	  
	  uint i1=get_global_id(0);
	  /*
	  uint i2=get_global_id(1); 
	  uint i3=get_global_id(2);
	  */		  
			  
		/*for(int i1=0; i1<r; i1++){ */
			for(int i2=0; i2<r; i2++){
				for(int i3=0; i3<r; i3++){
				  double acc=1.0;
				  
				  int flg = 1;
				  
					  float x1= -range/2 + range * (i1/(float)r);
					  float x2= -range/2 + range * (i2/(float)r);
					  float x3= -range/2 + range * (i3/(float)r);

					  float x[3]={x1,x2,x3};
					  

					if((x[0] > bounds[0]) || (x[1] > bounds[1]) || (x[2] > bounds[2])){
					  flg = 0;
					}

					  
				  if(flg == 1){
					  float dx=range/r;

					  
					  for(unsigned i=0; i<D; i++){
						float xt=C[i];
						xt += M[i*3+0] * x[0];
						xt += M[i*3+1] * x[1];
						xt += M[i*3+2] * x[2];
						acc *= (exp(-xt*xt/2) / sqrt(2*3.1415926535897932384626433832795)) * dx;
					  }


					  sum_total[i1] += acc;
				  }
				  
				}
			}
		/*}*/
	}