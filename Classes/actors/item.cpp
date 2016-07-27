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

std::unique_ptr<Actor> Item::clone(std::unique_ptr<Actor> allocated)
{
  Item* o = dynamic_cast<Item*>(allocated.release());
  if ( o == nullptr )
    o = new Item(nullptr);

  return std::move(Actor::clone(std::unique_ptr<Actor>{o}));
}

bool Item::isUsable() const
{
  return false;
}

ItemSlotType Item::getItemSlotType() const
{
  return ItemSlotType::NONE;
}

}
