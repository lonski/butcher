#ifndef EFFECTS_DATABASE_H
#define EFFECTS_DATABASE_H

#include <string>
#include <map>
#include <data/effects_generated.h>
#include <actors/effects/effect.h>

namespace butcher {

class EffectDatabase
{
public:
  EffectDatabase();
  bool load(const std::string& fn);

  Effect createEffect(EffectID id);

private:
  std::map<EffectID, Effect> _effects;

};

}

#endif // EFFECTS_DATABASE_H
