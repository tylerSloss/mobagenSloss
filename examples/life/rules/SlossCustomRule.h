//
// Created by tyler.sloss on 2/9/2023.
//

#ifndef MOBAGEN_SLOSSCUSTOMRULE_H
#define MOBAGEN_SLOSSCUSTOMRULE_H
#include "../RuleBase.h"

class SlossCustomRule : public RuleBase{
 public:
  explicit SlossCustomRule()=default;
  ~SlossCustomRule() override =default;
  std::string GetName() override{return "SlossCustomRule";}
  void Step(World& world) override;
  int CountNeighbors(World& world, Point2D point);
  GameOfLifeTileSetEnum GetTileSet() override{return GameOfLifeTileSetEnum::Square;};
};

#endif
