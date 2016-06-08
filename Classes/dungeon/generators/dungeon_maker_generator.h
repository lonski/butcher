#ifndef DUNGEONMAKERGENERATOR_H
#define DUNGEONMAKERGENERATOR_H

#include <dungeon/generators/grid_generator.h>

namespace butcher {

class DungeonMakerGenerator : public GridGenerator
{
public:
  DungeonMakerGenerator();

  virtual Grid generate();
};

}

#endif // DUNGEONMAKERGENERATOR_H
