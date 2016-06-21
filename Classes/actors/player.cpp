#include "player.h"
#include <actors/monster.h>
#include <actors/actions/attack_action.h>
#include <actors/object.h>
#include <butcher.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

Player::Player(const ActorData *data)
  : Character(data)
{
}

Actor* Player::clone(Actor *allocated)
{
  Player* p = dynamic_cast<Player*>(allocated);
  if ( p == nullptr )
    p = new Player(nullptr);

  for ( auto& kv : _inventory )
    p->_inventory[kv.first] = kv.second;

  Character::clone(p);

  return p;
}

void Player::onCollide(Actor *obstacle)
{
  Monster* mob = dynamic_cast<Monster*>(obstacle);
  if ( mob )
  {
    performAction( new AttackAction(Target(mob)) );
  }
}

}
