#include "character.h"

using namespace cocos2d;

namespace butcher {

Character::Character()
    : _level(0)
    , _exp(0)
{
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

Character* Character::create(const ActorData *data, Character *allocated)
{
  if ( allocated )
  {
    //fill with character data
  }
  return allocated;
}

Actor* Character::clone(Actor* allocated)
{
  Character* c = dynamic_cast<Character*>(allocated);
  if ( c != nullptr )
  {
    Actor::clone(c);

    c->_attributes = _attributes;
    c->_level = _level;
    c->_exp = _exp;
  }

  return c;
}

}
