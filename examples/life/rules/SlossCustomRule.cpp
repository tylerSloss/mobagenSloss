//
// Created by tyler.sloss on 2/9/2023.
//

#include "SlossCustomRule.h"
void SlossCustomRule::Step(World& world) {
  for(int lin = 0; lin<world.SideSize(); lin++){
    for(int col = 0; col<world.SideSize(); col++){
      auto point = Point2D{lin,col};
      auto neighs = CountNeighbors(world,point);
      auto isAlive = world.Get(point);
      if(isAlive) {
        if(/*neighs==2 || */neighs==3 || neighs==4)
          world.SetNext(point, true);
        else
          world.SetNext(point, false);
      }
      else if(neighs==3)
        world.SetNext(point, true);
    }
  }
}

int SlossCustomRule::CountNeighbors(World& world, Point2D point) {
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