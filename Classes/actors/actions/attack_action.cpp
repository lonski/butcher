#include "attack_action.h"
#include "cocos2d.h"
#include <actors/actor.h>
#include <actors/character.h>

namespace butcher {

AttackAction::AttackAction(Target target)
  : _target(target)
{
}

bool AttackAction::perform(Actor *performer)
{
  if ( !performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return false;
  }

  Character* a = dynamic_cast<Character*>(performer);

  if ( !a )
  {
    cocos2d::log("%s: performer is not a character.", __PRETTY_FUNCTION__);
    return false;
  }

  Character* t = dynamic_cast<Character*>(_target.first());

  if ( !t )
  {
    cocos2d::log("%s: target is null.", __PRETTY_FUNCTION__);
    return false;
  }

  int dice_size = (a->attribute(AttributeType::Attack) + t->attribute(AttributeType::Defense)) / 2;

  int attacker_score = ( a->attribute(AttributeType::Attack)  + cocos2d::RandomHelper::random_int(0, dice_size) );
  int target_score =   ( t->attribute(AttributeType::Defense) + cocos2d::RandomHelper::random_int(0, dice_size) );

  if ( attacker_score > target_score )
    t->takeDamage( a->attribute(AttributeType::Damage), performer );
  else
    cocos2d::log("%s: %s misses %s", __PRETTY_FUNCTION__, a->name().c_str(), t->name().c_str());

  return true;
}

}
