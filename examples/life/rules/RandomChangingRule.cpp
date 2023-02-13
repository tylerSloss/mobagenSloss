//
// Created by tyler.sloss on 2/13/2023.
//

#include "RandomChangingRule.h"
#include "Random.h"
void RandomChangingRule::Step(World& world) {
  int alive1 = Random::Range(2,5);
  int alive2 = Random::Range(2,5);
  int randCreate = Random::Range(2,5);
  for(int lin = 0; lin<world.SideSize(); lin++){
    for(int col = 0; col<world.SideSize(); col++){
      auto point = Point2D{lin,col};
      auto neighs = CountNeighbors(world,point);
      auto isAlive = world.Get(point);
      if(isAlive) {
        if(neighs==alive1 || neighs==alive2)
          world.SetNext(point, true);
        else
          world.SetNext(point, false);
      }
      else if(neighs==randCreate)
        world.SetNext(point, true);
    }
  }}
int RandomChangingRule::CountNeighbors(World& world, Point2D point) {
  return
      static_cast<int>(world.Get(point + Point2D::UP)) +
      static_cast<int>(world.Get(point + Point2D::UP + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::UP + Point2D::RIGHT)) +
      static_cast<int>(world.Get(point + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::RIGHT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN)) +
      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::RIGHT));
}
