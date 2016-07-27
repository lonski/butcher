#ifndef INVENTORY_H
#define INVENTORY_H

#include <memory>
#include <map>
#include <actors/item.h>
#include <actors/amounted_item.h>

namespace butcher {

class Inventory
{
public:
  typedef std::map<ActorID, AmountedItem> ItemContainer;

  Inventory();

  Inventory(const Inventory& inv);
  Inventory& operator=(const Inventory& rhs);
  std::unique_ptr<Inventory> clone();

  bool addItem(std::shared_ptr<Item> item, int amount = 1);
  bool addItem(const AmountedItem& i);
  bool removeItem(const AmountedItem& i);

  AmountedItem getItem(ActorID id);
  ItemContainer getItems() const;

private:
  ItemContainer _items;

};

}

#endif // INVENTORY_H
