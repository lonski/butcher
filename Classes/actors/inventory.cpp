#include "inventory.h"
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

Inventory::Inventory()
{

}

Inventory::Inventory(const Inventory& inv)
{
  *this = inv;
}

Inventory &Inventory::operator=(const Inventory &rhs)
{
  _items.clear();

  for ( auto& kv : rhs._items )
    _items[kv.first] = kv.second;

  return *this;
}

std::unique_ptr<Inventory> Inventory::clone()
{
  return std::unique_ptr<Inventory>( new Inventory(*this) );
}

bool Inventory::addItem(std::shared_ptr<Item> item, int amount)
{
  return addItem(AmountedItem(item, amount));
}

bool Inventory::addItem(const AmountedItem &i)
{
  if ( !i.item )
  {
    cc::log("%s item is null.", __PRETTY_FUNCTION__);
    return false;
  }

  if ( i.amount == 0 )
  {
    cc::log("%s amount = 0.", __PRETTY_FUNCTION__);
    return false;
  }

  auto it = _items.find(i.item->getID());

  if ( it != _items.end() )
  {
    AmountedItem& my_item = it->second;
    my_item.amount += i.amount;
  }
  else
  {
    _items[i.item->getID()] = i;
  }

  return true;
}

bool Inventory::removeItem(const AmountedItem &i)
{
  if ( !i.item )
  {
    cc::log("%s item is null.", __PRETTY_FUNCTION__);
    return false;
  }

  if ( i.amount == 0 )
  {
    return false;
  }

  auto it = _items.find(i.item->getID());

  if ( it == _items.end() )
    return false;

  AmountedItem& my_item = it->second;
  if ( my_item.amount > i.amount )
    my_item.amount -= i.amount;
  else
    _items.erase(it);

  return true;
}

AmountedItem Inventory::getItem(ActorID id)
{
  auto it = _items.find(id);
  return it != _items.end() ? it->second : AmountedItem();
}

Inventory::ItemContainer Inventory::getItems() const
{
  return _items;
}

}
