#include "monster.h"
#include <butcher.h>



namespace butcher {

Monster::Monster(const ActorData* data)
  : _ai(this)
  , Character(data)
{
}

Actor *Monster::clone(Actor *allocated)
{
  Monster* p = dynamic_cast<Monster*>(allocated);
  if ( p == nullptr )
    p = new Monster(nullptr);

  Character::clone(p);

  return p;
}

void Monster::nextTurn()
{
  _ai.update();
}

}
