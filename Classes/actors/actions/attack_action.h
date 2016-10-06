#ifndef ATTACKACTION_H
#define ATTACKACTION_H

#include <actors/actions/actor_action.h>
#include <utils/target.h>
#include <actors/item.h>

namespace butcher {

class AttackAction : public ActorAction
{
public:
  AttackAction(Target target);
  virtual ActorAction::Result perform(std::shared_ptr<Actor> performer);

private:
  Target _target;

  std::shared_ptr<Item> getWeapon(std::shared_ptr<Character> c);
};

}

#endif // ATTACKACTION_H
