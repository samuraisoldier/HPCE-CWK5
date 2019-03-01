unsigned mul(unsigned a, unsigned b) 
{ return (a*b) % 7; }

unsigned sub(unsigned a, unsigned b) 
{ return (7+a-b) % 7; }

unsigned mul_inv(unsigned x) 
{
  //assert(x!=0);
  for(unsigned i=1; i<7; i++){
	if( ((i*x) % 7) == 1 ){
	  return i;
	}
  }
  //assert(0);
  return 0;
}

unsigned div(unsigned a, unsigned b) 
{ return mul(a, mul_inv(b) ); }




__kernel  void decompose_kernel(
						unsigned rr, 
						unsigned cc, 
						__global uint *matrix,
						unsigned rank,
						unsigned c1) 
    {
	unsigned p = 7;

	uint r2=get_global_id(0);

		unsigned count = matrix[rr*c1 + r2];
		for(unsigned c2=0; c2<cc; c2++){
		  matrix[rr*c2 + r2] = sub( matrix[rr*c2 + r2] , mul( count, matrix[rr*c2 + rank]) );

		}

}

__kernel void decompose_kernel_2(
						unsigned rr, 
						unsigned cc,  
						__global uint *matrix,
						unsigned rank,
						unsigned pivot,
						unsigned r1) 	
	{   
		uint c2=get_global_id(0);
		unsigned tmp = matrix[rr*c2 + r1];
		matrix[rr*c2 + r1] = matrix[rr*c2 + rank];
		matrix[rr*c2 + rank] = tmp;
		
         matrix[rr*c2 + rank]=div(  matrix[rr*c2 + rank] , pivot );
		 matrix[rr*c2 + rank]=div(  matrix[rr*c2 + rank] , pivot );
	}
						