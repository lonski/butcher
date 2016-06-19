#include "spawn_builder.h"
#include <dungeon/dungeon_description.h>
#include <dungeon/room.h>

namespace cc = cocos2d;

namespace butcher {

bool SpawnBuilder::generateSpawns(DungeonDescription& dungeon)
{
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

  addActorSpawn(4, getRandomRoom()->getRandomTile(Tiles::FLOOR));
  addActorSpawn(3, getRandomRoom()->getRandomTile(Tiles::FLOOR));

  _objectsLayer->setObjects(_objects);

  Grid tmp = _dungeon->grid;
  tmp.floodfill(getRandomRoom()->getRandomTile(Tiles::FLOOR), '+');
  cc::log("%s", tmp.toStr().c_str());

  return true;
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
  addActorSpawn(id, pos.x, pos.y);
}

std::shared_ptr<Room> SpawnBuilder::getRandomRoom() const
{
  return _dungeon->rooms[ cc::RandomHelper::random_int(0, (int)_dungeon->rooms.size() - 1) ];
}


}
