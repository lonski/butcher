#include "level_manager.h"
#include <dungeon/dungeon_state.h>
#include <data/levels_generated.h>

namespace cc = cocos2d;

namespace butcher {

LevelManager::LevelManager()
{
  _levels.load("levels_data_wire.bin");
}

DungeonState *LevelManager::getLevel(int level)
{
  auto it = _dungeons.find(level);
  DungeonState* dungeonState = it != _dungeons.end() ? it->second : nullptr;

  if ( dungeonState == nullptr )
  {
    dungeonState = new DungeonState();
    dungeonState->setMap( generateMap(level) );
    _dungeons[level] = dungeonState;
  }

  return dungeonState;
}

cc::TMXTiledMap* LevelManager::generateMap(unsigned level)
{
  const LevelData* levelData = _levels.getLevelData(level);

  if ( !levelData )
  {
    cc::log("LevelManager::generateMap level id=%u not found!", level);
    return nullptr;
  }

  DungeonDescription dsc = _dungeonGenerator.generate(levelData);

  _mapBuilder.setMapTemplate( levelData->map_template()->c_str() );
  cc::TMXTiledMap* map = _mapBuilder.build( dsc.grid );

  return map;
}

}
