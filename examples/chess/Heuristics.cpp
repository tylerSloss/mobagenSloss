#include "Heuristics.h"
#include "WorldState.h"
#include "pieces/Pieces.h"

// Material. The easiest one to evaluate: just count up the value of your pieces vs. the value of your opponent's pieces. Pawn = 1, bishop/knight = 3,
//rook = 5, and queen = 9 is generally a good rule.
// Pawn structure. Isolated, passed, chained, backward, doubled, tripled, quadrupled (I have seen this happen): identify them in both your position and in your opponent's. Check also for any open files or half-open files either of you may have as a result of the pawn structure. You can also determine, using the pawn structure, the nature of the position: is it a closed position (the center is blocked by pawns) or an open position (the center is relatively free of pawns, with open files and diagonals)?
// Space. Technically this would be done by counting up the squares controlled by each side, but it's pretty easy to gauge a spatial advantage by examining the pawns, which tend to mark out space both by forming a frontier between the two sides and by denying squares to the opponent. Weak squares / strong squares. Are there any squares that can't be protected by pawns? Also note that your weak squares are only really weak if they can be turned into strong squares for the opponent.
// Minor piece imbalance. Easy to take stock of; difficult to evaluate. Do you have the same minor pieces (bishops and knights) as your opponent, or is it a bishop vs. knight situation? If it's bishop vs. bishop, are they on squares of the same color? Two bishops vs. knight and bishop? You get the idea.
// Development. Who has more pieces out?
// King safety. Are there any structural weaknesses in either king's position? Does one player have a bunch of attacking pieces near the opponent's king?
// Initiative. This is probably the most difficult of all of them to figure out: the initiative is the ability to dictate the tempo of the game. Generally speaking, the initiative belongs to the player who is able to make threats, positional or tactical. The player with the initiative makes the opponent's plan subservient to his own--you will carry out your plan only if I want you to, and only on my terms.


Heuristics Heuristics::BoardAnalysis(WorldState* state) {
  // todo: write your own heuristic here
  Heuristics heuristicScore;
  int score = 0;
  score += MaterialScore(state);
  score -= PiecesThreatened(state);
  heuristicScore.score = score;
  return heuristicScore;
  //return {};
}
int Heuristics::MaterialScore(WorldState* state) {
  int score = 0;
  for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      auto location = Point2D(column, line);
      auto piece = state->PieceAtPosition(location);
      auto pieceScore = 0;
      int moves;
      switch (piece.Piece()) {
        case PieceType::King:
          pieceScore += 1000;                                        // piece value
          pieceScore += King::AttackMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          if (KingInCheck(state))
          {
            if (KingInCheckmate(state)) 
                pieceScore += 750;
            else
                pieceScore += 500;
          }
              
          // todo: king safety, check, draw and mate
          break;
        case PieceType::Queen:
          pieceScore += 90;                                           // piece value
          pieceScore += Queen::AttackMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          break;
        case PieceType::Rook:
          pieceScore += 50;                                          // piece value
          pieceScore += Rook::AttackMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          break;
        case PieceType::Knight:
          pieceScore += 35;                                            // piece value
          pieceScore += Knight::AttackMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          break;
        case PieceType::Bishop:
          pieceScore += 30;                                            // piece value
          pieceScore += Bishop::AttackMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          break;
        case PieceType::Pawn:
          pieceScore += 10;                                      // piece value
          moves = Pawn::PossibleMoves(*state, location).size();  // mobility
          pieceScore += distanceToCenter(location);
          pieceScore += moves;
          if (moves == 0) pieceScore -= 1;                          // blocked
          pieceScore -= 2 * Pawn::CountDoubles(*state, location);   // doubled
          if (Pawn::IsIsolated(*state, location)) pieceScore -= 5;  // isolation
          break;
        default:
          continue;
      }
      if (piece.Color() == PieceColor::Black)
        score -= pieceScore;
      else
        score += pieceScore;
    }
  }
  return score;
}

