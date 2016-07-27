#include "item.h"
#include <data/actors_generated.h>

namespace butcher {

Item::Item(const ActorData* data)
  : Actor(data)
  , _slot(ItemSlotType::NONE)
{
  if ( data )
  {
    _slot = static_cast<ItemSlotType>(data->body_slot());
  }
}

int Item::getZ() const
{
  return 11;
}

std::unique_ptr<Actor> Item::clone(std::unique_ptr<Actor> allocated)
{
  Item* o = dynamic_cast<Item*>(allocated.release());
  if ( o == nullptr )
    o = new Item(nullptr);

  o->_slot = _slot;

  return std::move(Actor::clone(std::unique_ptr<Actor>{o}));
}

bool Item::isUsable() const
{
  return false;
}

ItemSlotType Item::getItemSlotType() const
{
  return _slot;
}

}
