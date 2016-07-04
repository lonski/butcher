#include "level_manager.h"
#include <dungeon/dungeon_state.h>
#include <data/levels_generated.h>
#include <dungeon/generators/minimum_corridor_generator.h>
#include <dungeon/generators/celular_automata_generator.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

namespace butcher {

LevelManager::LevelManager()
{
  _levels.load("levels_data_wire.bin");
  _spawnBuilder.setMobIntroduction(_levels.getMobIntroduction());

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
  Profiler profiler;

  const LevelData* levelData = _levels.getLevelData(level);

  if ( !levelData )
  {
    cc::log("LevelManager::generateMap level id=%u not found!", level);
    return nullptr;
  }

  DungeonDescription description;
  description.settings = levelData;
  description.level = level;

  std::unique_ptr<DungeonGenerator> generator;

  switch(description.settings->generator())
  {
    case   GeneratorType_MinimumCorridorGenerator:
      generator.reset(new MinimumCorridorGenerator);
      break;
    case GeneratorType_CelularAutomanataGenerator:
      generator.reset(new CelularAutomataGenerator);
      break;
    default:
      cc::log("%s: Incorrect dungeon generator type (%d)",
              __PRETTY_FUNCTION__, description.settings->generator());
      return nullptr;
      break;
  }

  profiler.count();

  if ( !generator->generate(description) )
    return nullptr;

  profiler.log("Dungeon generation", Profiler::LAST_READ);

  _mapBuilder.setMapTemplate( levelData->map_template()->c_str() );
  description.tmx = _mapBuilder.build( description.grid );

  profiler.log("Tmx building", Profiler::LAST_READ);

  if ( !_spawnBuilder.generateSpawns(description) )
    return nullptr;

  profiler.log("Spawn generation", Profiler::LAST_READ);
  profiler.log("Whole level generation", Profiler::START);

  return description.tmx;
}

}
