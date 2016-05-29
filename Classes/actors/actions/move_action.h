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
  MoveAction(Direction direction);
  virtual bool perform(Actor* actor);

private:
  DungeonState* _state;
  Direction _direction;

  bool validatePosition(cocos2d::Vec2 pos);
};

}

#endif // MOVEACTION_H
