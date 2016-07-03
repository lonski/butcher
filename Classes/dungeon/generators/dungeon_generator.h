#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <dungeon/dungeon_description.h>

namespace butcher {

class DungeonGenerator
{
public:
  DungeonGenerator()
  {}

  virtual bool generate(DungeonDescription&) = 0;

};

}

#endif // DUNGEON_GENERATOR_H
