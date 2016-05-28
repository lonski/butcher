#include "monster.h"
#include <utils/directions.h>
#include <actors/actions/move_action.h>
#include <butcher.h>

using namespace cocos2d;

namespace butcher {

Monster* Monster::create(const ActorData *data, Monster *allocated)
{
  Monster* p = allocated ? allocated : new Monster;
  Character::create(data, p);
  return p;
}

Actor *Monster::clone(Actor *allocated)
{
  Monster* p = dynamic_cast<Monster*>(allocated);
  if ( p == nullptr )
    p = new Monster();

  Character::clone(p);

  return p;
}

void Monster::nextTurn()
{
  int i = RandomHelper::random_int( 0, 4 );

  performAction( new MoveAction( (Direction)i, BUTCHER.currentDungeon() ) );
}

}
