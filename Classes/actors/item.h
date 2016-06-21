#ifndef ITEM_H
#define ITEM_H

#include <actors/actor.h>

namespace butcher {

struct ActorData;

class Item : public Actor
{
public:
  Item(const ActorData* data);

  virtual int getZ() const;

  virtual Actor* clone(Actor* allocated = nullptr);
};

struct AmountedItem
{
  AmountedItem()
    : amount(0)
  {
  }

  AmountedItem(const AmountedItem& i)
  {
    *this = i;
  }

  AmountedItem& operator=(const AmountedItem& rhs)
  {
    item.reset( dynamic_cast<Item*>(rhs.item->clone()) );
    amount = rhs.amount;

    return *this;
  }

  std::shared_ptr<Item> item;
  int amount;
};


}
#endif // ITEM_H
