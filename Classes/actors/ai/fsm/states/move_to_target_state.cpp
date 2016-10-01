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
  : _path(nullptr)
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
    if (_ai->getTarget().pos == cc::Vec2::ZERO)
      return;

//    cc::Vec2 myPos = _ai->getActor()->getTileCoord();

//    cc::Vec2 dd = _ai->getTarget().pos - myPos;
//    int stepdx = (dd.x > 0 ? 1:-1);
//    int stepdy = (dd.y > 0 ? 1:-1);

//    DungeonState* dungeon = BUTCHER.getCurrentDungeon();

//    float distance=sqrtf(dd.x*dd.x+dd.y*dd.y);
//    if ( distance >= 2 )
//    {
//        dd.x = (int)(round(dd.x/distance));
//        dd.y = (int)(round(dd.y/distance));

//        cc::log("MoveToTarget: %d %d", (int)dd.x, (int)dd.y);

//        if ( !dungeon->isBlocked(myPos + dd) )
//        {
//          _ai->getActor()->performAction( new MoveAction(dd) );
//        }
//        else if ( !dungeon->isBlocked(cc::Vec2(myPos.x + stepdx, myPos.y)) )
//        {
//          _ai->getActor()->performAction( new MoveAction(cc::Vec2(stepdx, 0)) );
//        }
//        else if ( !dungeon->isBlocked(cc::Vec2(myPos.x,myPos.y+stepdy)) )
//        {
//            _ai->getActor()->performAction( new MoveAction(cc::Vec2(0, stepdy)) );
//        }
//   }
    if ( _ai->getTarget().pos != _lastTarget.pos || _path->empty() )
      calculatePath();

    if ( !_path->empty() )
    {
      _path->walk();
      Direction::Symbol d = Direction::fromPosition(_path->walk() - _ai->getActor()->getTileCoord());
      _ai->getActor()->performAction( new MoveAction(d) );
    }
  }
}

bool MoveToTarget::canEnter()
{
  return true;
}

void MoveToTarget::onExit()
{
  _lastTarget = Target();
}

void MoveToTarget::calculatePath()
{
  cc::Vec2 myPos = _ai->getActor()->getTileCoord();
  Target target = _ai->getTarget();

  _path = &_aPath;
  DungeonState* dungeon = BUTCHER.getCurrentDungeon();

  bool calculated = _aPath.calculate(myPos, target.pos,
                    [target, dungeon](cocos2d::Vec2 pos){
                      return dungeon->isBlocked(pos) && pos != target.pos;
                    });

  if ( !calculated )
  {
    _dPath.calculate(myPos, target.pos, [target, dungeon](cocos2d::Vec2 pos){
      return dungeon->isBlocked(pos) && pos != target.pos;
    }, false);
    _path = &_dPath;
  }

  _lastTarget = target;
}

}}
