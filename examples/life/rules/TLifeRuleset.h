//
// Created by tyler.sloss on 2/13/2023.
//

#ifndef MOBAGEN_TLIFERULESET_H
#define MOBAGEN_TLIFERULESET_H
#include "../RuleBase.h"

class TLifeRuleset : public RuleBase {
 public:
  explicit TLifeRuleset()=default;
  ~TLifeRuleset() override =default;
  std::string GetName() override{return "TLifeRuleset";}
  void Step(World& world) override;
  int CountNeighbors(World& world, Point2D point);
  int CountOrthagnalNeighbors(World& world, Point2D point);
  GameOfLifeTileSetEnum GetTileSet() override{return GameOfLifeTileSetEnum::Square;};
};

#endif  // MOBAGEN_TLIFERULESET_H
