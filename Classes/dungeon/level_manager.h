#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <memory>
#include <map>
#include "cocos2d.h"
#include <data/levels_database.h>
#include <dungeon/tmx_builder.h>
#include <dungeon/dungeon_generator.h>
#include <dungeon/spawn_builder.h>

namespace butcher {

class DungeonState;
typedef std::shared_ptr<DungeonState> DungeonStatePtr;

/**
 * @brief This class holds all visited levels - DungeonStates
 *        and generates new, unexplored ones.
 */
class LevelManager
{
public:
  LevelManager();

  DungeonState* getLevel(int level);

private:
  std::map<unsigned /*level*/, DungeonState*> _dungeons;
  LevelDatabase _levels;
  TMXBuilder _mapBuilder;
  DungeonGenerator _dungeonGenerator;
  SpawnBuilder _spawnBuilder;

  cocos2d::TMXTiledMap* generateMap(unsigned level);

};

}

#endif // LEVEL_MANAGER_H
