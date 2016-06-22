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

std::unique_ptr<Actor> Player::clone(std::unique_ptr<Actor> allocated)
{
  Player* p = dynamic_cast<Player*>(allocated.release());
  if ( p == nullptr )
    p = new Player(nullptr);

  for ( auto& kv : _inventory )
    p->_inventory[kv.first] = kv.second;

  return std::move( Character::clone(std::move(std::unique_ptr<Actor>{p})) );
}

void Player::onCollide(std::shared_ptr<Actor> obstacle)
{
  std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(obstacle);
  if ( mob )
  {
    performAction( new AttackAction(Target(mob)) );
  }
}

}
