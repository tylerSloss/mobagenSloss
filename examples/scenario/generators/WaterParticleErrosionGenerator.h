//
// Created by tyler.sloss on 1/30/2023.
//

#ifndef MOBAGEN_WATERPARTICLEERROSIONGENERATOR_H
#define MOBAGEN_WATERPARTICLEERROSIONGENERATOR_H

#include "../GeneratorBase.h"
#include <glm/vec3.hpp>
#include <glm/gtx/normal.hpp>

class WaterParticleErrosionGenerator: public ScenarioGeneratorBase {
 private:
  float heights[2048][2048];
  int sideSizeCached=0;
  glm::vec3 surfaceNormal(int i, int j);
  glm::vec2 surfaceNormalVec2(int i, int j);
  std::vector<Color32> heightsToColor();
  void generateRandomHeights();
  void Erode(float dt);
  float minVol = 0.01;
  float density = 1;
  float friction = 0.05;
  float depositionRate = 0.1;
  float evapRate = 0.001;
  float lastUpdate=0;
 public:
  std::vector<Color32> Generate(int sideSize, float displacement=0) override;
  std::string GetName() override;
};

struct WaterParticle{
  //Construct Particle at Position
  WaterParticle(glm::vec2 _pos){ pos = _pos; }

  glm::vec2 pos;
  glm::vec2 speed = glm::vec2(0.0);

  float volume = 1.0;   //This will vary in time
  float sediment = 0.0; //Fraction of Volume that is Sediment!
};


#endif  // MOBAGEN_WATERPARTICLEERROSIONGENERATOR_H
