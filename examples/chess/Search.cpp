#include <iostream>
#include <algorithm>
#include "Search.h"
#include "Heuristics.h"
#include "WorldState.h"

#include "pieces/Bishop.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"

auto Search::ListMoves(WorldState& state, PieceColor turn) -> std::vector<Move> {
  vector<Move> moves;
  for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      Point2D const location = {column, line};
      auto p = state.PieceAtPosition(location);
      // continue if it is not a valid piece
      if (p.Piece() == PieceType::NONE || p.Piece() == PieceType::WRONG || turn != p.Color() || turn != state.GetTurn())
        continue;
      vector<Move> toAdd;
      // todo: improve this switch
      switch (p.Piece()) {
        case PieceType::Rook:
          toAdd = Move::GenerateListOfMoves(p, location, Rook::AttackMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
          break;
        case PieceType::Bishop:
          toAdd = Move::GenerateListOfMoves(p, location, Bishop::AttackMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
          break;
        case PieceType::Pawn:
          toAdd = Move::GenerateListOfMoves(p, location, Pawn::PossibleMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
          break;
        case PieceType::Queen:
          toAdd = Move::GenerateListOfMoves(p, location, Queen::AttackMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
          break;
        case PieceType::Knight:
          toAdd = Move::GenerateListOfMoves(p, location, Knight::AttackMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
          break;
        case PieceType::King:
          toAdd = Move::GenerateListOfMoves(p, location, King::AttackMoves(state, location));
          moves.insert(moves.end(), toAdd.begin(), toAdd.end());
        default:
          break;
      }
    }
  }
  return moves;
}

Move Search::NextMove(WorldState& state) { 
    //return Move();
  /*
  Innumerate all possible states from the current state,
  for each state calulate score / hueristic
  for every new state go deep and resimulate the state (recursive)

  for pruning maximize the score for your turn
  */
    vector<MoveState> sorted; 
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
    for (auto move : possibleMoves)
    {
        auto possibleState = state;
        possibleState.Move(move.From(),move.To());
        auto score = Heuristics::MaterialScore(&possibleState);
        sorted.push_back({possibleState, move, score});
    }
    std::sort(sorted.begin(), sorted.end());
    if (sorted.size() > 0)
    {
        if (state.GetTurn() == PieceColor::Black) 
        {
            Move nextmove = sorted.front().move;
            int movescore = sorted.front().score;
            return nextmove;
        } 
        else if (state.GetTurn() == PieceColor::White) 
        {
            Move nextmove = sorted.back().move;
            int movescore = sorted.back().score;
            return nextmove;
        }
    }
    return Move();

}

auto Search::ListPlacesKingCannotGo(WorldState& state, PieceColor turn) -> unordered_set<Point2D> {
  unordered_set<Point2D> moves;
  for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      Point2D const location = {column, line};
      auto p = state.PieceAtPosition(location);
      // continue if it is not a valid piece
      if (p.Piece() == PieceType::NONE || p.Piece() == PieceType::WRONG) continue;
      if (turn == p.Color()) continue;
      // todo: improve this switch
      switch (p.Piece()) {
        case PieceType::Rook:
          moves.merge(Rook::CoverMoves(state, location));
          break;
        case PieceType::Bishop:
          moves.merge(Bishop::CoverMoves(state, location));
          break;
        case PieceType::Pawn:
          moves.merge(Pawn::CoverMoves(state, location));
          break;
        case PieceType::Queen:
          moves.merge(Queen::CoverMoves(state, location));
          break;
        case PieceType::Knight:
          moves.merge(Knight::CoverMoves(state, location));
          break;
        case PieceType::King:
          moves.merge(King::CoverMovesNaive(state, location));
          break;
        default:
          break;
      }
    }
  }

  cout << "attacked: ";
  for (auto item : moves) cout << item.to_string();
  std::cout << endl;
  return moves;
}