int Heuristics::PiecesThreatened(WorldState* state) {
  int score = 0;
  for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      auto location = Point2D(column, line);
      auto piece = state->PieceAtPosition(location);
      auto pieceScore = 0;
      int moves;
      switch (piece.Piece()) {
        case PieceType::King:
          if (KingInCheck(state)) pieceScore += 500;
          break;
        case PieceType::Queen:
          if (piece.Color() == PieceColor::Black) {
            if (PieceThreatened(state, piece, PieceColor::Black)) {
                pieceScore += 90;
                pieceScore += Queen::AttackMoves(*state, location).size();  // mobility
                pieceScore += distanceToCenter(location);
            }
          }
          if (piece.Color() == PieceColor::White) {
            if (PieceThreatened(state, piece, PieceColor::White)) {
                pieceScore += 90;
                pieceScore += Queen::AttackMoves(*state, location).size();  // mobility
                pieceScore += distanceToCenter(location);
            }
          }
          break;
        case PieceType::Rook:
          if (piece.Color() == PieceColor::Black) {
            if (PieceThreatened(state, piece, PieceColor::Black)) {
                pieceScore += 50;
                pieceScore += Rook::AttackMoves(*state, location).size();  // mobility
                pieceScore += distanceToCenter(location);
            }
            if (piece.Color() == PieceColor::White) {
                if (PieceThreatened(state, piece, PieceColor::White)) {
                  pieceScore += 50;
                  pieceScore += Rook::AttackMoves(*state, location).size();  // mobility
                  pieceScore += distanceToCenter(location);
                }
            }
            break;
            case PieceType::Knight:
                if (piece.Color() == PieceColor::Black) {
                  if (PieceThreatened(state, piece, PieceColor::Black)){
                      pieceScore += 50;
                      pieceScore += Knight::AttackMoves(*state, location).size();  // mobility
                      pieceScore += distanceToCenter(location);
                    }
                }
                if (piece.Color() == PieceColor::White) {
                  if (PieceThreatened(state, piece, PieceColor::White)) {
                    pieceScore += 50;
                    pieceScore += Knight::AttackMoves(*state, location).size();  // mobility
                    pieceScore += distanceToCenter(location);
                  }
                }
                break;
            case PieceType::Bishop:
                if (piece.Color() == PieceColor::Black) {
                  if (PieceThreatened(state, piece, PieceColor::Black)) {
                    pieceScore += 30;
                    pieceScore += Bishop::AttackMoves(*state, location).size();  // mobility
                    pieceScore += distanceToCenter(location);
                  }
                }
                if (piece.Color() == PieceColor::White) {
                  if (PieceThreatened(state, piece, PieceColor::White)) {
                    pieceScore += 30;
                    pieceScore += Bishop::AttackMoves(*state, location).size();  // mobility
                    pieceScore += distanceToCenter(location);
                  }
                }
                break;
            case PieceType::Pawn:
                if (piece.Color() == PieceColor::Black) {
                  if (PieceThreatened(state, piece, PieceColor::Black)) {
                    pieceScore += 10;
                    pieceScore += Pawn::AttackMoves(*state, location).size();  // mobility
                    pieceScore += distanceToCenter(location);
                  }
                }
                else if (piece.Color() == PieceColor::White) {
                  if (PieceThreatened(state, piece, PieceColor::White)) {
                    pieceScore += 10;
                    pieceScore += Pawn::AttackMoves(*state, location).size();  // mobility
                    pieceScore += distanceToCenter(location);
                  }
                }
                break;
            default:
                continue;
          }
          if (piece.Color() == PieceColor::Black)
            score -= pieceScore;
          else
            score += pieceScore;
      }
    }

    return score;
  }
}

int Heuristics::PieceThreatened(WorldState* state, PieceData tPiece, PieceColor color) {
    auto tPiecePoint = FindPiece(state, tPiece, color); 
    
    for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      auto location = Point2D(column, line);
      auto piece = state->PieceAtPosition(location);
      auto pieceScore = 0;
      int moves;
      if (piece != tPiece)
      {
        switch (piece.Piece()) {
          /*case PieceType::King:          // Use KingInCheck or KingInCheckmate instead
            if (KingInCheck(state)) pieceScore += 500;
            break;*/
          case PieceType::Queen:
            if (piece.Color() != color) {
                if (Queen::AttackMoves(*state, location).contains(tPiecePoint)) 
                    return true;
            }
            break;
          case PieceType::Rook:
            if (piece.Color() != color) {
                if (Rook::AttackMoves(*state, location).contains(tPiecePoint)) 
                    return true;
            }
            
            break;
          case PieceType::Knight:
            if (piece.Color() != color) {
                if (Knight::AttackMoves(*state, location).contains(tPiecePoint)) 
                    return true;
            }
            
            break;
          case PieceType::Bishop:
            if (piece.Color() != color) {
                if (Bishop::AttackMoves(*state, location).contains(tPiecePoint))
                    return true;
            }
            
            break;
          case PieceType::Pawn:
            if (piece.Color() != color) {
                if (Pawn::AttackMoves(*state, location).contains(tPiecePoint)) 
                    return true;
            }
            
            break;
          default:
            continue;
        }
      }
      
    }
    }

    return false;

    return 0;
}

