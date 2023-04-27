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

    return MinMax(state, 1);


    return Move();

}

//MoveNode Search::NextPossibleMoves(WorldState& state) { return MoveNode(); }

MoveNode Search::NextPossibleMove(WorldState& state, int lvlsDeep) { 
    
    MoveNode base = {state, {}, nullptr, {}, 0};

     // Go Deep

    
    vector<MoveNode> sorted;
    //vector<Move> possibleMoves = ListMoves(state, state.GetTurn());

    
    base.AddChildren(NextPossibleMoves(base.state, &base, lvlsDeep));
    

    //return base;


    

    // Accend the Tree to find best move
    MoveNode bestMove = AnalyseMoveTree(&base);

    return bestMove;
    
    //return MoveNode(); 
}

vector<MoveNode> Search::NextPossibleMovesVector(WorldState& state) { 
    
    MoveNode base = {state, {}, nullptr, {}, 0};
    vector<MoveNode> sorted;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());

    for (auto move : possibleMoves) {
        auto possibleState = state;
        possibleState.Move(move.From(), move.To());
        sorted.push_back({possibleState, {}, &base, move, 0});
        base.AddChild({possibleState, {}, &base, move, 0});
    }
    
    return sorted;
    
    return vector<MoveNode>(); }

vector<MoveNode> Search::NextPossibleMoves(WorldState& state, MoveNode& parent) { 
    
     //MoveNode base = {state, {}, nullptr, 0};
    vector<MoveNode> sorted;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());

    for (auto move : possibleMoves) {
        auto possibleState = state;
        possibleState.Move(move.From(), move.To());
        sorted.push_back({possibleState, {}, &parent, move, 0});
        
    }

    return sorted;
    
    
    return vector<MoveNode>(); }

vector<MoveNode> Search::NextPossibleMovesScored(WorldState& state, MoveNode* parent) { 
    vector<MoveNode> sorted;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());

    for (auto move : possibleMoves) {
        auto possibleState = state;
        possibleState.Move(move.From(), move.To());
        auto score = Heuristics::BoardAnalysis(&possibleState);
        
        sorted.push_back({possibleState, {}, parent, move, (int)score.Score()});
    }

    if (state.GetTurn() == PieceColor::White)
    {
        sort(sorted.begin(), sorted.end());
    }
    if (state.GetTurn() == PieceColor::Black)
    {
        sort(sorted.rbegin(), sorted.rend());
    }

    return sorted;
    
}

vector<MoveNode> Search::NextPossibleMoves(WorldState& state, MoveNode* parent, int lvlsDeep) { 
    

    vector<MoveNode> children;
    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());

    if (lvlsDeep > 1) {
        for (auto move : possibleMoves) {
          auto possibleState = state;
          possibleState.Move(move.From(), move.To());
          MoveNode child = {possibleState, {}, parent, move, 0};
          // parent->AddChild(child);
          children.push_back(child);
          child.AddChildren(NextPossibleMoves(possibleState, &child, lvlsDeep - 1));
        }
    } else if (lvlsDeep <= 1) {
        children = NextPossibleMovesScored(parent->state,parent);
    }

    

    return children;
    
    return vector<MoveNode>();
}

MoveNode Search::AnalyseMoveTree(MoveNode* nodetree) {
    
    MoveNode movenode = *nodetree;

    for (auto child : nodetree->children) {
        //auto currentChild = child;
        while (child.score <= 0 && !child.children.empty()) {
             AnalyseMoveTree(&child.GetCurrentChild()); 
            
        }
        
        movenode.ChangeScore(AnalyseMoveNode(movenode));
    }

    if (movenode.state.GetTurn() == PieceColor::White)
    {
        sort(movenode.children.rbegin(), movenode.children.rend());
        return movenode.GetFirstChild();
    }
    if (movenode.state.GetTurn() == PieceColor::Black)
    {
        sort(movenode.children.begin(), movenode.children.end());
        return movenode.GetFirstChild();
        
    }

    

    

    return movenode.GetCurrentChild();

    
}

