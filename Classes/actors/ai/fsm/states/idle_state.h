#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include <actors/ai/fsm/states/fsm_state.h>

namespace butcher {

class Ai;

namespace state{

class Idle : public FSMState
{
public:
  Idle();

  virtual FSMStateType getType() const;
  virtual void update();
  virtual bool canEnter();

};

}}

#endif // IDLE_STATE_H
