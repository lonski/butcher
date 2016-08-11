#include "level_manager.h"
#include <dungeon/dungeon_state.h>
#include <data/levels_generated.h>
#include <dungeon/generators/minimum_corridor_generator.h>
#include <dungeon/generators/celular_automata_generator.h>
#include <utils/profiler.h>
#include <utils/path.h>

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

    auto map = generateMap(level);
    while (!map)
    {
      cc::log("%s: regenerating map..", __PRETTY_FUNCTION__);
      map = generateMap(level);
    }

    dungeonState->setMap( map );
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

  if ( !validateConnection(description) )
  {
    cc::log("%s validate connection failed!", __PRETTY_FUNCTION__);
    return nullptr;
  }

  profiler.log("Spawn generation", Profiler::LAST_READ);
  profiler.log("Whole level generation", Profiler::START);

  return description.tmx;
}

bool LevelManager::validateConnection(const DungeonDescription& dsc)
{
  cc::Vec2 start = getSpawn(ActorID::STAIRS_UP, dsc);
  cc::Vec2 finish = getSpawn(ActorID::STAIRS_DOWN, dsc);

  if ( start == cc::Vec2::ZERO || finish == cc::Vec2::ZERO )
  {
    cc::log("%s failed to find stairs!", __PRETTY_FUNCTION__);
    return false;
  }

  auto isBlockedFn = [dsc](cocos2d::Vec2 pos){
    return dsc.grid.get(pos) != Tiles::FLOOR;
  };

  Path path;

  return path.calculate(start, finish, isBlockedFn);
}

cocos2d::Vec2 LevelManager::getSpawn(ActorID actor, const DungeonDescription &dsc)
{
  for ( auto& pair : dsc.spawns )
    if ( pair.second == actor )
      return pair.first;

  return cc::Vec2::ZERO;
}

}
