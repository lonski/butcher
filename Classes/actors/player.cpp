#include "player.h"
#include <actors/monster.h>
#include <actors/actions/attack_action.h>
#include <actors/object.h>
#include <actors/instances/door.h>
#include <butcher.h>
#include <cmath>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

Player::Player(const ActorData *data)
  : Character(data)
{
}

std::unique_ptr<Actor> Player::clone(std::unique_ptr<Actor> allocated)
{
  Player* p = dynamic_cast<Player*>(allocated.release());
  if ( p == nullptr )
    p = new Player(nullptr);

  p->_inventory = _inventory;
  p->_craftbook = _craftbook;

  return std::move( Character::clone(std::move(std::unique_ptr<Actor>{p})) );
}

void Player::load(const SaveData *data)
{
  if ( data )
  {
    _exp = data->exp();
    _level = data->level();

    _inventory = Inventory();
    auto inv = data->inventory();
    for ( unsigned i = 0; i < inv->Length(); ++i)
    {
      const InventoryData* item_data = inv->Get(i);

      //cc::log("%d %d %d", item_data->amount(), item_data->item_id(), item_data->equipped());
      AmountedItem item;
      item.amount = item_data->amount();
      item.item = BUTCHER.actorsDatabase().createActor<Item>( (ActorID)item_data->item_id() );

      if ( item_data->equipped() )
        _inventory.equip(item);
      else
        _inventory.addItem(item);
    }

    auto cb = data->craftbook();

    _craftbook = CraftBook();
    _craftbook.setFreePoints( cb->free_points());

    auto recs = cb->recipes();
    for (unsigned i = 0; i < recs->Length(); ++i )
    {
      _craftbook.addRecipe( BUTCHER.recipesDatabase().createRecipe( (RecipeID)recs->Get(i) ) );
    }
  }
}

void Player::onCollide(std::shared_ptr<Actor> obstacle)
{
  std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(obstacle);
  if ( mob )
  {
    performAction( AttackAction(Target(mob)) );
  }

  std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(obstacle);
  if ( door )
  {
    door->open();
  }
}

void Player::onKill(std::shared_ptr<Character> killed)
{
  setExp( getExp() + killed->getExp() );
}

void Player::onHit(std::shared_ptr<Character>)
{
  AmountedItem wpn = getInventory().equipped(ItemSlotType::WEAPON);
  if ( wpn.item && wpn.item->rollBreak() )
  {
    getInventory().unequip(ItemSlotType::WEAPON);
    fadeText("Weapon cracked!", cc::Color4B::RED);
  }
}

Inventory& Player::getInventory()
{
  return _inventory;
}

Damage Player::getDamage()
{
  AmountedItem i = getInventory().equipped(ItemSlotType::WEAPON);
  if ( i.item )
  {
    return i.item->getDamage();
  }

  return Character::getDamage();
}

int Player::getAttribute(AttributeType type)
{
  int atr = Character::getAttribute(type);
  for ( auto s : ItemSlotType() )
  {
    AmountedItem i = getInventory().equipped(s);
    if ( i.item != nullptr )
    {
      atr += i.item->getAttribute(type);
    }
  }

  return atr;
}

int Player::takeDamage(Damage damage, std::shared_ptr<Actor> attacker)
{
  int dmg = Character::takeDamage(damage, attacker);

  for ( auto s : ItemSlotType() )
  {
    if ( s != ItemSlotType::WEAPON )
    {
      AmountedItem i = getInventory().equipped(s);
      if ( i.item && i.item->rollBreak() )
      {
        getInventory().unequip(s);
        fadeText(ItemSlotType2Str(s) + " destroyed!", cc::Color4B::RED);
      }
    }
  }

  return dmg;
}

void Player::setExp(int exp)
{
  Character::setExp(exp);
  if ( getExp() >= getExpForNextLevel() )
  {
    setExp(getExpForNextLevel() - getExp());
    setLevel( getLevel() + 1 );
    giveLevelUpBonuses();
  }
}

int Player::getExpForNextLevel() const
{
  return std::pow(2, getLevel() - 1) * 100;
}

void Player::giveLevelUpBonuses()
{
  int newCraftPoints = getCraftPointsOnLevel(getLevel());
  getCraftbook().setFreePoints(getCraftbook().getFreePoints() + newCraftPoints + 20);
  setHp( getMaxHp() );
  notify(EventType::LevelUP);
}

CraftBook& Player::getCraftbook()
{
  return _craftbook;
}

}
