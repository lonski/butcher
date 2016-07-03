#ifndef MINIMUM_CORRIDOR_GENERATOR_H
#define MINIMUM_CORRIDOR_GENERATOR_H

#include <dungeon/generators/dungeon_generator.h>
#include <dungeon/room.h>

namespace butcher {

class MinimumCorridorGenerator : public DungeonGenerator
{
public:
  MinimumCorridorGenerator();

  bool generate(DungeonDescription& dsc);

private:
  DungeonDescription* _data;
  std::vector<Rect> _exits;

  bool makeRoom(int x, int y, Direction::Symbol dir, bool firstRoom = false);
  bool createFeature();
  bool createFeature(int x, int y, Direction::Symbol dir);
  bool makeCorridor(int x, int y, Direction::Symbol dir);
  void removeDeadEnds();
};

}

#endif // MINIMUM_CORRIDOR_GENERATOR_H
