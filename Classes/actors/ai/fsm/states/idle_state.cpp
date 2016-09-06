#include "idle_state.h"
#include <actors/ai/ai.h>
#include <actors/character.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

namespace state{

Idle::Idle()
{
}

FSMStateType Idle::getType() const
{
  return FSMStateType::IDLE;
}

void Idle::update()
{
  if ( validatePreconditions() )
  {
    //Autoheal
    std::shared_ptr<Character> c = std::dynamic_pointer_cast<Character>(_ai->getActor());
    if ( c&& c->getHp() < c->getMaxHp())
      if ( cc::RandomHelper::random_int(0,100) < 25 )
        c->setHp(c->getHp() + 1);
  }
}

bool Idle::canEnter()
{
  return true;
}

}}
