#ifndef LEVELS_DATABASE_H
#define LEVELS_DATABASE_H

#include <string>
#include <data/levels_generated.h>
#include <actors/actor_id.h>
#include "cocos2d.h"

namespace butcher {

typedef std::multimap< int, ActorID > MobIntroductionMap;

class LevelDatabase
{
public:
  LevelDatabase();
  bool load(const std::string& fn);
  const LevelData* getLevelData(unsigned depth);
  MobIntroductionMap getMobIntroduction() const;

private:
  cocos2d::Data _fileData;
  const flatbuffers::Vector<flatbuffers::Offset<LevelData>> * _levels;
  MobIntroductionMap _mobIntroduction;

};

}

#endif // LEVELS_DATABASE_H
