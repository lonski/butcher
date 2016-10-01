#ifndef MOVEACTION_H
#define MOVEACTION_H

#include <actors/actions/actor_action.h>
#include <utils/directions.h>
#include "cocos2d.h"

namespace butcher {

class DungeonState;

class MoveAction : public ActorAction
{
public:
  MoveAction(Direction::Symbol direction);
  MoveAction(cocos2d::Vec2 diff);
  virtual bool perform(std::shared_ptr<Actor> actor);

private:
  DungeonState* _state;
  Direction::Symbol _direction;
  cocos2d::Vec2 _dd;

  bool validatePosition(cocos2d::Vec2 pos) const;
};

}

#endif // MOVEACTION_H
