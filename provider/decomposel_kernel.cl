__kernel  void decompose(ILog *log, 
						unsigned rr, 
						unsigned cc, 
						unsigned p, 
						uint32_t *matrix) 
    {
      auto at = [=](unsigned r, unsigned c) -> uint32_t &{
        assert(r<rr && c<cc);
        return matrix[rr*c+r];
      };

      dump(log, Log_Debug, rr, cc, matrix);

      unsigned rank=0;
      for(unsigned c1=0; c1<cc; c1++){
        unsigned r1=rank;
        while(r1<rr && at(r1,c1)==0){
          ++r1;
        }

        if(r1!=rr){
          unsigned pivot=at(r1,c1);
          for(unsigned c2=0; c2<cc; c2++){
            std::swap( at(r1,c2), at(rank,c2) );
            at(rank,c2)=div( at(rank,c2) , pivot );
          }

          for(unsigned r2=rank+1; r2<rr; r2++){
            unsigned count=at(r2, c1);
            for(unsigned c2=0; c2<cc; c2++){
              at(r2,c2) = sub( at(r2,c2) , mul( count, at(rank,c2)) );
            }
          }

          ++rank;
        }

        dump(log, Log_Debug, rr, cc, matrix);
      }
    }
	
	
    uint32_t make_bit(uint32_t seed, uint32_t input) const
    {
      const uint32_t PRIME32_1  = 2654435761U;
      const uint32_t PRIME32_2 = 2246822519U;
      seed += input * PRIME32_2;
      seed  = (seed<<13) | (seed>>(32-13));
      seed *= PRIME32_1;
      return seed % P;
    }

    unsigned mul(unsigned a, unsigned b) const
    { return (a*b)%P; }

    unsigned sub(unsigned a, unsigned b) const
    { return (P+a-b)%P; }

    unsigned mul_inv(unsigned x) const
    {
      assert(x!=0);
      for(unsigned i=1; i<P; i++){
        if( ((i*x)%P) == 1 ){
          return i;
        }
      }
      assert(0);
      return 0;
    }

    unsigned div(unsigned a, unsigned b) const
    { return mul(a, mul_inv(b) ); }