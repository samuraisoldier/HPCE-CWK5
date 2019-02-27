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



//unsigned at(unsigned r, unsigned c, unsigned rr ){
	//assert(r<rr && c<cc);
	//return matrix[rr*c+r];
//}


__kernel  void decompose_kernel(
						unsigned rr, 
						unsigned cc, 
						unsigned p, 
						__global uint *matrix,
						unsigned rank,
						unsigned c1) 
    {

	uint r2=get_global_id(0);

	//  for(unsigned r2=rank+1; r2<rr; r2++){
		//unsigned count=at(r2, c1,rr);
		unsigned count = matrix[rr*c1 + r2];
		for(unsigned c2=0; c2<cc; c2++){
		  matrix[rr*c2 + r2] = sub( matrix[rr*c2 + r2] , mul( count, matrix[rr*c2 + rank]) );
		  //at(r2,c2,rr) = sub( at(r2,c2,rr) , mul( count, at(rank,c2,rr)) );
		}
  //}

}