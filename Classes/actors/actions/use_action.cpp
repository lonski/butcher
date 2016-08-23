#include "use_action.h"
#include "cocos2d.h"
#include <butcher.h>
#include <actors/character.h>

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
  EffectID effectId = _item.item->getEffectID();
  if ( effectId != EffectID::None )
    _user->addEffect( BUTCHER.effectsDatabase().createEffect(effectId) );

  //Modify HP
  if ( _item.item->getHp() != 0 )
    _user->setHp( _user->getHp() + _item.item->getHp() );

  return true;
}

bool UseAction::useOnWeapon()
{
  return false;
}

bool UseAction::useOnFloor()
{
  return false;
}

}
