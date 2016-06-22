#include "attack_action.h"
#include "cocos2d.h"
#include <actors/actor.h>
#include <actors/character.h>

namespace butcher {

AttackAction::AttackAction(Target target)
  : _target(target)
{
}

bool AttackAction::perform(std::shared_ptr<Actor> performer) const
{
  if ( !performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return false;
  }

  std::shared_ptr<Character> a = std::dynamic_pointer_cast<Character>(performer);

  if ( !a )
  {
    cocos2d::log("%s: performer is not a character.", __PRETTY_FUNCTION__);
    return false;
  }

  std::shared_ptr<Character> t = std::dynamic_pointer_cast<Character>(_target.first());

  if ( !t )
  {
    cocos2d::log("%s: target is null.", __PRETTY_FUNCTION__);
    return false;
  }

  int dice_size = (a->getAttribute(AttributeType::Attack) + t->getAttribute(AttributeType::Defense)) / 2;

  int attacker_score = ( a->getAttribute(AttributeType::Attack)  + cocos2d::RandomHelper::random_int(0, dice_size) );
  int target_score =   ( t->getAttribute(AttributeType::Defense) + cocos2d::RandomHelper::random_int(0, dice_size) );

  if ( attacker_score > target_score )
    t->takeDamage( a->getAttribute(AttributeType::Damage), performer );
  else
    a->fadeText("miss", cocos2d::Color4B(32,32,32,255));

  return true;
}

}
