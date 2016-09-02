#include "effect.h"
#include <data/effects_generated.h>

namespace butcher {

Effect::Effect(const EffectData *data)
  : _id(EffectID::None)
  , _turns(0)
  , _onRemoveFn([](){})
{
  if ( data )
  {
    _id = static_cast<EffectID>(data->id());
    _turns = data->turns();

    if ( data->sprite_file() )
      _spriteFile = data->sprite_file()->c_str();

    _name = data->name()->c_str();

    auto mods = data->modifiers();
    for (unsigned i = 0; i < mods->Length(); ++i)
    {
      const ModifierData* m = mods->Get(i);

      _modifiers.push_back(
            Modifier(static_cast<AttributeType>(m->attr()),
                     m->value(),
                     static_cast<SpecialModifierType>(m->special()) ) );
    }

  }
}

EffectID Effect::getID() const
{
  return _id;
}

std::vector<Modifier> Effect::getModifiers() const
{
  return _modifiers;
}

std::string Effect::getSpriteFile() const
{
  return _spriteFile;
}

bool Effect::tick()
{
  if ( _turns < 0 )
    return false;

  return _turns-- <= 0;
}

void Effect::setOnRemoveFn(std::function<void ()> fn)
{
  _onRemoveFn = fn;
}

void Effect::onRemove()
{
  _onRemoveFn();
}

int Effect::getTurns() const
{
  return _turns;
}

std::string Effect::getName() const
{
  return _name;
}

}
