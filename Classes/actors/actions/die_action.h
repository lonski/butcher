#ifndef DIEACTION_H
#define DIEACTION_H

#include <actors/actions/actor_action.h>

namespace butcher {

class DieAction : public ActorAction
{
public:
  DieAction();
  virtual bool perform(Actor* actor);
};

}

#endif // DIEACTION_H
