#ifndef ACTOR_ACTION_H
#define ACTOR_ACTION_H

#include <memory>
#include <utils/target.h>

namespace butcher {

class Actor;

class ActorAction
{
public:
  enum class Result
  {
    OK,
    NOK,
    COST_EXTRA_TURN
  };

  virtual ActorAction::Result perform(std::shared_ptr<Actor> actor) = 0;
  virtual void setTarget(Target target) {}

};

}

#endif // ACTOR_ACTION_H
