__kernel void iteration(//ILog *log, 0
 						unsigned n, //1
						__global uint (*edges)[100], //2
						__global const float *current, //3
						__global float *next) //4
						//__global int *edgesSizes) //5
{

  for(unsigned i=0; i<n; i++){
	next[i]=0;
  }
  
  for(unsigned i=0; i<n; i++){
	for(unsigned j=0; j<5; j++){
	  int dst=edges[i][j];
	  next[dst] += current[i] /5;
	}
  }

  double total=0;
  for(unsigned i=0; i<n; i++){
	next[i] = (current[i] * 0.3  + next[i] * 0.7 );
	total += next[i];
  }
  
  //log->LogVerbose("  total=%g", total);
  
  for(unsigned i=0; i<n; i++){
	next[i] /= total;
	//log->LogVerbose("    c[%u] = %g", i, next[i]);
  }
}