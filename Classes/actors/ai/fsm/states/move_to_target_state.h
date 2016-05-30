#ifndef MOVE_TO_TARGET_STATE_H
#define MOVE_TO_TARGET_STATE_H

#include <actors/ai/fsm/states/fsm_state.h>

namespace butcher {

class Ai;

namespace state{

class MoveToTarget : public FSMState
{
public:
  MoveToTarget();

  virtual FSMStateType getType() const;
  virtual void update();
  virtual bool canEnter();

};

}}

#endif // MOVE_TO_TARGET_STATE_H
