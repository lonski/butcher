#ifndef USE_ACTION_H
#define USE_ACTION_H

#include <memory>
#include <actors/actions/actor_action.h>
#include <actors/amounted_item.h>

namespace butcher {

class Item;

class UseAction : public ActorAction
{
public:
  UseAction(std::shared_ptr<Item> item);
  UseAction(const AmountedItem& amountedItem);

  virtual bool perform(std::shared_ptr<Actor> user);

private:
 AmountedItem _item;
 std::shared_ptr<Character> _user;
 Effect _effect;

 bool useOnSelf();
 bool useOnWeapon();
 bool useOnFloor();
 bool useRange();

};

}

#endif // USE_ACTION_H
