#include "target.h"
#include <actors/actor.h>
#include "cocos2d.h"

namespace butcher {

Target::Target()
  : pos(cocos2d::Vec2::ZERO)
{
}

Target::Target(std::shared_ptr<Actor> actor)
  : pos(cocos2d::Vec2::ZERO)
{
  if ( actor )
  {
    actors.push_back(actor);
    pos = actor->getTileCoord();
  }
}

Target::Target(cocos2d::Vec2 coord)
  : pos(coord)
{
}

std::shared_ptr<Actor> Target::first() const
{
  if ( actors.empty() )
    return nullptr;

  return actors.front();
}

bool Target::isEmpty() const
{
  return actors.empty() && pos.isZero();
}

}
