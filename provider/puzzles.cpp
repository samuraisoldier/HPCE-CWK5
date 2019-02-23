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

void puzzler::PuzzleRegistrar::UserRegisterPuzzles()
{
  Register("decompose.ref", std::make_shared<puzzler::DecomposePuzzle>());
  Register("integral.ref", std::make_shared<puzzler::IntegralPuzzle>());
  Register("ising.ref", std::make_shared<puzzler::IsingPuzzle>());
  Register("rank.ref", std::make_shared<puzzler::RankPuzzle>());

  // TODO: Register more engines!
  Register("rankv1.ref", std::make_shared<puzzler::RankProviderv1>());
  Register("decomposev1.ref", std::make_shared<puzzler::DecomposeProviderv1>());
  Register("integralv1.ref", std::make_shared<puzzler::IntegralProviderv1>());
  Register("isingv1.ref", std::make_shared<puzzler::IsingProviderv1>());
  // Note that you can register the same engine twice under different names, for
  // example you could register the same engine for "ising.tbb" and "ising.opt"
}


#endif
