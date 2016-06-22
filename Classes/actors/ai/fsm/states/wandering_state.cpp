#include "wandering_state.h"
#include <utils/directions.h>
#include <actors/actions/move_action.h>
#include "cocos2d.h"
#include <actors/ai/ai.h>
#include <actors/actor.h>

namespace butcher {

namespace state{

Wandering::Wandering()
{
}

FSMStateType Wandering::getType() const
{
  return FSMStateType::WANDERING;
}

void Wandering::update()
{
  if ( validatePreconditions() )
  {
    int i = cocos2d::RandomHelper::random_int( 0, 4 );
    _ai->getActor()->performAction( MoveAction((Direction::Symbol)i) );
  }
}

bool Wandering::canEnter()
{
  return true;
}

}}
