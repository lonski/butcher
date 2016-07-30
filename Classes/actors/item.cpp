#include "item.h"
#include <data/actors_generated.h>
#include "cocos2d.h"
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

Item::Item(const ActorData* data)
  : Actor(data)
  , _slot(ItemSlotType::NONE)
  , _breakChance(0)
  , _category(ItemCategory::None)
{
  if ( data )
  {
    _slot = static_cast<ItemSlotType>(data->body_slot());

    if ( data->attack() )
      setAttribute(AttributeType::Attack, data->attack());

    if ( data->defense() )
      setAttribute(AttributeType::Defense, data->defense());

    if ( data->damage_reduction() )
      setAttribute(AttributeType::DamageReduction, data->damage_reduction());

    if ( data->damage() )
      _damage.parse(data->damage()->c_str());

    _breakChance = data->break_chance();
    _level = data->level();
    _category = static_cast<ItemCategory>(data->category());
  }
}

int Item::getZ() const
{
  return 11;
}

std::unique_ptr<Actor> Item::clone(std::unique_ptr<Actor> allocated)
{
  Item* o = dynamic_cast<Item*>(allocated.release());
  if ( o == nullptr )
    o = new Item(nullptr);

  o->_slot = _slot;
  o->_attributes = _attributes;
  o->_damage = _damage;
  o->_breakChance = _breakChance;
  o->_level = _level;
  o->_category = _category;

  return std::move(Actor::clone(std::unique_ptr<Actor>{o}));
}

bool Item::isUsable() const
{
  return false;
}

ItemSlotType Item::getItemSlotType() const
{
  return _slot;
}

Damage Item::getDamage() const
{
  return _damage;
}

int Item::getAttribute(AttributeType type)
{
  return _attributes[type];
}

void Item::setAttribute(AttributeType type, int value)
{
  _attributes[type] = value;
}

bool Item::rollBreak() const
{
  int roll = cc::RandomHelper::random_int(1,1000000);
  return  roll <= _breakChance;
}

int Item::getLevel() const
{
  return _level;
}

std::vector<std::string> Item::getItemInfo()
{
  std::vector<std::string> info;

  if ( _category != ItemCategory::None )
    info.push_back("Category: " + ItemCategory2Str(_category));

  if ( _level > 0 )
    info.push_back("Level: " + toStr(_level));

  if ( _damage.toInt() != 0 )
    info.push_back("Damage: " + _damage.toString());

  for ( auto a : AttributeType() )
  {
    int val = getAttribute(a);
    if ( val != 0 )
      info.push_back(AttributeType2Str(a) + ": " + (val > 0 ? "+" : "") + toStr(val));
  }

  return info;
}

ItemCategory Item::getCategory() const
{
  return _category;
}

}
