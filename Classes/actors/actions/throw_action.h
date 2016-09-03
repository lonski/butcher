#ifndef THROW_ACTION_H
#define THROW_ACTION_H

#include <memory>
#include <actors/actions/actor_action.h>
#include <utils/target.h>

namespace butcher {

class Item;

class ThrowAction : public ActorAction
{
public:
  ThrowAction(std::shared_ptr<Actor> grenade);
  virtual bool perform(std::shared_ptr<Actor> performer);
  virtual void setTarget(Target target);

private:
  Target _target;
  std::shared_ptr<Actor> _performer;
  std::shared_ptr<Actor> _grenade;

  void runThrowAnimation();
  void executeEffects(std::shared_ptr<Item> grenadeItem);
  void runExplosionAnimation(std::shared_ptr<Item> grenadeItem);
};

}

#endif // THROW_ACTION_H
