#ifndef SPAWNLOGIC_H
#define SPAWNLOGIC_H

#include "cocos2d.h"

namespace butcher {

class LevelData;
struct DungeonDescription;

class SpawnBuilder
{
public:
  void generateSpawns(const cocos2d::TMXTiledMap& map, const DungeonDescription& dungeon);

private:
  const LevelData* _settings;

};

}

#endif // SPAWNLOGIC_H
