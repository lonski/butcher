#include "character.h"
#include <data/actors_generated.h>
#include <actors/actions/die_action.h>
#include <butcher.h>
#include <dungeon/dungeon_state.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

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
    _level = data->level();
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
    notify(EventType::Modified);
}

int Character::getAttribute(AttributeType type)
{
  int base = _attributes[type];

  int mod = agregateModifiers([type](Modifier m){
    return m.attribute == type ? m.value : 0;
  });

  return base + mod;
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
  if ( hp != _hp )
  {
    int toSet = std::min(hp, getMaxHp());
    int diff = toSet - _hp;

    if ( diff != 0 )
    {
      _hp = toSet;

      if ( diff > 0 )
        fadeText("+"+ toStr(diff)+"HP", cc::Color4B::GREEN);
      else
        fadeText(toStr(diff), cc::Color4B::RED);

      notify(EventType::Modified);
    }
  }
}

int Character::takeDamage(Damage damage, std::shared_ptr<Actor> attacker)
{
  if ( getHp() <= 0 )
    return 0;

  int dmg = damage.roll() - getAttribute(AttributeType::DamageReduction);

  if ( dmg < 0 )
    dmg = 0;

  setHp( getHp() - dmg );

  if ( dmg == 0 )
    fadeText( "Resisted", cocos2d::Color4B::GRAY );

  if ( getHp() <= 0 )
    onDestroy(attacker);

  return dmg;
}

Damage Character::getDamage()
{  
  int mod = agregateModifiers([](Modifier m){
    return m.attribute == AttributeType::Damage ? m.value : 0;
  });

  Damage dmg = _damage;
  dmg.bonus += mod;

  return dmg;
}

void Character::setDamage(const Damage &damage)
{
  _damage = damage;
}

bool Character::canShootAt(cocos2d::Vec2)
{
  return false;
}

bool Character::isOutOfControl()
{
  int mod = agregateModifiers([](Modifier m){
    return m.special == SpecialModifierType::Paralyzed;
  });

  return mod > 0;
}

void Character::onDestroy(std::shared_ptr<Actor> killer)
{
  std::shared_ptr<Character> thisPtr = std::dynamic_pointer_cast<Character>(shared_from_this());
  killer->onKill( thisPtr );

  cc::Sprite* s = getSprite().release();

  for ( auto c : s->getChildren() )
    c->runAction( cc::Sequence::create(cc::FadeOut::create(0.5), cc::RemoveSelf::create(), nullptr) );

  s->runAction( cc::Sequence::create(cc::FadeOut::create(0.5), cc::RemoveSelf::create(), nullptr) );

  if ( !BUTCHER.getCurrentDungeon()->removeActor(shared_from_this(), false) )
    cc::log("%s Failed to remove actor!", __PRETTY_FUNCTION__);
}

bool Character::isAlive() const
{
  return getHp() > 0;
}

void Character::addEffect(const Effect &effect)
{
  if ( effect.getTurns() == -1 ) //Permament instant effect
  {
    for ( Modifier m : effect.getModifiers() )
    {
      if ( m.attribute == AttributeType::HP )
        setHp( getHp() + m.value );
      else if ( m.attribute == AttributeType::MAX_HP )
        setMaxHp(getMaxHp() + m.value);
      else
        setAttribute( m.attribute, getAttribute(m.attribute) + m.value );
    }
  }
  else
  {
    Actor::addEffect(effect);
  }
}

int Character::agregateModifiers(std::function<int (Modifier)> filter) const
{
  int sum = 0;

  for ( auto& kv : _effects )
    for ( Modifier m : kv.second.getModifiers() )
        sum += filter(m);

  return sum;
}

int Character::getMaxHp() const
{
  int base = _maxHp;

  int mod = agregateModifiers([](Modifier m){
    return m.attribute == AttributeType::Damage ? m.value : 0;
  });

  return base + mod;
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
