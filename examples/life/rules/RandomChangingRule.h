//
// Created by tyler.sloss on 2/13/2023.
//

#ifndef MOBAGEN_RANDOMCHANGINGRULE_H
#define MOBAGEN_RANDOMCHANGINGRULE_H
#include "../RuleBase.h"

class RandomChangingRule : public RuleBase{
   public:
    explicit RandomChangingRule()=default;
    ~RandomChangingRule() override =default;
    std::string GetName() override{return "RandomChangingRule";}
    void Step(World& world) override;
    int CountNeighbors(World& world, Point2D point);
    GameOfLifeTileSetEnum GetTileSet() override{return GameOfLifeTileSetEnum::Square;};
  };

#endif  // MOBAGEN_RANDOMCHANGINGRULE_H
