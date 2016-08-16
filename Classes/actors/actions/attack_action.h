#ifndef ATTACKACTION_H
#define ATTACKACTION_H

#include <actors/actions/actor_action.h>
#include <utils/target.h>

namespace butcher {

class AttackAction : public ActorAction
{
public:
  AttackAction(Target target);
  virtual bool perform(std::shared_ptr<Actor> performer);

private:
  Target _target;
};

}

#endif // ATTACKACTION_H
