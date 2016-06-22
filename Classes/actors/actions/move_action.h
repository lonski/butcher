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
  virtual bool perform(std::shared_ptr<Actor> actor) const;

private:
  DungeonState* _state;
  Direction::Symbol _direction;

  bool validatePosition(cocos2d::Vec2 pos) const;
};

}

#endif // MOVEACTION_H
