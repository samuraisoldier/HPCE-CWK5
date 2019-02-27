__kernel void iteration(__global uint *edges, //0
						__global const float *current, //1
						__global float *next) //2

{
	uint i = get_global_id(0);
	for(unsigned j=0; j<5; j++){
	  int dst=edges[j];
	  next[dst] += current[i] /5;
	}
 
}