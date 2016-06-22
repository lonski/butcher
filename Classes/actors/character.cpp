#include "character.h"
#include <data/actors_generated.h>
#include <actors/actions/die_action.h>

namespace butcher {

Character::Character(const ActorData* data)
  : Actor(data)
  , _level(0)
  , _exp(0)
{
  if ( data )
  {
    _hp = data->hp();
    setAttribute(AttributeType::Attack, data->attack());
    setAttribute(AttributeType::Defense, data->defense());
    setAttribute(AttributeType::Damage, data->damage());
    setAttribute(AttributeType::DamageReduction, data->damage_reduction());
  }
}

Character::~Character()
{
}

int Character::level() const
{
    return _level;
}

void Character::setLevel(int level)
{
    _level = level;
}

int Character::exp() const
{
    return _exp;
}

void Character::setExp(int exp)
{
    _exp = exp;
}

int Character::attribute(AttributeType type)
{
    return _attributes[type];
}

void Character::setAttribute(AttributeType type, int value)
{
  _attributes[type] = value;
}

int Character::hp() const
{
  return _hp;
}

void Character::setHp(int hp)
{
  _hp = hp;
}

int Character::takeDamage(int damage, std::shared_ptr<Actor> attacker)
{
  damage -= attribute(AttributeType::DamageReduction);

  if ( damage < 0 )
    damage = 0;

  _hp -= damage;

  fadeText( "-" + cocos2d::Value(damage).asString(), cocos2d::Color4B::RED );

  if ( _hp <= 0 )
    performAction( DieAction(attacker) );

  return damage;
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
  }

  return std::move(Actor::clone(std::unique_ptr<Actor>{c}));
}

int Character::getZ() const
{
  return 20;
}

}
