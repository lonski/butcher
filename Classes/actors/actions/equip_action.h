#ifndef EQUIP_ACTION_H
#define EQUIP_ACTION_H

#include <memory>
#include <actors/actions/actor_action.h>
#include <actors/amounted_item.h>

namespace butcher {

class Item;

class EquipAction : public ActorAction
{
public:
  EquipAction(std::shared_ptr<Item> item);
  EquipAction(const AmountedItem& amountedItem);

  virtual bool perform(std::shared_ptr<Actor> equipper);

private:
 AmountedItem _item;

};

}


#endif // EQUIP_ACTION_H
