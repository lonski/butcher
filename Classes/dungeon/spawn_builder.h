#ifndef SPAWNLOGIC_H
#define SPAWNLOGIC_H

#include <map>
#include <actors/actor_id.h>
#include "cocos2d.h"

namespace butcher {

typedef std::multimap< int, ActorID > MobIntroductionMap;

class LevelData;
struct DungeonDescription;
struct Rect;

class SpawnBuilder
{
public:
  bool generateSpawns(DungeonDescription& dungeon);
  void setMobIntroduction(const std::multimap< int, ActorID >& mobIntroduction);

private:
  DungeonDescription* _dungeon;
  cocos2d::TMXObjectGroup* _objectsLayer;
  cocos2d::ValueVector _objects;
  std::multimap< int, ActorID > _mobIntroduction;

  void addPredefinedSpawns();
  bool addStairs();
  void addMobs();
  void addPortal();

  bool addActorSpawn(int id, int y, int x);
  bool addActorSpawn(int id, cocos2d::Vec2 pos);
  std::shared_ptr<Rect> getRandomRoom() const;
  ActorID getRandomMobID();
  bool canInsertObject(cocos2d::ValueMap obj);

  void debugMapPrint();
  void debugSpawnStatsPrint();

  std::map<ActorID, int> _spawnStats;

  void addWell();
};

}

#endif // SPAWNLOGIC_H
