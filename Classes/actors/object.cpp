#include "object.h"
#include  <data/actors_generated.h>
#include <butcher.h>
#include <data/effects_database.h>

namespace butcher {

Object::Object(const ActorData* data)
  : Actor(data)
  , _effect(EffectID::None)
{
  if ( data )
  {
    _effect = static_cast<EffectID>(data->effect_id());
  }
}

int Object::getZ() const
{
  return 10;
}

std::unique_ptr<Actor> Object::clone(std::unique_ptr<Actor> allocated)
{
  Object* o = dynamic_cast<Object*>(allocated.release());
  if ( o == nullptr )
    o = new Object(nullptr);

  o->_effect = _effect;

  return std::move( Actor::clone(std::unique_ptr<Actor>{o}) );
}

void Object::onInterract(std::shared_ptr<Actor> actor)
{
  if ( _effect != EffectID::None )
    actor->addEffect(BUTCHER.effectsDatabase().createEffect(_effect));
}

}
