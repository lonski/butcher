#include "ranged_attack_state.h"
#include <memory>
#include <actors/ai/ai.h>
#include <actors/monster.h>
#include <actors/actions/shot_action.h>

namespace butcher {

state::RangedAttack::RangedAttack()
{
}

FSMStateType state::RangedAttack::getType() const
{
  return FSMStateType::RANGED_ATTACK;
}

void state::RangedAttack::update()
{
  if ( validatePreconditions() )
  {
    _ai->getActor()->performAction( new ShotAction(_ai->getTarget()) );
  }
}

bool state::RangedAttack::canEnter()
{
  std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(_ai->getActor());
  return mob && mob->canShootAt(_ai->getTarget().pos);
}

}
