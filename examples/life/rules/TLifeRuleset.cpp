//
// Created by tyler.sloss on 2/13/2023.
//

#include "TLifeRuleset.h"
void TLifeRuleset::Step(World& world) {
  for(int lin = 0; lin<world.SideSize(); lin++){
    for(int col = 0; col<world.SideSize(); col++){
      auto point = Point2D{lin,col};
      auto neighs = CountNeighbors(world,point);
      auto orthNeighs = CountOrthagnalNeighbors(world,point);
      auto horzNeighs = CountHorisontalNeighbors(world,point);
      auto vertNeighs = CountVerticalNeighbors(world,point);
      auto diagnolNeighs = CountDiagnolNeighbors(world,point);
      auto lrDiagnolNeighs = CountLRDiagnolNeighbors(world,point);
      auto rlDiagnolNeighs = CountRLDiagnolNeighbors(world,point);
      auto isAlive = world.Get(point);
//      if(isAlive) {
//        if (orthNeighs==3)
//          world.SetNext(point, false);
//        else if(neighs==2 || neighs==3 || neighs==4)
//          world.SetNext(point, true);
//        else
//          world.SetNext(point, false);
//      }
//      else {
//        if(neighs==3) {
//          world.SetNext(point, true);
//        }
//        else
//          world.SetNext(point, false);
//      }

      if(isAlive) {
        if(neighs==2 || neighs==3)
          world.SetNext(point, true);
        else
          world.SetNext(point, false);
      }
      else if(neighs==3)
        world.SetNext(point, true);

      if (neighs==4 && isAlive && (rlDiagnolNeighs==2 || lrDiagnolNeighs==2))
        world.SetNext(point, true);

      if (isAlive && (horzNeighs==2 || vertNeighs==2))
        world.SetNext(point,false);

      if (neighs==3)
        world.SetNext(point, true);






    }
  }
}



int TLifeRuleset::CountNeighbors(World& world, Point2D point) {
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
int TLifeRuleset::CountOrthagnalNeighbors(World& world, Point2D point) {
  return
      static_cast<int>(world.Get(point + Point2D::UP)) +
      static_cast<int>(world.Get(point + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::RIGHT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN));

}

int TLifeRuleset::CountHorisontalNeighbors(World& world, Point2D point) {
  return
      static_cast<int>(world.Get(point + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::RIGHT));

}
int TLifeRuleset::CountVerticalNeighbors(World& world, Point2D point) {
  return
      static_cast<int>(world.Get(point + Point2D::UP)) +
      static_cast<int>(world.Get(point + Point2D::DOWN));
}

int TLifeRuleset::CountDiagnolNeighbors(World& world, Point2D point) {
  return
      static_cast<int>(world.Get(point + Point2D::UP + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::UP + Point2D::RIGHT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::LEFT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::RIGHT));
}

int TLifeRuleset::CountLRDiagnolNeighbors(World& world, Point2D point) { //Left to right diagnol
  return
      static_cast<int>(world.Get(point + Point2D::UP + Point2D::LEFT)) +


      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::RIGHT));
}

int TLifeRuleset::CountRLDiagnolNeighbors(World& world, Point2D point) { //Right to left diagnol
  return

      static_cast<int>(world.Get(point + Point2D::UP + Point2D::RIGHT)) +
      static_cast<int>(world.Get(point + Point2D::DOWN + Point2D::LEFT));
}