#include "recipe.h"
#include "cocos2d.h"
#include <butcher.h>

namespace cc = cocos2d;

namespace butcher {

Recipe::Recipe()
  : _successChance(100)
  , _inventory(nullptr)
  , _level(0)
  , _type(CraftType::NONE)
  , _cost(0)
  , _id(RecipeID::NONE)
{
}

std::unique_ptr<Recipe> Recipe::create(const RecipeData* data)
{
  std::unique_ptr<Recipe> r(new Recipe());

  r->_product = AmountedItem( std::shared_ptr<Item>(BUTCHER.actorsDatabase().createActor<Item>((ActorID)data->product_id()).release()),
                              data->product_amount() );

  r->_successChance = data->success_chance();
  r->_level = data->level();
  r->_type = static_cast<CraftType>(data->type());
  r->_cost = data->cost();
  r->_id = static_cast<RecipeID>(data->id());

  for(unsigned i = 0; i < data->ingridients()->Length(); ++i)
  {
    const IngridientData* idata = data->ingridients()->Get(i);
    r->_ingridientList[ (ActorID)idata->item_id() ] = idata->amount();
  }

  return r;
}

std::unique_ptr<Recipe> Recipe::clone()
{
  std::unique_ptr<Recipe> c(new Recipe());

  c->_ingridientList = _ingridientList;
  c->_product = _product;
  c->_successChance = _successChance;
  c->_level = _level;
  c->_type = _type;
  c->_cost = _cost;
  c->_id = _id;

  return c;
}

bool Recipe::produce()
{
  if (!checkIngridients())
    return false;

  //remove ingridients from inventory
  for ( auto& kv : _ingridientList )
  {
    for(auto kvi : _inventory->getItems())
    {
      if ( kvi.second.item->getID() == kv.first )
      {
        AmountedItem toRemove = kvi.second;
        toRemove.amount = kv.second;

        if ( !_inventory->removeItem(toRemove) )
        {
          cc::log("%s failed to remove item from inventory", __PRETTY_FUNCTION__);
          return false;
        }
      }
    }
  }

  if ( cc::RandomHelper::random_int(1,100) > _successChance )
  {
    cc::log("%s: failed to produce item.", __PRETTY_FUNCTION__);
    return false;
  }

  _inventory->addItem( AmountedItem( std::shared_ptr<Item>(dynamic_cast<Item*>(_product.item->clone().release())),
                                    _product.amount) );

  return true;
}

Recipe::IngridientList Recipe::getIngridientList() const
{
  return _ingridientList;
}

bool Recipe::checkIngridients()
{
  if ( !_inventory )
  {
    cc::log("%s: inventory is null.", __PRETTY_FUNCTION__);
    return false;
  }

  for ( auto& kv : _ingridientList )
  {
    bool present = false;
    for(auto kvi : _inventory->getItems())
    {
      if ( kvi.second.item->getID() == kv.first && kvi.second.amount >= kv.second )
      {
        present = true;
        break;
      }
    }
    if ( !present )
    {
      cc::log("%s: missing ingridient ID%d", __PRETTY_FUNCTION__, (int)kv.first);
      return false;
    }
  }

  return true;
}

int Recipe::getLevel() const
{
  return _level;
}

CraftType Recipe::getType() const
{
  return _type;
}

int Recipe::getSuccessChance() const
{
  return _successChance;
}

int Recipe::getCost() const
{
  return _cost;
}

void Recipe::setInventory(Inventory *inv)
{
  _inventory = inv;
}

RecipeID Recipe::getId() const
{
  return _id;
}

}
