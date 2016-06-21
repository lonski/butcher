#ifndef DIEACTION_H
#define DIEACTION_H

#include <actors/actions/actor_action.h>

namespace butcher {

class DieAction : public ActorAction
{
public:
  DieAction(Actor* killer);
  virtual bool perform(Actor* actor);

private:
 Actor* _killer;
};

}

#endif // DIEACTION_H
