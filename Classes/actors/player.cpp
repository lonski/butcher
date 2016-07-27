#include "player.h"
#include <actors/monster.h>
#include <actors/actions/attack_action.h>
#include <actors/object.h>
#include <actors/instances/door.h>
#include <butcher.h>
#include <cmath>
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

  p->_inventory = _inventory;

  return std::move( Character::clone(std::move(std::unique_ptr<Actor>{p})) );
}

void Player::onCollide(std::shared_ptr<Actor> obstacle)
{
  std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(obstacle);
  if ( mob )
  {
    performAction( AttackAction(Target(mob)) );
  }

  std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(obstacle);
  if ( door )
  {
    door->open();
  }
}

void Player::onKill(std::shared_ptr<Character> killed)
{
  setExp( getExp() + killed->getExp() );
}

Inventory& Player::getInventory()
{
  return _inventory;
}

Damage Player::getDamage()
{
  AmountedItem i = getInventory().equipped(ItemSlotType::WEAPON);
  if ( i.item )
  {
    return i.item->getDamage();
  }

  return Character::getDamage();
}

int Player::getAttribute(AttributeType type)
{
  int atr = Character::getAttribute(type);
  for ( auto s : ItemSlotType() )
  {
    AmountedItem i = getInventory().equipped(s);
    if ( i.item != nullptr )
    {
      atr += i.item->getAttribute(type);
    }
  }

  return atr;
}

void Player::setExp(int exp)
{
  Character::setExp(exp);
  if ( getExp() >= getExpForNextLevel() )
  {
    setExp(getExpForNextLevel() - getExp());
    setLevel( getLevel() + 1 );
  }
}

int Player::getExpForNextLevel() const
{
  return std::pow(2, getLevel() - 1) * 100;
}

}
