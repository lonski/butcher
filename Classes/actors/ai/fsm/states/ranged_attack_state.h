#ifndef RANGED_ATTACK_STATE_H
#define RANGED_ATTACK_STATE_H

#include <actors/ai/fsm/states/fsm_state.h>

namespace butcher {

class Ai;

namespace state{

class RangedAttack : public FSMState
{
public:
  RangedAttack();

  virtual FSMStateType getType() const;
  virtual void update();
  virtual bool canEnter();

};

}}
#endif // RANGED_ATTACK_STATE_H
