#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <string>
#include <dungeon/room.h>
#include <dungeon/dungeon_description.h>
#include "cocos2d.h"

namespace butcher {

struct LevelData;

class DungeonGenerator
{
public:
  bool generate(DungeonDescription& description);

private:
  DungeonDescription* _data;

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
