#ifndef DUNGEON_DESCRIPTION_H
#define DUNGEON_DESCRIPTION_H

#include <utils/grid.h>
#include "cocos2d.h"

namespace butcher {

struct LevelData;
struct Room;

/**
 * @brief Helper struct used for generating dungeon
 */
struct DungeonDescription
{
  DungeonDescription()
    : settings(nullptr)
    , tmx(nullptr)
  {}

  DungeonDescription(const Grid& g,
                     std::vector<std::shared_ptr<Room>> r,
                     const LevelData* s = nullptr,
                     cocos2d::TMXTiledMap* m = nullptr)
    : grid(g)
    , rooms(r)
    , settings(s)
    , tmx(m)
  {}

  Grid grid;
  std::vector<std::shared_ptr<Room>> rooms;
  const LevelData* settings;
  cocos2d::TMXTiledMap* tmx;
};

}

#endif // DUNGEON_DESCRIPTION_H
