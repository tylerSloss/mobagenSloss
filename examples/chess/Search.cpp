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

    vector<MoveState> score = NextPossibleMovesSorted(state);
    //vector<MoveState> scoreH1(score.begin(), score.begin() + score.size() / 2);
    //vector<MoveState> scoreH2(score.begin() + score.size() / 2, score.end());
    
    vector<Move> scoreMoves;
    vector<MoveState> score2;
    for (auto moves : score)
    {
        
        auto possibleState = state;
        possibleState.Move(moves.moves.back().From(), moves.moves.back().To());
        
        score2.append_range(NextPossibleMovesSorted(possibleState, moves.moves));
    }
    /*vector<MoveState> score2H1(score2.begin(), score2.begin() + score2.size() / 2);
    std::sort(score2H1.begin(), score2H1.end());*/
    std::sort(score2.begin(), score2.end());


    /*for (auto moves2 : score2H1)
    {
        
    }*/



    vector<MoveState> score3;
    for (auto moves2 : score2) {
        auto possibleState = state;
        possibleState.Move(moves2.moves.back().From(), moves2.moves.back().To());
        score3.append_range(NextPossibleMovesSorted(possibleState, moves2.moves));
    }
    std::sort(score3.begin(), score3.end());
    //vector<MoveState> score3H1(score3.begin(), score3.begin() + score.size() / 2);

    /*vector<MoveState> sorted; 
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
    for (auto move : possibleMoves)
    {
        auto possibleState = state;
        possibleState.Move(move.From(),move.To());
        auto score = Heuristics::MaterialScore(&possibleState);
        sorted.push_back({possibleState, move, score});
    }
    std::sort(sorted.begin(), sorted.end());*/

    /*vector<Move> l2moves;
    for (auto moves : sorted)
    {

    }*/





    
    if (score3.size() > 0)
    {
        if (state.GetTurn() == PieceColor::Black)
            return score3.front().moves.front();
        else
            return score3.back().moves.front();
    }


    return Move();

}

vector<MoveState> Search::NextPossibleMovesSorted(WorldState& state) { 
   
    vector<Move> bestPossibleMoves;

    vector<MoveState> sorted;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
    for (auto move : possibleMoves) {
        auto possibleState = state;
        possibleState.Move(move.From(), move.To());
        auto score = Heuristics::BoardAnalysis(&possibleState);
        sorted.push_back({possibleState, {move}, (int)score.Score()});
    }
    std::sort(sorted.begin(), sorted.end());
    vector<MoveState> reverseSorted = sorted;
    std::sort(reverseSorted.rbegin(), reverseSorted.rend());
    
    if (sorted.size() > 0) {
        if (state.GetTurn() == PieceColor::Black) {
            Move nextmove = sorted.front().GetFirstMove();
            int movescore = sorted.front().score;
            /*for (auto bestmove : sorted)
            {
                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
            }
            return bestPossibleMoves;*/
            return sorted;
        } else if (state.GetTurn() == PieceColor::White) {
            Move nextmove = sorted.front().GetCurrentMove();
            int movescore = sorted.back().score;
            /*for (auto bestmove : reverseSorted) 
            {
                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
            }
            return bestPossibleMoves;*/
            return reverseSorted;
        }
    }
    
    return vector<MoveState>(); 
}

vector<MoveState> Search::NextPossibleMovesSorted(WorldState& state, vector<Move> prevMoves) { 
    vector<Move> bestPossibleMoves;

    vector<MoveState> sorted;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
    for (auto move : possibleMoves) {
        auto possibleState = state;
        possibleState.Move(move.From(), move.To());
        auto score = Heuristics::MaterialScore(&possibleState);
        MoveState moveState(possibleState,prevMoves, score);
        moveState.AddMoveToMoves(move);
        sorted.push_back(moveState);
    }
    std::sort(sorted.begin(), sorted.end());
    vector<MoveState> reverseSorted = sorted;
    std::sort(reverseSorted.rbegin(), reverseSorted.rend());

    if (sorted.size() > 0) {
        if (state.GetTurn() == PieceColor::Black) {
            Move nextmove = sorted.front().GetFirstMove();
            int movescore = sorted.front().score;
            /*for (auto bestmove : sorted)
            {
                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
            }
            return bestPossibleMoves;*/
            return sorted;
        } else if (state.GetTurn() == PieceColor::White) {
            Move nextmove = sorted.front().GetCurrentMove();
            int movescore = sorted.back().score;
            /*for (auto bestmove : reverseSorted)
            {
                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
            }
            return bestPossibleMoves;*/
            return reverseSorted;
        }
    }
    

    return vector<MoveState>();
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

  /*cout << "attacked: ";
  for (auto item : moves) cout << item.to_string();
  std::cout << endl;*/
  return moves;
}
