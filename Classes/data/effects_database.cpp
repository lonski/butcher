#include "effects_database.h"
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

EffectDatabase::EffectDatabase()
{
}

bool EffectDatabase::load(const std::string &fn)
{
  cc::Data file_data = cc::FileUtils::getInstance()->getDataFromFile(fn);

  if ( file_data.getSize() == 0 )
  {
    cc::log("%s: failed to load file.", __PRETTY_FUNCTION__);
    return false;
  }

  const EffectsData* data = GetEffectsData( file_data.getBytes() );

  if ( data == nullptr )
  {
    cc::log("%s: failed to load effects.", __PRETTY_FUNCTION__);
    return false;
  }

  auto effects = data->effects();
  for (unsigned i = 0; i < effects->Length(); ++i)
  {
    const EffectData* d = effects->Get(i);
    _effects[ static_cast<EffectID>(d->id()) ] = Effect(d);
  }

  cc::log("%s: loaded %lu effects.", __PRETTY_FUNCTION__, _effects.size());

  return true;
}

Effect EffectDatabase::createEffect(EffectID id)
{
  return _effects[id];
}

}
