#include "ai.h"
#include <actors/ai/fsm/states/fsm_state.h>
#include <actors/actor.h>
#include <butcher.h>
#include <utils/utils.h>
#include <dungeon/dungeon_state.h>
#include <actors/player.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

Ai::Ai(std::shared_ptr<Actor> actor)
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
    _fsm.changeState( cc::RandomHelper::random_int(0,100) < 20 ? FSMStateType::WANDERING
                                                               : FSMStateType::IDLE );
  }

  _fsm.update();
}

std::shared_ptr<Actor> Ai::getActor()
{
  return _actor;
}

Target Ai::getTarget()
{
  if ( BUTCHER.getCurrentDungeon()->isInFov( getActor()->getTileCoord()) )
    return Target( std::dynamic_pointer_cast<Actor>(BUTCHER.getPlayer()) );

  return Target();
}

}
