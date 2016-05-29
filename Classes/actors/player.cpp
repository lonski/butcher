#include "player.h"
#include <actors/monster.h>
#include <actors/actions/attack_action.h>

using namespace cocos2d;

namespace butcher {

Player* Player::create(const ActorData *data, Player *allocated)
{
  Player* p = allocated ? allocated : new Player;
  Character::create(data, p);
  return p;
}

Actor *Player::clone(Actor *allocated)
{
  Player* p = dynamic_cast<Player*>(allocated);
  if ( p == nullptr )
    p = new Player();

  Character::clone(p);

  return p;
}

bool Player::collide(Actor *obstacle)
{
  Monster* mob = dynamic_cast<Monster*>(obstacle);
  if ( mob )
  {
    return performAction( new AttackAction(Target(mob)) );
  }
  return false;
}

}
