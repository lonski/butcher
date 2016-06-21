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

std::unique_ptr<Actor> Object::clone(std::unique_ptr<Actor> allocated)
{
  Object* o = dynamic_cast<Object*>(allocated.release());
  if ( o == nullptr )
    o = new Object(nullptr);

  return std::move( Actor::clone(std::unique_ptr<Actor>{o}) );
}

}
