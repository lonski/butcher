#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <string>
#include <dungeon/room.h>
#include "cocos2d.h"

namespace butcher {

struct LevelData;

struct DungeonDescription
{
  DungeonDescription()
  {}

  DungeonDescription(const Grid& g, std::vector<std::shared_ptr<Room>> r)
    : grid(g)
    , rooms(r)
  {}

  Grid grid;
  std::vector<std::shared_ptr<Room>> rooms;
};

class DungeonGenerator
{
public:
  DungeonDescription generate(const LevelData* data);

    
private:
  std::vector<std::shared_ptr<Room>> _rooms;
  Grid _grid;
  const LevelData* _settings;

  void putRooms();
  void growMaze(cocos2d::Vec2 pos);
  bool canCarve(cocos2d::Vec2 pos, Direction::Symbol dir);
  void removeDeadEnds();
  void connectRooms();
  bool connectRoomAt(cocos2d::Vec2 hole);
  int exitCount(cocos2d::Vec2 pos);
};

}

#endif // DUNGEON_GENERATOR_H
