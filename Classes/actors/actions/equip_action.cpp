#include "equip_action.h"
#include <actors/item.h>
#include <actors/player.h>

namespace butcher {

EquipAction::EquipAction(std::shared_ptr<Item> item)
  : _item(item, 1)
  , _itemID(ActorID::INVALID)
{
}

EquipAction::EquipAction(ActorID item_id)
  : _itemID(item_id)
{
}

EquipAction::EquipAction(const AmountedItem &amountedItem)
  : _item(amountedItem)
  , _itemID(ActorID::INVALID)
{
}

ActorAction::Result EquipAction::perform(std::shared_ptr<Actor> equipper)
{
  std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(equipper);

  if ( _item.item == nullptr && _itemID == ActorID::INVALID)
    return ActorAction::Result::NOK;

  if ( player )
  {
    Inventory& inv = player->getInventory();

    if ( _itemID != ActorID::INVALID )
    {
      _item = inv.getItem(_itemID);
      if (_item.amount > 1) _item.amount = 1;
      inv.removeItem(_item);
    }

    if ( !_item.item )
      return ActorAction::Result::NOK;

    if ( _item.item->getLevel() > player->getLevel() )
      return ActorAction::Result::NOK;

    if ( inv.equip(_item) )
    {
      player->fadeText("Equipped " + _item.item->getName(), cocos2d::Color4B::GREEN);
      return ActorAction::Result::OK;
    }

  }

  return ActorAction::Result::NOK;
}

}
