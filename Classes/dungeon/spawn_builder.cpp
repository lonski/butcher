#include "spawn_builder.h"
#include <dungeon/dungeon_description.h>
#include <dungeon/room.h>
#include <utils/path.h>
#include <data/levels_generated.h>
#include <butcher.h>
#include <data/actors_database.h>
#include <dungeon/waypoints.h>

namespace cc = cocos2d;

namespace butcher {

bool SpawnBuilder::generateSpawns(DungeonDescription& dungeon)

{
  bool result = true;

  _spawnStats.clear();
  _objects.clear();
  _dungeon = &dungeon;

  if(_dungeon->tmx == nullptr)
  {
      cc::log("%s: Tile map is null.", __PRETTY_FUNCTION__);
      return false;
  }

  _objectsLayer = _dungeon->tmx->getObjectGroup("Objects");
  if(_objectsLayer == nullptr)
  {
      cc::log("%s: Tile map has no objects object layer.", __PRETTY_FUNCTION__);
      return false;
  }

  addPredefinedSpawns();
  result &= addWell();
  result &= addPortal();
  result &= addStairs();
  addMobs();

  debugMapPrint();
  debugSpawnStatsPrint();

  _objectsLayer->setObjects(_objects);

  return result;
}

bool SpawnBuilder::addWell()
{
  int tries = MAX_TRIES;
  while(tries--)
  {
    auto room = getRandomNotUsedRoom();
    cc::Vec2 coord = room->findCoordWithPattern(_dungeon->grid, ".........", 10);
    if ( coord != cc::Vec2::ZERO && _dungeon->spawns.find(coord) == _dungeon->spawns.end())
    {
      _usedRooms.insert(room);
      addActorSpawn((int)ActorID::WELL, coord.y, coord.x);
      _dungeon->spawns[coord] = ActorID::WELL;
      return true;
    }
  }
  cc::log("%s Failed to place well.", __PRETTY_FUNCTION__);
  return false;
}

void SpawnBuilder::setMobIntroduction(const std::multimap<int, ActorID> &mobIntroduction)
{
  _mobIntroduction = mobIntroduction;
//  std::sort(_mobIntroduction.begin(), _mobIntroduction.end(), [](const std::pair<ActorID, int>& r, const std::pair<ActorID, int>& l){
//    return r.second > l.second;
//  });
}

void SpawnBuilder::addPredefinedSpawns()
{
  for ( auto& kv : _dungeon->spawns )
  {
    addActorSpawn((int)kv.second, kv.first);
  }
}

bool SpawnBuilder::addStairs()
{
  bool added = false;

  enum {
    TRY_COUNT = 100
  };

  cc::Vec2 upStairs;
  cc::Vec2 downStairs;

  for(int i = 0; i < TRY_COUNT; ++i)
  {
    auto room = getRandomNotUsedRoom();
    upStairs = room->findCoordWithPattern(_dungeon->grid, ".........", 10);

    if ( _dungeon->spawns.find(upStairs) == _dungeon->spawns.end() && addActorSpawn(4, upStairs ) )
    {
      added = true;
      _dungeon->spawns[upStairs] = ActorID::STAIRS_UP;
      _usedRooms.insert(room);
      break;
    }

    upStairs = cc::Vec2::ZERO;
  }

  if  ( added )
  {
    added = false;
    for(int i = 0; i < TRY_COUNT; ++i)
    {
      auto room = getRandomNotUsedRoom();
      downStairs = room->findCoordWithPattern(_dungeon->grid, ".........", 10);

      if ( _dungeon->spawns.find(downStairs) == _dungeon->spawns.end() && addActorSpawn(3,  downStairs ) )
      {
        added = true;
        _dungeon->spawns[downStairs] = ActorID::STAIRS_DOWN;
        _usedRooms.insert(room);
        break;
      }

      downStairs = cc::Vec2::ZERO;
    }
  }

  return added;
}

void SpawnBuilder::addMobs()
{
  //Grid g = _dungeon->grid;

  int mobCount(0);

  for ( std::shared_ptr<Rect> room : _dungeon->rooms )
  {
    int maxMobsInRoom = room->getFloorSize(_dungeon->grid) / _dungeon->settings->max_mob_density();
    while ( maxMobsInRoom-- )
    {
      if ( cc::RandomHelper::random_int(0, 100) <= _dungeon->settings->mob_spawn_chance() )
      {
        cc::Vec2 coord = room->getRandomFloorCoord(_dungeon->grid);
        if ( addActorSpawn( (int)getRandomMobID(), coord ) )
        {
          //g.set(coord, 'M');
          ++mobCount;
        }
      }
    }
  }

  cc::log("Spawned %d mobs in %u rooms.", mobCount, (unsigned)_dungeon->rooms.size());
  //cc::log("%s", g.toStr().c_str());
}

bool SpawnBuilder::addPortal()
{
  if ( waypoints.find(_dungeon->level) == waypoints.end() )
    return true;

  int tries = MAX_TRIES;
  while(tries--)
  {
    auto room = getRandomNotUsedRoom();
    cc::Vec2 coord = room->findCoordWithPattern(_dungeon->grid, ".........", 10);
    if ( coord != cc::Vec2::ZERO && _dungeon->spawns.find(coord) == _dungeon->spawns.end())
    {
      _usedRooms.insert(room);
      addActorSpawn((int)ActorID::WAYPOINT, coord.y, coord.x);
      _dungeon->spawns[coord] = ActorID::WAYPOINT;
      return true;
    }
  }

  cc::log("%s Failed to spawn portal", __PRETTY_FUNCTION__);
  return false;
}

bool SpawnBuilder::addActorSpawn(int id, int y, int x)
{
  if ( _dungeon->grid.get(cc::Vec2(x,y)) != Tiles::FLOOR )
    return false;

  _spawnStats[ (ActorID)id ]++;

  cc::ValueMap spawn;
  spawn["name"] = cc::Value("actor_spawn");
  spawn["id"] = cc::Value(id);
  spawn["x"] = cc::Value(x);
  spawn["y"] = cc::Value(y);

  if ( !canInsertObject(spawn) )
    return false;

  _objects.push_back(cc::Value(spawn));

  return true;
}

bool SpawnBuilder::addActorSpawn(int id, cocos2d::Vec2 pos)
{
  return addActorSpawn(id, pos.y, pos.x);
}

std::shared_ptr<Rect> SpawnBuilder::getRandomRoom() const
{
  return _dungeon->rooms[ cc::RandomHelper::random_int(0, (int)_dungeon->rooms.size() - 1) ];
}

std::shared_ptr<Rect> SpawnBuilder::getRandomNotUsedRoom() const
{
  int tries = MAX_TRIES;
  while(tries--)
  {
    auto room = getRandomRoom();
    if ( _usedRooms.find(room) == _usedRooms.end() )
      return room;
  }
  return nullptr;
}

void SpawnBuilder::debugMapPrint()
{
  Grid tmp = _dungeon->grid;
  //tmp.floodfill(getRandomRoom()->getRandomCoord(), '+');
  for ( auto& kv : _dungeon->spawns ){
    if ( kv.second == ActorID::WELL )
      tmp.set(kv.first.x, kv.first.y, 'O');
    else if ( kv.second == ActorID::STAIRS_DOWN )
      tmp.set(kv.first.x, kv.first.y, '>');
    else if ( kv.second == ActorID::STAIRS_UP )
      tmp.set(kv.first.x, kv.first.y, '<');
    else if ( kv.second == ActorID::WAYPOINT )
      tmp.set(kv.first.x, kv.first.y, '&');
  }

  cc::log("%s", tmp.toStr().c_str());
}

void SpawnBuilder::debugSpawnStatsPrint()
{
  cc::log(" == SPAWN STATS == ");

  for ( auto& kv : _spawnStats )
    cc::log("  |%s [%d] -> %d", BUTCHER.actorsDatabase().getName(kv.first).c_str(), (int)kv.first, kv.second);

  cc::log("  +-----");
}

ActorID SpawnBuilder::getRandomMobID()
{
  int tries = 100;

  while( tries-- )
  {
    int lastLevel = 0;
    for( auto kv : _mobIntroduction)
    {
      if ( kv.first != lastLevel )
      {
        lastLevel = kv.first;

        if ( lastLevel <= _dungeon->level )
        {
          int levelDiff = std::max(std::abs( _dungeon->level - lastLevel), 1);
          float chance = 0.25 * 1/(float)levelDiff;

          if ( cc::RandomHelper::random_real<float>(0, 1) < chance )
          {
            //Get all mobs that can spawn on this level
            std::pair<MobIntroductionMap::iterator, MobIntroductionMap::iterator> levelGroup =
                _mobIntroduction.equal_range(kv.first);

            std::vector<ActorID> mobsOnThisLevel;
            for(auto i = levelGroup.first; i != levelGroup.second; ++i)
              mobsOnThisLevel.push_back(i->second);

            if ( !mobsOnThisLevel.empty() )
              return mobsOnThisLevel[ cc::RandomHelper::random_int(0, (int)mobsOnThisLevel.size() - 1) ];
          }
        }
      }
    }
  }

  return ActorID::COW;
}

bool SpawnBuilder::canInsertObject(cocos2d::ValueMap obj)
{
  for ( cc::Value& v : _objects )
  {
    cc::ValueMap vm = v.asValueMap();
    if ( vm["x"] == obj["x"] && vm["y"] == obj["y"] )
      return false;
  }
  return true;
}

}
