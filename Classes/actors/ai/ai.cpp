#include "ai.h"
#include <actors/ai/fsm/states/fsm_state.h>
#include <actors/actor.h>
#include <butcher.h>

namespace butcher {

Ai::Ai(Actor *actor)
  : _actor(actor)
  , _fsm(this)
{
}

void Ai::update()
{
  //TODO determine ai state
  if ( _fsm.getCurrentState()->getType() != FSMStateType::MOVE_TO_TARGET )
    _fsm.changeState( FSMStateType::MOVE_TO_TARGET );

  _fsm.update();
}

Actor* Ai::getActor()
{
  return _actor;
}

Target Ai::getTarget()
{
  //TODO
  return Target((Actor*)BUTCHER.getPlayer().get());
}

}
