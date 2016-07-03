#include "spawn_builder.h"
#include <dungeon/dungeon_description.h>
#include <dungeon/room.h>

namespace cc = cocos2d;

namespace butcher {

bool SpawnBuilder::generateSpawns(DungeonDescription& dungeon)
{
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

  addStairs();
  addMobs();

  //debugMapPrint();

  _objectsLayer->setObjects(_objects);

  return true;
}

void SpawnBuilder::addStairs()
{
  enum {
    TRY_COUNT = 100
  };

  for(int i = 0; i < TRY_COUNT; ++i)
    if ( addActorSpawn(4, _dungeon->rooms.front()->getRandomCoord() ) )
      break;

  for(int i = 0; i < TRY_COUNT; ++i)
    if ( addActorSpawn(3, _dungeon->rooms.back()->getRandomCoord() ) )
      break;
}

void SpawnBuilder::addMobs()
{
  int mobCount(0);

  for ( std::shared_ptr<Rect> room : _dungeon->rooms )
  {
    int maxMobsInRoom = std::min( room->getAreaSize() / 10, 2 );
    while ( maxMobsInRoom-- )
    {
      if ( cc::RandomHelper::random_int(0,1) == 1 )
      {
        if ( addActorSpawn(2, room->getRandomCoord() ) )
          ++mobCount;
      }
    }
  }

  cc::log("Spawned %d mobs in %u rooms.", mobCount, (unsigned)_dungeon->rooms.size());
}

bool SpawnBuilder::addActorSpawn(int id, int y, int x)
{
  if ( _dungeon->grid.get(cc::Vec2(x,y)) != Tiles::FLOOR )
    return false;

  cc::ValueMap spawn;
  spawn["name"] = cc::Value("actor_spawn");
  spawn["id"] = cc::Value(id);
  spawn["x"] = cc::Value(x);
  spawn["y"] = cc::Value(y);

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

void SpawnBuilder::debugMapPrint()
{
  Grid tmp = _dungeon->grid;
  tmp.floodfill(getRandomRoom()->getRandomCoord(), '+');
  cc::log("%s", tmp.toStr().c_str());
}

}
