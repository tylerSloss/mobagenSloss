#ifndef MOBAGEN_HEURISTICS_H
#define MOBAGEN_HEURISTICS_H

#include <cstdint>
#include "WorldStateFwd.h"  // forward is needed here to avoid collisions
#include "Point2D.h"

struct Heuristics {
public:
  inline float Score() const { return score; };
  static int MaterialScore(WorldState* state);
  static int PiecesThreatened(WorldState* state);
  static int PieceThreatened(WorldState* state, PieceData piece, PieceColor color);

private:
  // positive score means white is winning
  float score;

  static int distanceToCenter(Point2D location);
  static Point2D FindPiece(WorldState* state, PieceData piece, PieceColor color);

public:
  static bool KingInCheck(WorldState* state);
  static bool KingInCheckmate(WorldState* state);
  static Heuristics BoardAnalysis(WorldState* state);
  
};

#endif  // MOBAGEN_HEURISTICS_H
