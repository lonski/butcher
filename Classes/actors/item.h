#ifndef ITEM_H
#define ITEM_H

#include <actors/actor.h>
#include <actors/item_slot_type.h>

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

};

}
#endif // ITEM_H
