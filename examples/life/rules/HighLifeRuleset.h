//
// Created by tyler.sloss on 2/13/2023.
//

#ifndef MOBAGEN_HIGHLIFERULESET_H
#define MOBAGEN_HIGHLIFERULESET_H
#include "../RuleBase.h"

class HighLifeRuleset : public RuleBase {
 public:
  explicit HighLifeRuleset()=default;
  ~HighLifeRuleset() override =default;
  std::string GetName() override{return "HighLifeRuleset";}
  void Step(World& world) override;
  int CountNeighbors(World& world, Point2D point);
  GameOfLifeTileSetEnum GetTileSet() override{return GameOfLifeTileSetEnum::Square;};
};

#endif  // MOBAGEN_HIGHLIFERULESET_H
