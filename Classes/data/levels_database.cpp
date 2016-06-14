#include "levels_database.h"

namespace cc = cocos2d;

namespace butcher {

LevelDatabase::LevelDatabase()
  : _levels(nullptr)
{
}

bool LevelDatabase::load(const std::string &fn)
{
  _fileData = cc::FileUtils::getInstance()->getDataFromFile(fn);

  if ( _fileData.getSize() == 0 )
  {
    cc::log("LevelDatabase: failed to load file.");
    return false;
  }

  auto levelsData = GetLevelsData( _fileData.getBytes() );

  if ( levelsData == nullptr )
  {
    cc::log("LevelDatabase: failed to load levels.");
    return false;
  }

  _levels = levelsData->levels();

  cc::log("LevelsData: loaded %u levels.", _levels->Length());

  return true;
}

const LevelData *LevelDatabase::getLevelData(unsigned id)
{
  for (unsigned i = 0; i < _levels->Length(); ++i)
  {
    const LevelData* d = _levels->Get(i);
    if ( d->id() == id )
      return d;
  }
  return nullptr;
}

}
