#ifndef ITEM_H
#define ITEM_H

#include <actors/actor.h>
#include <actors/item_slot_type.h>
#include <utils/damage.h>
#include <actors/attribute_type.h>
#include <actors/item_category.h>
#include <actors/effects/effect_id.h>
#include <actors/use_target.h>

namespace butcher {

struct ActorData;

class Item : public Actor
{
public:
  Item(const ActorData* data);

  virtual int getZ() const;
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  bool isUsable() const;
  ItemSlotType getItemSlotType() const;

  Damage getDamage() const;
  virtual int getAttribute(AttributeType type);
  virtual void setAttribute(AttributeType type, int value);

  bool rollBreak() const;
  int getLevel() const;

  std::vector<std::string> getItemInfo();

  ItemCategory getCategory() const;
  int getRange() const;
  ActorID getAmmoId() const;
  EffectID getEffectID() const;
  UseTarget getUseTarget() const;
  int getHp() const;

private:
  ItemSlotType _slot;
  Damage _damage;
  std::map<AttributeType, int> _attributes;
  int _breakChance;
  int _level;
  ItemCategory _category;
  int _range;
  ActorID _ammoId;
  bool _usable;
  EffectID _effect;
  UseTarget _useTarget;
  int _hp;

};

}
#endif // ITEM_H
