#ifndef MOBAGEN_PIECEBASE_H
#define MOBAGEN_PIECEBASE_H

#include "../World.h"
#include "../WorldState.h"
#include "Point2D.h"
#include <vector>

using namespace std;

class PieceBase {
  // ListPossibleMoves to be used in the interface to draw the possible moves
  virtual vector<Point2D> PossibleMoves(const World& world, Point2D origin) = 0;
  // GenerateStates to be used when we explcout << a << " ";ore

};

#endif  // MOBAGEN_PIECEBASE_H
