//
// Created by tyler.sloss on 1/23/2023.
//
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>
#include "WaterErrosionGenerator.h"
std::string WaterErrosionGenerator::GetName() { return "Water Erosion"; }

std::vector<Color32> WaterErrosionGenerator::Generate(int sideSize,float displacement)
{
  std::vector<Color32> colors;
  //   create your own function for noise generation
  FastNoiseLite base;
  base.SetFractalOctaves(3);
  base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
  base.SetFractalType(FastNoiseLite::FractalType_FBm);
  FastNoiseLite cellular;
  cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
  cellular.SetFractalOctaves(3);

  std::vector<float> elevation;
  std::vector<float> water;
  std::vector<float>material;
  std::vector<float>prevMaterial;
  std::vector<float> island;

  

  for(int l=0; l<sideSize; l++){
    for(int c=0;c<sideSize; c++){
      // island generation example
      float posY = (float)((l - (float)sideSize/2)) / (float)((float)sideSize/2);
      float posX = (float)((c - (float)sideSize/2)) / (float)((float)sideSize/2);

      float islandInfluance = (2 - (abs(posX)) + (abs(posY)))/2;
      islandInfluance *= 255;

      elevation.push_back(((base.GetNoise((float)c,(float)l, displacement*50) +1)/2)*255);
      //float c2 = ((cellular.GetNoise((float)c,(float)l, displacement*50) +1)/2) *255;
    }
  }
 // color
  for (auto e : elevation)
  {
    colors.push_back(Color32(e,e,e));
  }

  std::cout<<colors.size() << std::endl;
  return colors;

}
