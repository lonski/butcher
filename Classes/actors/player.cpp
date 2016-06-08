#include "player.h"
#include <actors/monster.h>
#include <actors/actions/attack_action.h>
#include <actors/object.h>
#include <butcher.h>

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

  Object* obj = dynamic_cast<Object*>(obstacle);
  if ( obj )
  {
    switch( static_cast<Actor::ID>(obj->id()) )
    {
      case Actor::StairsDownID:
        BUTCHER.goToNextLevel();
        break;
      case Actor::StairsUpID:
        BUTCHER.goToLevel(BUTCHER.getDungeonLevel() - 1);
        break;
      default:;
    }
  }

  return false;
}

}