int Heuristics::distanceToCenter(Point2D location) {
  // todo: improve. I am unsure if this is the best way. I am doubling because I am targeting the center.
  auto doubled = Point2D(location.x * 2 - 7, location.y * 2 - 7);
  auto maxed = 3 - (std::min(std::abs(doubled.x), std::abs(doubled.y)) - 1) / 2;
  return maxed;
}

bool Heuristics::KingInCheck(WorldState* state) { 

    auto blackKing = King::FindKing(*state, PieceColor::Black);
    auto whiteKing = King::FindKing(*state, PieceColor::White);

    for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      auto location = Point2D(column, line);
      auto piece = state->PieceAtPosition(location);
      auto pieceScore = 0;
      int moves;
      switch (piece.Piece()) {
        //case PieceType::King:
        //  pieceScore += 1000;                                        // piece value
        //  pieceScore += King::AttackMoves(*state, location).size();  // mobility
        //  pieceScore += distanceToCenter(location);
        //  // todo: king safety, check, draw and mate
        //  break;
        case PieceType::Queen:
            if (piece.Color() == PieceColor::Black)
            {
                if (Queen::AttackMoves(*state, location).contains(whiteKing)) return true;
            }
            if (piece.Color() == PieceColor::White) {
                if (Queen::AttackMoves(*state, location).contains(blackKing)) return true;
            }
          break;
        case PieceType::Rook:
          if (piece.Color() == PieceColor::Black) {
                if (Rook::AttackMoves(*state, location).contains(whiteKing)) return true;
          }
          if (piece.Color() == PieceColor::White) {
                if (Rook::AttackMoves(*state, location).contains(blackKing)) return true;
          }
          break;
        case PieceType::Knight:
          if (piece.Color() == PieceColor::Black) {
                if (Knight::AttackMoves(*state, location).contains(whiteKing)) return true;
          }
          if (piece.Color() == PieceColor::White) {
                if (Knight::AttackMoves(*state, location).contains(blackKing)) return true;
          }
          break;
        case PieceType::Bishop:
          if (piece.Color() == PieceColor::Black) {
                if (Bishop::AttackMoves(*state, location).contains(whiteKing)) return true;
          }
          if (piece.Color() == PieceColor::White) {
                if (Bishop::AttackMoves(*state, location).contains(blackKing)) return true;
          }
          break;
        case PieceType::Pawn:
          if (piece.Color() == PieceColor::Black) {
                if (Pawn::AttackMoves(*state, location).contains(whiteKing)) return true;
          }
          if (piece.Color() == PieceColor::White) {
                if (Pawn::AttackMoves(*state, location).contains(blackKing)) return true;
          }
          break;
        default:
          continue;
      }
      
    }
  }

    return false; 
}

Point2D Heuristics::FindPiece(WorldState* state, PieceData piece, PieceColor color) { 
    
    for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      Point2D location = {column, line};
      auto p = state->PieceAtPosition(location);
      if (p.Color() != color) continue;
      if (p.Piece() == piece.Piece()) return location;
    }
    }
    return {};
    
}

bool Heuristics::KingInCheckmate(WorldState* state) {
    
    if (KingInCheck(state))
    {
        if (state->GetTurn() == PieceColor::Black)
        {
           auto kingMoves = King::AttackMoves(*state, King::FindKing(*state, PieceColor::Black));
           if (kingMoves.size() <= 0)
           {
                return true;
            }
        }
        if (state->GetTurn() == PieceColor::White) {
            auto kingMoves = King::AttackMoves(*state, King::FindKing(*state, PieceColor::White));
            if (kingMoves.size() <= 0) {
                return true;
            }
        }
    }
    
    return false; 
}
