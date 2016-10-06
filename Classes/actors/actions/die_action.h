#ifndef DIEACTION_H
#define DIEACTION_H

#include <actors/actions/actor_action.h>

namespace butcher {

class DieAction : public ActorAction
{
public:
  DieAction(std::shared_ptr<Actor> killer);
  virtual ActorAction::Result perform(std::shared_ptr<Actor> actor);

private:
 std::shared_ptr<Actor> _killer;
};

}

#endif // DIEACTION_H
