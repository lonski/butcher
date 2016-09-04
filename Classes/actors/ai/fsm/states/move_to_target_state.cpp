#include "move_to_target_state.h"
#include <utils/target.h>
#include <actors/ai/ai.h>
#include <actors/actor.h>
#include <dungeon/dungeon_state.h>
#include <butcher.h>
#include <actors/actions/move_action.h>
#include <utils/directions.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

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
    DungeonState* dungeon = BUTCHER.getCurrentDungeon();

    cocos2d::Vec2 myPos = me->getTileCoord();

    IPath* path = &_aPath;

    bool calculated = _aPath.calculate(myPos, target.pos, [target, dungeon](cocos2d::Vec2 pos){
                        return dungeon->isBlocked(pos) && pos != target.pos;
                      });

    if ( !calculated )
    {
      _dPath.calculate(myPos, target.pos, [target, dungeon](cocos2d::Vec2 pos){
        return dungeon->isBlocked(pos) && pos != target.pos;
      }, false);
      path = &_dPath;
    }

    path->walk();
    Direction::Symbol d = Direction::fromPosition(path->walk() - myPos);
    me->performAction( new MoveAction(d) );

  }
}

bool MoveToTarget::canEnter()
{
  return true;
}

}}
