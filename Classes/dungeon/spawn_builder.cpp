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

  debugMapPrint();

  _objectsLayer->setObjects(_objects);

  return true;
}

void SpawnBuilder::addStairs()
{
  addActorSpawn(4, _dungeon->rooms.front()->getRandomTile(Tiles::FLOOR));
  addActorSpawn(3, _dungeon->rooms.back()->getRandomTile(Tiles::FLOOR));
}

void SpawnBuilder::addMobs()
{
  for ( std::shared_ptr<Room> room : _dungeon->rooms )
  {
    int maxMobsInRoom = room->getTiles(Tiles::FLOOR).size() / 10;
    while ( maxMobsInRoom-- )
      if ( cc::RandomHelper::random_int(0,1) == 1 )
        addActorSpawn(2, room->getRandomTile(Tiles::FLOOR));
  }
}

void SpawnBuilder::addActorSpawn(int id, int y, int x)
{
  cc::ValueMap spawn;
  spawn["name"] = cc::Value("actor_spawn");
  spawn["id"] = cc::Value(id);
  spawn["x"] = cc::Value(x);
  spawn["y"] = cc::Value(y);

  _objects.push_back(cc::Value(spawn));
}

void SpawnBuilder::addActorSpawn(int id, cocos2d::Vec2 pos)
{
  addActorSpawn(id, pos.y, pos.x);
}

std::shared_ptr<Room> SpawnBuilder::getRandomRoom() const
{
  return _dungeon->rooms[ cc::RandomHelper::random_int(0, (int)_dungeon->rooms.size() - 1) ];
}

void SpawnBuilder::debugMapPrint()
{
  Grid tmp = _dungeon->grid;
  tmp.floodfill(getRandomRoom()->getRandomTile(Tiles::FLOOR), '+');
  cc::log("%s", tmp.toStr().c_str());
}

}
