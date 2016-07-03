#ifndef SPAWNLOGIC_H
#define SPAWNLOGIC_H

#include "cocos2d.h"

namespace butcher {

class LevelData;
struct DungeonDescription;
struct Rect;

class SpawnBuilder
{
public:
  bool generateSpawns(DungeonDescription& dungeon);


private:
  DungeonDescription* _dungeon;
  cocos2d::TMXObjectGroup* _objectsLayer;
  cocos2d::ValueVector _objects;

  void addStairs();
  void addMobs();

  bool addActorSpawn(int id, int y, int x);
  bool addActorSpawn(int id, cocos2d::Vec2 pos);
  std::shared_ptr<Rect> getRandomRoom() const;
  void debugMapPrint();

};

}

#endif // SPAWNLOGIC_H
