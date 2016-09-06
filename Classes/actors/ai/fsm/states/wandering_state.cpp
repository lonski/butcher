#include "wandering_state.h"
#include <utils/directions.h>
#include <actors/actions/move_action.h>
#include "cocos2d.h"
#include <actors/ai/ai.h>
#include <actors/character.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

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
    //Autoheal
    std::shared_ptr<Character> c = std::dynamic_pointer_cast<Character>(_ai->getActor());
    if ( c && c->getHp() < c->getMaxHp())
      if ( cc::RandomHelper::random_int(0,100) < 35 )
        c->setHp(c->getHp() + 1);

    //Move randomly
    int i = cocos2d::RandomHelper::random_int( 0, 4 );
    _ai->getActor()->performAction( new MoveAction((Direction::Symbol)i) );
  }
}

bool Wandering::canEnter()
{
  return true;
}

}}
