#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <string>
#include <lib/DungeonMaker.h>
#include <dungeon/generators/grid_generator.h>
#include "cocos2d.h"

namespace butcher {

class CaveGenerator : public GridGenerator
{
public:
  CaveGenerator();

  virtual Grid generate(const LevelData* data);

private:

  struct GenerationParams
  {
    int r1_cutoff, r2_cutoff;
    int reps;
  };


  Grid grid;
  Grid grid2;

  int fillprob = 40;
  int r1_cutoff = 5, r2_cutoff = 2;
  int size_x = 64, size_y = 20;
  GenerationParams *params;

  GenerationParams *params_set;
  int generations;

  char randpick();
  void initmap();
  void generation();

  std::string toStr();
};

}

#endif // LEVELGENERATOR_H
