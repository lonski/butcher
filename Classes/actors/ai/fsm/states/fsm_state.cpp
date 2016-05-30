#include "fsm_state.h"
#include "cocos2d.h"
#include <actors/ai/fsm/states/idle_state.h>
#include <actors/ai/fsm/states/wandering_state.h>
#include <actors/ai/fsm/states/move_to_target_state.h>
#include <actors/ai/ai.h>

namespace butcher {

FSMState::FSMState()
  : _ai(nullptr)
{
}

FSMState::~FSMState()
{
}

FSMStatePtr FSMState::create(FSMStateType type)
{
  FSMStatePtr s;

  switch( type )
  {
    case FSMStateType::WANDERING:      s.reset( new state::Wandering ); break;
    case FSMStateType::MELEE_ATTACK:   s.reset( ); break;
    case FSMStateType::MOVE_TO_TARGET: s.reset( new state::MoveToTarget ); break;
    case FSMStateType::EVADING:        s.reset( ); break;
    case FSMStateType::IDLE:           s.reset( new state::Idle ); break;

    default:;
  }

  return s;
}

void FSMState::setAi(Ai *ai)
{
  _ai = ai;
}

bool FSMState::validatePreconditions()
{
  if ( !_ai || !_ai->getActor() )
  {
    if ( !_ai )
      cocos2d::log("%s: ai is null.", __PRETTY_FUNCTION__);
    else
      cocos2d::log("%s: actor is null.", __PRETTY_FUNCTION__);

    return false;
  }
  return true;
}

}
