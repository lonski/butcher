#include "item.h"

namespace butcher {

Item::Item(const ActorData* data)
  : Actor(data)
{
}

int Item::getZ() const
{
  return 11;
}

Actor* Item::clone(Actor* allocated)
{
  Item* o = dynamic_cast<Item*>(allocated);
  if ( o == nullptr )
    o = new Item(nullptr);

  Actor::clone(o);

  return o;
}

}
