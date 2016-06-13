#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <memory>
#include <map>
#include "cocos2d.h"

namespace butcher {

class DungeonState;
typedef std::shared_ptr<DungeonState> DungeonStatePtr;

class LevelManager
{
public:
  LevelManager();

  DungeonState* getLevel(int level);

private:
  std::map<unsigned /*level*/, DungeonState*> _dungeons;

  cocos2d::TMXTiledMap* generateMap(unsigned level);

};

}

#endif // LEVEL_MANAGER_H
