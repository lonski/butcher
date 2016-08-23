#ifndef EFFECT_H
#define EFFECT_H

#include <vector>
#include <string>
#include <actors/effects/modifier.h>
#include <actors/effects/effect_id.h>

namespace butcher {

struct EffectData;

class Effect
{
public:
  Effect(const EffectData* data = nullptr);

  EffectID getID() const;
  std::vector<Modifier> getModifiers() const;
  std::string getSpriteFile() const;
  std::string getName() const;

  bool tick();

  int getTurns() const;

private:
  EffectID _id;
  int _turns;
  std::vector<Modifier> _modifiers;
  std::string _spriteFile;
  std::string _name;

};

}

#endif // EFFECT_H
