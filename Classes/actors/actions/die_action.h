#ifndef DIEACTION_H
#define DIEACTION_H

#include <actors/actions/actor_action.h>

namespace butcher {

class DieAction : public ActorAction
{
public:
  DieAction(std::shared_ptr<Actor> killer);
  virtual bool perform(std::shared_ptr<Actor> actor) const;

private:
 std::shared_ptr<Actor> _killer;
};

}

#endif // DIEACTION_H
