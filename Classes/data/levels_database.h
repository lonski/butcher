#ifndef LEVELS_DATABASE_H
#define LEVELS_DATABASE_H

#include <string>
#include <data/levels_generated.h>
#include "cocos2d.h"

namespace butcher {

class LevelDatabase
{
public:
  LevelDatabase();
  bool load(const std::string& fn);
  const LevelData* getLevelData(unsigned id);

private:
  cocos2d::Data _fileData;
  const flatbuffers::Vector<flatbuffers::Offset<LevelData>> * _levels;

};

}

#endif // LEVELS_DATABASE_H
