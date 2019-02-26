#ifndef local_puzzles_hpp
#define local_puzzles_hpp

#include "decompose.hpp"
#include "rank.hpp"
#include "ising.hpp"
#include "integral.hpp"

// TODO: include your engine headers
#include "rankv1.hpp"
#include "decomposev1.hpp"
#include "isingv1.hpp"
#include "integralv1.hpp"

#include "integralv3.hpp"

void puzzler::PuzzleRegistrar::UserRegisterPuzzles()
{
  Register("decompose.ref", std::make_shared<puzzler::DecomposePuzzle>());
  Register("integral.ref", std::make_shared<puzzler::IntegralPuzzle>());
  Register("ising.ref", std::make_shared<puzzler::IsingPuzzle>());
  Register("rank.ref", std::make_shared<puzzler::RankPuzzle>());

  // TODO: Register more engines!
  //tbb
  Register("rank.tbb", std::make_shared<puzzler::RankProviderv1>());
  Register("decompose.tbb", std::make_shared<puzzler::DecomposeProviderv1>());
  Register("integral.tbb", std::make_shared<puzzler::IntegralProviderv1>());
  Register("ising.tbb", std::make_shared<puzzler::IsingProviderv1>());
  //opencl
  Register("integral.opencl", std::make_shared<puzzler::IntegralProviderv3>());
  // Note that you can register the same engine twice under different names, for
  // example you could register the same engine for "ising.tbb" and "ising.opt"
}


#endif
