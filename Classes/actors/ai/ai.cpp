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

  FSMStateType state = cc::RandomHelper::random_int(0,100) < 5 ? FSMStateType::WANDERING
                                                               : FSMStateType::IDLE;

  if ( getTarget().pos != cc::Vec2::ZERO )
  {
    //  Enemy not in FoV.
    //  Move to position where he was last seen.
    if ( getTarget().actors.empty() )
    {
      state = FSMStateType::MOVE_TO_TARGET;
    }
    // Enemy in FoV. Determine action.
    else
    {
      float distance = calculateDistance(getActor()->getTileCoord(), getTarget().pos);

      //Enemy in neighbour tile
      if ( distance < 2 )
      {
        state = FSMStateType::MELEE_ATTACK;
      }
      //Enemy visible in range
      else
      {
        state = _fsm.canEnter(FSMStateType::RANGED_ATTACK) ? FSMStateType::RANGED_ATTACK
                                                           : FSMStateType::MOVE_TO_TARGET;
      }
    }
  }

  _fsm.changeState(state);
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
    _target = Target(std::dynamic_pointer_cast<Actor>(BUTCHER.getPlayer()));
  else
    _target.actors.clear();

  if ( getActor()->getTileCoord() == _target.pos )
    clearTarget();
}

}
