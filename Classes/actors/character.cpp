#include "character.h"
#include <data/actors_generated.h>
#include <actors/actions/die_action.h>

namespace butcher {

Character::Character(const ActorData* data)
  : Actor(data)
  , _level(1)
  , _exp(0)
{
  if ( data )
  {
    _maxHp = data->hp();
    _hp = _maxHp;
    setAttribute(AttributeType::Attack, data->attack());
    setAttribute(AttributeType::Defense, data->defense());
    setAttribute(AttributeType::DamageReduction, data->damage_reduction());
    _damage.parse(data->damage()->c_str());
    _exp = data->exp();
  }
}

Character::~Character()
{
}

int Character::getLevel() const
{
    return _level;
}

void Character::setLevel(int level)
{
    _level = level;
}

int Character::getExp() const
{
    return _exp;
}

void Character::setExp(int exp)
{
    _exp = exp;
    notify();
}

int Character::getAttribute(AttributeType type)
{
    return _attributes[type];
}

void Character::setAttribute(AttributeType type, int value)
{
  _attributes[type] = value;
}

int Character::getHp() const
{
  return _hp;
}

void Character::setHp(int hp)
{
  _hp = hp;
  notify();
}

int Character::takeDamage(Damage damage, std::shared_ptr<Actor> attacker)
{
  int dmg = damage.roll() - getAttribute(AttributeType::DamageReduction);

  if ( dmg < 0 )
    dmg = 0;

  setHp( getHp() - dmg );

  fadeText( "-" + cocos2d::Value(dmg).asString(), cocos2d::Color4B::RED );

  if ( getHp() <= 0 )
    performAction( DieAction(attacker) );

  return dmg;
}

Damage Character::getDamage()
{  
  return _damage;
}

void Character::setDamage(const Damage &damage)
{
  _damage = damage;
}

int Character::getMaxHp() const
{
  return _maxHp;
}

void Character::setMaxHp(int maxHp)
{
  _maxHp = maxHp;
}

std::unique_ptr<Actor> Character::clone(std::unique_ptr<Actor> allocated)
{
  Character* c = dynamic_cast<Character*>(allocated.release());
  if ( c != nullptr )
  {
    c->_attributes = _attributes;
    c->_level = _level;
    c->_exp = _exp;
    c->_hp = _hp;
    c->_maxHp = _maxHp;
    c->_damage = _damage;
  }

  return std::move(Actor::clone(std::unique_ptr<Actor>{c}));
}

int Character::getZ() const
{
  return 20;
}

}
