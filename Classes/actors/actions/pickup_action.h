#ifndef PICKUP_ACTION_H
#define PICKUP_ACTION_H

#include <memory>
#include <actors/actions/actor_action.h>
#include <actors/amounted_item.h>

namespace butcher {

class Item;

class PickUpAction : public ActorAction
{
public:
  PickUpAction(std::shared_ptr<Item> item);
  PickUpAction(const AmountedItem& amountedItem);

  virtual ActorAction::Result perform(std::shared_ptr<Actor> pickuper);

private:
 AmountedItem _item;

};

}

#endif // PICKUP_ACTION_H
