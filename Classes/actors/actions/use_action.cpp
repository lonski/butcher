#include "use_action.h"
#include "cocos2d.h"
#include <butcher.h>
#include <actors/character.h>
#include <actors/player.h>

namespace cc = cocos2d;

namespace butcher {

UseAction::UseAction(std::shared_ptr<Item> item)
  : _item(item, 1)
{
}

UseAction::UseAction(const AmountedItem &amountedItem)
  : _item(amountedItem)
{
}

bool UseAction::perform(std::shared_ptr<Actor> user)
{
  _user = std::dynamic_pointer_cast<Character>(user);

  if ( !_user )
  {
    cc::log("%s user is not a character", __PRETTY_FUNCTION__);
    return false;
  }

  if ( !_item.item )
  {
    cc::log("%s item is null", __PRETTY_FUNCTION__);
    return false;
  }

  _effect = BUTCHER.effectsDatabase().createEffect(_item.item->getEffectID());

  bool ret = false;

  switch (_item.item->getUseTarget())
  {
    case UseTarget::Self:
      ret = useOnSelf();
    break;
    case UseTarget::Weapon:
      ret = useOnWeapon();
    break;
    case UseTarget::Floor:
      ret = useOnFloor();
    break;
    default:
      cc::log("%s invalid UseTarget: %d", __PRETTY_FUNCTION__, (int)_item.item->getUseTarget());
    break;
  }

  return ret;
}

bool UseAction::useOnSelf()
{
  //Apply effect
  if ( _effect.getID() != EffectID::None )
  {
    if ( _user == BUTCHER.getPlayer() )
    {
      std::string message = _effect.getName() + " fades";
      _effect.setOnRemoveFn([=](){
        BUTCHER.getPlayer()->fadeText( message, cc::Color4B::ORANGE, 1, false);
      });
    }

    _user->addEffect( _effect );
    _user->fadeText( _effect.getName(), cc::Color4B::ORANGE, 1, false);
  }

  //Modify HP
  if ( _item.item->getHp() != 0 )
    _user->setHp( _user->getHp() + _item.item->getHp() );

  return true;
}

bool UseAction::useOnWeapon()
{
  std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(_user);

  if ( !_user )
  {
    cc::log("%s user is not a player", __PRETTY_FUNCTION__);
    return false;
  }

  AmountedItem wpn = player->getInventory().equipped(ItemSlotType::WEAPON);
  if ( !wpn.item )
  {
    cc::log("%s weapon is not equipped", __PRETTY_FUNCTION__);
    return false;
  }

  if ( _effect.getID() == EffectID::None )
  {
    cc::log("%s invalid effect", __PRETTY_FUNCTION__);
    return false;
  }

  std::string message = "Weapon: " + _effect.getName() + " fades";
  _effect.setOnRemoveFn([=](){
    BUTCHER.getPlayer()->fadeText( message, cc::Color4B::ORANGE, 1, false);
  });

  wpn.item->addEffect( _effect );
  player->fadeText( "Weapon: " + _effect.getName(), cc::Color4B::ORANGE, 1, false);

  return true;
}

bool UseAction::useOnFloor()
{
  return false;
}

}
