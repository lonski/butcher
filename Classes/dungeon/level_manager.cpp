#include "level_manager.h"
#include <dungeon/dungeon_state.h>
#include <dungeon/generators/cave_grid_generator.h>
#include <dungeon/generators/tmx_builder.h>
#include <dungeon/generators/dungeon_maker_generator.h>

namespace cc = cocos2d;

namespace butcher {

LevelManager::LevelManager()
{
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

cocos2d::TMXTiledMap *LevelManager::generateMap(unsigned level)
{
  std::string mapFn = "dungeons/" + cc::Value(level).asString() + ".tmx";
  cc::TMXTiledMap* map = nullptr;

  if ( !cc::FileUtils::getInstance()->isFileExist(mapFn) )
  {
    std::unique_ptr<GridGenerator> gen;

    gen.reset( new CaveGenerator );

    Grid grid = gen->generate();

    TMXBuilder builder;
    map = builder.build(grid);
  }
  else
  {
    map = new cc::TMXTiledMap();
    map->initWithTMXFile(mapFn);
  }

  return map;
}

}
