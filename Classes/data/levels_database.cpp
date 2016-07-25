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
  _mobIntroduction.clear();

  for(int i = 0; i < levelsData->mobs_introduction()->size(); ++i)
  {
    const MobIntroductionData* mi = levelsData->mobs_introduction()->Get(i);
    _mobIntroduction.insert(
          std::make_pair(mi->introduced_at(), (ActorID)mi->mob_id()) );
  }

  cc::log("LevelsData: loaded %u levels.", _levels->Length());

  return true;
}

const LevelData *LevelDatabase::getLevelData(unsigned depth)
{
  for (unsigned i = 0; i < _levels->Length(); ++i)
  {
    const LevelData* d = _levels->Get(i);
    if ( depth >= (unsigned)d->depth()->Get(0) &&  depth <= (unsigned)d->depth()->Get(1) )
      return d;
  }
  return nullptr;
}

MobIntroductionMap LevelDatabase::getMobIntroduction() const
{
  return _mobIntroduction;
}

}
