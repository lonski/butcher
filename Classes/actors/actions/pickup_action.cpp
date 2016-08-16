#include "pickup_action.h"
#include <actors/item.h>
#include <actors/player.h>
#include <butcher.h>
#include <sstream>

namespace butcher {

PickUpAction::PickUpAction(std::shared_ptr<Item> item)
  : _item(item, 1)
{
}

PickUpAction::PickUpAction(const AmountedItem &amountedItem)
  : _item(amountedItem)
{
}

bool PickUpAction::perform(std::shared_ptr<Actor> pickuper)
{
  //currently only player holds inventory
  std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(pickuper);

  if ( player )
  {
    Inventory& inv = player->getInventory();
    if ( inv.addItem(_item) )
    {
      std::ostringstream os;
      os << "Picked up "
         << _item.item->getName()
         << " x" << _item.amount;

      BUTCHER.print(os.str());

      return true;
    }
  }

  return false;
}

}
