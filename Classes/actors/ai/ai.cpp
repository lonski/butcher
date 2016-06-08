#include "ai.h"
#include <actors/ai/fsm/states/fsm_state.h>
#include <actors/actor.h>
#include <butcher.h>
#include <utils/utils.h>
#include <dungeon/dungeon_state.h>

namespace butcher {

Ai::Ai(Actor *actor)
  : _actor(actor)
  , _fsm(this)
{
}

void Ai::update()
{
  if ( !getTarget().actors.empty() )
  {
    float distance = calculateDistance(getActor()->getTileCoord(), getTarget().first()->getTileCoord());
    _fsm.changeState( distance < 2 ? FSMStateType::MELEE_ATTACK : FSMStateType::MOVE_TO_TARGET );
  }
  else
  {
    _fsm.changeState( FSMStateType::WANDERING );
  }

  _fsm.update();
}

Actor* Ai::getActor()
{
  return _actor;
}

Target Ai::getTarget()
{
  if ( BUTCHER.getCurrentDungeon()->isInFov( getActor()->getTileCoord()) )
    return Target((Actor*)BUTCHER.getPlayer().get());

  return Target();
}

}
