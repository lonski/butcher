#ifndef WANDERING_STATE_H
#define WANDERING_STATE_H

#include <actors/ai/fsm/states/fsm_state.h>

namespace butcher {

class Ai;

namespace state{

class Wandering : public FSMState
{
public:
  Wandering();

  virtual FSMStateType getType() const;
  virtual void update();
  virtual bool canEnter();

};

}}

#endif // WANDERING_STATE_H
