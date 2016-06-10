#include "object.h"

namespace butcher {

Object::Object(const ActorData* data)
  : Actor(data)
{
}

int Object::getZ() const
{
  return 10;
}

Actor* Object::clone(Actor* allocated)
{
  Object* o = dynamic_cast<Object*>(allocated);
  if ( o == nullptr )
    o = new Object(nullptr);

  Actor::clone(o);

  return o;
}

}
