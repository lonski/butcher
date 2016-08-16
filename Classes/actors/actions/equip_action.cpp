#include "equip_action.h"
#include <actors/item.h>
#include <actors/player.h>

namespace butcher {

EquipAction::EquipAction(std::shared_ptr<Item> item)
  : _item(item, 1)
{
}

EquipAction::EquipAction(const AmountedItem &amountedItem)
  : _item(amountedItem)
{
}

bool EquipAction::perform(std::shared_ptr<Actor> equipper)
{
  std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(equipper);

  if ( player )
  {
    Inventory& inv = player->getInventory();

    if ( _item.item->getLevel() > player->getLevel() )
      return false;

    if ( inv.equip(_item) )
      return true;
  }

  return false;
}

}
