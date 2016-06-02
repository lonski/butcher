#ifndef MELEE_ATTACK_STATE_H
#define MELEE_ATTACK_STATE_H

#include <actors/ai/fsm/states/fsm_state.h>

namespace butcher {

class Ai;

namespace state{

class MeleeAttack : public FSMState
{
public:
  MeleeAttack();

  virtual FSMStateType getType() const;
  virtual void update();
  virtual bool canEnter();

};

}}

#endif // MELEE_ATTACK_STATE_H
