#ifndef AMOUNTED_ITEM_H
#define AMOUNTED_ITEM_H

#include <memory>
#include <actors/item.h>

namespace butcher {

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

#endif // AMOUNTED_ITEM_H
