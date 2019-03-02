#ifndef local_puzzles_hpp
#define local_puzzles_hpp

#include "decompose.hpp"
#include "rank.hpp"
#include "ising.hpp"
#include "integral.hpp"

// TBB
#include "rankv1.hpp"
#include "decomposev1.hpp"
#include "isingv1.hpp"
#include "integralv1.hpp"

//OPENCL
#include "integralv2.hpp"
#include "decomposev2.hpp"

//OPT
#include "isingv3.hpp"
#include "integralv3.hpp"
#include "decomposev3.hpp"
#include "rankv3.hpp"


void puzzler::PuzzleRegistrar::UserRegisterPuzzles()
{
  Register("decompose.ref", std::make_shared<puzzler::DecomposePuzzle>());
  Register("integral.ref", std::make_shared<puzzler::IntegralPuzzle>());
  Register("ising.ref", std::make_shared<puzzler::IsingPuzzle>());
  Register("rank.ref", std::make_shared<puzzler::RankPuzzle>());

  //tbb
  Register("rank.tbb", std::make_shared<puzzler::RankProviderv1>());
  Register("decompose.tbb", std::make_shared<puzzler::DecomposeProviderv1>());
  Register("integral.tbb", std::make_shared<puzzler::IntegralProviderv1>());
  Register("ising.tbb", std::make_shared<puzzler::IsingProviderv1>());
  
  //opencl
  Register("integral.opencl", std::make_shared<puzzler::IntegralProviderv2>());
  Register("decompose.opencl", std::make_shared<puzzler::DecomposeProviderv2>());
  
  //opt
  Register("integral.opt", std::make_shared<puzzler::IntegralProviderv3>());
  Register("decompose.opt", std::make_shared<puzzler::DecomposeProviderv3>());
  Register("ising.opt", std::make_shared<puzzler::IsingProviderv3>());
  Register("rank.opt", std::make_shared<puzzler::RankProviderv3>());
 

}


#endif
