#ifndef MOBAGEN_SEARCH_H
#define MOBAGEN_SEARCH_H

#include "WorldState.h"

struct Search {
  // todo: create your algorithm to calculate next move
  static Move NextMove(WorldState& state);
  //static MoveNode NextPossibleMoves(WorldState& state);
  static MoveNode NextPossibleMove(WorldState& state, int lvlsDeep);
  static vector<MoveNode> NextPossibleMovesVector(WorldState& state);
  static vector<MoveNode> NextPossibleMoves(WorldState& state, MoveNode& parent);
  static vector<MoveNode> NextPossibleMovesScored(WorldState& state, MoveNode* parent);
  static vector<MoveNode> NextPossibleMoves(WorldState& state, MoveNode* parent, int lvlsDeep);

  static MoveNode AnalyseMoveTree(MoveNode* nodetree);
  static int AnalyseMoveNode(MoveNode node);


  static Move MinMax(WorldState& state, int levelsDeep);
  /**
   * @param state world state
   * @param currentPlayer calculate the current turn possible moves
   * @return
   */
  static auto ListPlacesKingCannotGo(WorldState& state, PieceColor turn) -> unordered_set<Point2D>;
  static auto ListMoves(WorldState& state, PieceColor turn) -> std::vector<Move>;
};

#endif  // MOBAGEN_SEARCH_H
