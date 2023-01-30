//
// Created by tyler.sloss on 1/23/2023.
//

#ifndef MOBAGEN_WATERERROSIONGENERATOR_H
#define MOBAGEN_WATERERROSIONGENERATOR_H

#include "../GeneratorBase.h"

class WaterErrosionGenerator: public ScenarioGeneratorBase{
 public:
  std::vector<Color32> Generate(int sideSize, float displacement=0) override;
  std::string GetName() override;
};

#endif  // MOBAGEN_WATERERROSIONGENERATOR_H
