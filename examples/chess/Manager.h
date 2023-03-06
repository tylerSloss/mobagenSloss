#ifndef CHESS_MANAGER_H
#define CHESS_MANAGER_H

#include "Texture.h"
#include "WorldState.h"
#include <ColorT.h>
#include <Engine.h>
#include <GameObject.h>
#include <iostream>
#include <map>
#include <unordered_set>

class Manager: GameObject  {
 private:
  WorldState state;
  vector<WorldState> previousStates;
  Point2D selected={INT32_MIN,INT32_MIN};
  unordered_set<Point2D> validMoves;
  map<uint8_t, Texture*> piecePackedToTexture;

 public:
  explicit Manager(Engine* pEngine);
  void Start() override;
  ~Manager();
  void OnGui(ImGuiContext* context) override;
  void OnDraw(SDL_Renderer* renderer) override;

 private:
  Point2D mousePositionToIndex(ImVec2& pos);
  unordered_set<Point2D> getMoves(PieceType t, Point2D point);
  void drawSquare(SDL_Renderer* renderer, Color32& color, SDL_Rect& rect);
  void drawPiece(SDL_Renderer* renderer, PieceData piece, Vector2 location, Vector2 scale);
};

#endif  // CHESS_MANAGER_H
