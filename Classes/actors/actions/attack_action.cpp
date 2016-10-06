#include "attack_action.h"
#include "cocos2d.h"
#include <actors/actor.h>
#include <actors/character.h>
#include <actors/player.h>
#include <actors/inventory.h>
#include <actors/item.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

namespace butcher {

AttackAction::AttackAction(Target target)
  : _target(target)
{
}

ActorAction::Result AttackAction::perform(std::shared_ptr<Actor> performer)
{
  if ( !performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  std::shared_ptr<Character> a = std::dynamic_pointer_cast<Character>(performer);

  if ( !a )
  {
    cocos2d::log("%s: performer is not a character.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  std::shared_ptr<Character> t = std::dynamic_pointer_cast<Character>(_target.first());

  if ( !t )
  {
    cocos2d::log("%s: target is null.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  float atk = a->getAttribute(AttributeType::Attack);
  float def = t->getAttribute(AttributeType::Defense);

  float score = std::min( (atk/def/2) + 0.05, 0.95);

  if ( score >= cc::RandomHelper::random_real<float>(0,1.f) )
  {
    t->takeDamage( a->getDamage(), performer );

    auto wpn = getWeapon(a);

    if ( wpn )
    {
      for ( Effect e : wpn->getEffects() )
      {
        t->addEffect( e );
        t->fadeText( e.getName(), cc::Color4B::ORANGE, 1, false);
      }
    }

    a->onHit(t);
  }
  else
  {
    t->fadeText("miss", cocos2d::Color4B(128,128,128,255));
  }
  return ActorAction::Result::OK;
}

std::shared_ptr<Item> AttackAction::getWeapon(std::shared_ptr<Character> c)
{
  std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(c);

  if ( !p )
    return nullptr;

  AmountedItem ai = p->getInventory().equipped(ItemSlotType::WEAPON);
  return ai.item;
}

}
