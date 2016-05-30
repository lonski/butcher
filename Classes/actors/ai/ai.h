#ifndef AI_H
#define AI_H

#include <actors/ai/fsm/fsm.h>
#include <utils/target.h>

namespace butcher {

class Actor;

class Ai
{
public:
  Ai(Actor* actor);

  void update();
  Actor* getActor();

  Target getTarget();

private:
  Actor* _actor;
  FSM _fsm;

};

}

#endif // AI_H
