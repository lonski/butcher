#ifndef AI_H
#define AI_H

#include <memory>
#include <actors/ai/fsm/fsm.h>
#include <utils/target.h>

namespace butcher {

class Actor;

class Ai
{
public:
  Ai(std::shared_ptr<Actor> actor);

  void update();
  std::shared_ptr<Actor> getActor();

  Target getTarget();
  void clearTarget();

private:
  std::shared_ptr<Actor> _actor;
  FSM _fsm;
  Target _target;

  void updateTarget();
};

}

#endif // AI_H
