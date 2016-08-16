#include "shot_action.h"
#include <actors/player.h>
#include <actors/actions/attack_action.h>

namespace cc = cocos2d;

namespace butcher {

ShotAction::ShotAction(Target target)
  : _target(target)
{
}

bool ShotAction::perform(std::shared_ptr<Actor> performer)
{
  _performer = performer;
  if ( !performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return false;
  }

  std::shared_ptr<Character> shotter = std::dynamic_pointer_cast<Character>(performer);

  if ( !shotter )
  {
    cocos2d::log("%s: shotter is not a character.", __PRETTY_FUNCTION__);
    return false;
  }

  if ( !shotter->canShootAt(_target.pos) )
  {
    if ( !hasAmmo())
      shotter->fadeText("Out of ammo!", cc::Color4B::RED);

    return false;
  }


  std::shared_ptr<Character> victim = std::dynamic_pointer_cast<Character>(_target.first());
  if ( !victim )
  {
    cocos2d::log("%s: victim is not a character.", __PRETTY_FUNCTION__);
    return false;
  }

  if ( !takeAmmo() )
    return false;

  //TODO:
  //check path
  //run animation

  return performer->performAction( new AttackAction(_target));
}

bool ShotAction::takeAmmo()
{
  std::shared_ptr<Player> shotter = std::dynamic_pointer_cast<Player>(_performer);

  if ( !shotter )
    return true; //only player has to use ammo

  AmountedItem wpn = shotter->getInventory().equipped(ItemSlotType::WEAPON);
  bool taken = shotter->getInventory().removeItem(wpn.item->getAmmoId(), 1);
  if ( !taken )
    shotter->fadeText("Out of ammo!", cc::Color4B::RED);

  return taken;
}

bool ShotAction::hasAmmo()
{
  std::shared_ptr<Player> shotter = std::dynamic_pointer_cast<Player>(_performer);

  if ( !shotter )
    return true; //only player has to use ammo

  AmountedItem wpn = shotter->getInventory().equipped(ItemSlotType::WEAPON);

  if ( !wpn.item )
    return true;

  auto item = shotter->getInventory().getItem(wpn.item->getAmmoId());

  return item.amount > 0;
}

}
