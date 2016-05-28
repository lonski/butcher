#include "player.h"

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

}
