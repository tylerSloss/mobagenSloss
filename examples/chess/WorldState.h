#ifndef CHESS_WORLDSTATE_H
#define CHESS_WORLDSTATE_H

#include "Point2D.h"
#include <vector>
using namespace std;

enum class PieceType: uint8_t {
  NONE =      0b0000, // debug purposes
  King =      0b0001,
  Queen =     0b0010,
  Bishop =    0b0011,
  Knight =    0b0100,
  Tower =     0b0101,
  Pawn =      0b0110,
  PIECEMASK = 0b0111, // debug purposes
  WRONG     = 0b0111
};

enum class PieceColor: uint8_t { // chess color to avoid colliding with namespace from engine
  Black =     0b0000,
  White =     0b1000,
  COLORMASK = 0b1000,
  NONE =      0b0000
};

struct PieceBase;

struct WorldState {
 private:
  PieceColor turn;
  uint16_t score; // to store evaluation heuristics to be used in AI branch and cut algorithms
//  vector<PieceState> board;
  uint8_t state[32];

 public:
  PieceColor GetTurn(){return turn;};
  PieceBase PieceAtPosition(Point2D pos);
  void SetPieceAtPosition(PieceBase piece, Point2D pos);
  void Reset();
  string toString();
};


struct PieceBase {
  PieceBase(PieceType type, PieceColor color): color(color), piece(type){}
  
  PieceType piece;
  PieceColor color;
  
  // ListPossibleMoves to be used in the interface to draw the possible moves
  virtual vector<Point2D> PossibleMoves(WorldState* world, const Point2D& origin){};
  // GenerateStates to be used when we explcout << a << " ";ore
  virtual vector<WorldState> GenerateStates(WorldState* world, const Point2D& origin){};

  static inline PieceBase Empty() {return {PieceType::NONE, PieceColor::NONE};}
  static inline uint8_t   Pack(PieceBase piece){return (uint8_t)piece.piece | (uint8_t)piece.color;};
  static inline PieceBase UnPack(uint8_t data){return {(PieceType)(data & (uint8_t)PieceType::PIECEMASK), (PieceColor)(data & (uint8_t)PieceColor::COLORMASK)};};
  char toChar();
};

#endif  // CHESS_WORLDSTATE_H
