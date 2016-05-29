#include "target.h"
#include <actors/actor.h>
#include "cocos2d.h"

namespace butcher {

Target::Target()
  : x(0)
  , y(0)
{
}

Target::Target(Actor *actor)
  : x(0)
  , y(0)
{
  if ( actor )
  {
    actors.push_back(actor);

    cocos2d::Vec2 pos = actor->getTileCoord();
    x = pos.x;
    y = pos.y;
  }
}

Actor *Target::first()
{
  if ( actors.empty() )
    return nullptr;

  return actors.front();
}

}
