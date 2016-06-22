#include "move_to_target_state.h"
#include <utils/target.h>
#include <actors/ai/ai.h>
#include <actors/actor.h>
#include <dungeon/dungeon_state.h>
#include <butcher.h>
#include <actors/actions/move_action.h>
#include <utils/directions.h>

namespace butcher {

namespace state{

MoveToTarget::MoveToTarget()
{
}

FSMStateType MoveToTarget::getType() const
{
  return FSMStateType::MOVE_TO_TARGET;
}

void MoveToTarget::update()
{
  if ( validatePreconditions() )
  {
    Target target = _ai->getTarget();
    std::shared_ptr<Actor> me = _ai->getActor();
    cocos2d::Vec2 myPos = me->getTileCoord();
    DungeonState* dungeon = BUTCHER.getCurrentDungeon();

    int dx = target.x - myPos.x;
    int dy = target.y - myPos.y;
    int stepDx = (dx > 0 ? 1:-1);
    if ( dx == 0 ) stepDx = 0;
    int stepDy = (dy > 0 ? 1:-1);
    if ( dy == 0 ) stepDy = 0;

    float distance = sqrtf(dx*dx + dy*dy);

    if ( distance > 0 )
    {
      if ( stepDx != 0 && !dungeon->isBlocked( cocos2d::Vec2(myPos.x+stepDx, myPos.y) ) )
      {
        me->performAction( MoveAction(stepDx > 0 ? Direction::East : Direction::West) );
      }
      else if ( stepDy != 0 && !dungeon->isBlocked( cocos2d::Vec2(myPos.x, myPos.y + stepDy) ) )
      {
        me->performAction( MoveAction(stepDy > 0 ? Direction::North : Direction::South) );
      }

    }

  }

}

bool MoveToTarget::canEnter()
{
  return true;
}

}}
