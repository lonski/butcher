#ifndef ITEM_H
#define ITEM_H

#include <actors/actor.h>
#include <actors/item_slot_type.h>
#include <utils/damage.h>
#include <actors/attribute_type.h>

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

private:
  ItemSlotType _slot;
  Damage _damage;
  std::map<AttributeType, int> _attributes;
  int _breakChance;

};

}
#endif // ITEM_H
