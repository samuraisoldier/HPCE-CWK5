__kernel void iteration(ILog *log, //0
 						unsigned n, //1
						const std::vector<std::vector<uint32_t> > &edges, //2
						const float *current, //3
						float *next) //4
{

  for(unsigned i=0; i<n; i++){
	next[i]=0;
  }
  
  for(unsigned i=0; i<n; i++){
	for(unsigned j=0; j<edges[i].size(); j++){
	  int dst=edges[i][j];
	  next[dst] += current[i] / edges[i].size();
	}
  }

  double total=0;
  for(unsigned i=0; i<n; i++){
	next[i] = (current[i] * 0.3  + next[i] * 0.7 );
	total += next[i];
  }
  
  log->LogVerbose("  total=%g", total);
  
  for(unsigned i=0; i<n; i++){
	next[i] /= total;
	log->LogVerbose("    c[%u] = %g", i, next[i]);
  }
}