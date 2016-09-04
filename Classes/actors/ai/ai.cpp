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
  updateTarget();

  if ( getTarget().pos != cc::Vec2::ZERO )
  {
    /** Player not in FoV.
     *  Move to position where he was last seen.
      */
    if ( getTarget().actors.empty() )
    {
      _fsm.changeState( FSMStateType::MOVE_TO_TARGET );
    }
    /** Player in FoV. Determine action.
      */
    else
    {
      float distance = calculateDistance(getActor()->getTileCoord(), getTarget().pos);
      _fsm.changeState( distance < 2 ? FSMStateType::MELEE_ATTACK : FSMStateType::MOVE_TO_TARGET );
    }
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
  return _target;
}

void Ai::clearTarget()
{
  _target = Target();
}

void Ai::updateTarget()
{
  if ( BUTCHER.getCurrentDungeon()->isInFov( getActor()->getTileCoord()) )
    _target = std::dynamic_pointer_cast<Actor>(BUTCHER.getPlayer());
  else
    _target.actors.clear();

  if ( getActor()->getTileCoord() == _target.pos )
    clearTarget();
}

}
