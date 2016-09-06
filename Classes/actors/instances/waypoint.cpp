#include "waypoint.h"
#include "cocos2d.h"
#include <data/actors_generated.h>
#include <actors/player.h>
#include <butcher.h>

namespace cc = cocos2d;

namespace butcher {

Waypoint::Waypoint(const ActorData *data)
  : Object(data)
  , _level(0)
{
  if (data)
  {
    _activated = !data->is_closed();
    _openedSpriteImage = data->opened_sprite_file()->c_str();
    _closedSpriteImage = data->sprite_file()->c_str();
  }
}

std::unique_ptr<Actor> Waypoint::clone(std::unique_ptr<Actor> allocated)
{
  Waypoint* o = dynamic_cast<Waypoint*>(allocated.release());
  if ( o == nullptr )
    o = new Waypoint(nullptr);

  o->_activated = _activated;
  o->_openedSpriteImage = _openedSpriteImage;
  o->_closedSpriteImage = _closedSpriteImage;

  return std::move(Object::clone(std::unique_ptr<Actor>{o}));
}

void Waypoint::onInterract(std::shared_ptr<Actor> actor)
{
  std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(actor);
  if ( player && !_activated && _level > 0 )
  {
    activate();
    player->addWaypoint(_level);
  }

  BUTCHER.showWaypoints();
}

void Waypoint::activate()
{
  setSpriteTexture(_openedSpriteImage);
  _activated = true;
}

void Waypoint::setLevel(int level)
{
  _level = level;
}

}