int Search::AnalyseMoveNode(MoveNode node) { 
    
    int score = 0;


    for (auto child : node.children)
    {
        if (child.state.GetTurn() == PieceColor::Black)
        {
            if (child.score <= score)
            {
                score = child.score;
            }
        } 
        else if (child.state.GetTurn() == PieceColor::White)
        {
            if (child.score >= score) {
                score = child.score;
            }
        }
    }
    
    return score;
    
    return 0; }



//vector<MoveState> Search::NextPossibleMovesSorted(WorldState& state) { 
//   
//    vector<Move> bestPossibleMoves;
//
//    vector<MoveState> sorted;
//    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
//    for (auto move : possibleMoves) {
//        auto possibleState = state;
//        possibleState.Move(move.From(), move.To());
//        auto score = Heuristics::BoardAnalysis(&possibleState);
//        sorted.push_back({possibleState, {move}, (int)score.Score()});
//    }
//    std::sort(sorted.begin(), sorted.end());
//    vector<MoveState> reverseSorted = sorted;
//    std::sort(reverseSorted.rbegin(), reverseSorted.rend());
//    
//    if (sorted.size() > 0) {
//        if (state.GetTurn() == PieceColor::Black) {
//            Move nextmove = sorted.front().GetFirstMove();
//            int movescore = sorted.front().score;
//            /*for (auto bestmove : sorted)
//            {
//                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
//            }
//            return bestPossibleMoves;*/
//            return sorted;
//        } else if (state.GetTurn() == PieceColor::White) {
//            Move nextmove = sorted.front().GetCurrentMove();
//            int movescore = sorted.back().score;
//            /*for (auto bestmove : reverseSorted) 
//            {
//                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
//            }
//            return bestPossibleMoves;*/
//            return reverseSorted;
//        }
//    }
//    
//    return vector<MoveState>(); 
//}
//
//vector<MoveState> Search::NextPossibleMovesSorted(WorldState& state, vector<Move> prevMoves) { 
//    vector<Move> bestPossibleMoves;
//
//    vector<MoveState> sorted;
//    vector<Move> possibleMoves = ListMoves(state, state.GetTurn());
//    for (auto move : possibleMoves) {
//        auto possibleState = state;
//        possibleState.Move(move.From(), move.To());
//        auto score = Heuristics::MaterialScore(&possibleState);
//        MoveState moveState(possibleState,prevMoves, score);
//        moveState.AddMoveToMoves(move);
//        sorted.push_back(moveState);
//    }
//    std::sort(sorted.begin(), sorted.end());
//    vector<MoveState> reverseSorted = sorted;
//    std::sort(reverseSorted.rbegin(), reverseSorted.rend());
//
//    if (sorted.size() > 0) {
//        if (state.GetTurn() == PieceColor::Black) {
//            Move nextmove = sorted.front().GetFirstMove();
//            int movescore = sorted.front().score;
//            /*for (auto bestmove : sorted)
//            {
//                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
//            }
//            return bestPossibleMoves;*/
//            return sorted;
//        } else if (state.GetTurn() == PieceColor::White) {
//            Move nextmove = sorted.front().GetCurrentMove();
//            int movescore = sorted.back().score;
//            /*for (auto bestmove : reverseSorted)
//            {
//                bestPossibleMoves.push_back(bestmove.GetCurrentMove());
//            }
//            return bestPossibleMoves;*/
//            return reverseSorted;
//        }
//    }
//    
//
//    return vector<MoveState>();
//}

Move Search::MinMax(WorldState& state, int levelsDeep) { 

    /*vector<vector<MoveNode>> levels;
    vector<MoveNode> scoredLevel;

    for (int i = 0; i < levelsDeep; i++)
    {
        levels.emplace_back(new vector<MoveNode>);
    }

    levels[0].append_range(NextPossibleMoves(state));

    for (int i = 0; i + 1 < levelsDeep - 1; i++)
    {
        for (auto move : levels[i]) {
            levels[i + 1].append_range(NextPossibleMoves(move.state));
        }
    }

    for (auto move : levels[levelsDeep - 1]) {
        scoredLevel.append_range(NextPossibleMovesScored(move.state, move));
    }*/

   
    MoveNode bestMove = NextPossibleMove(state, levelsDeep);

    return bestMove.move;

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

  /*cout << "attacked: ";
  for (auto item : moves) cout << item.to_string();
  std::cout << endl;*/
  return moves;
}
