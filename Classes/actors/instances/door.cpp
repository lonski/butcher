#include "door.h"
#include <butcher.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

Door::Door(const ActorData *data)
  : Object(data)
  , _closed(true)
{
  if (data)
  {
    _closed = data->is_closed();
    //_openedSpriteImage = data->opened_sprite_file()->c_str();
    _closedSpriteImage = data->sprite_file()->c_str();
  }
}

std::unique_ptr<Actor> Door::clone(std::unique_ptr<Actor> allocated)
{
  Door* o = dynamic_cast<Door*>(allocated.release());

  if ( o == nullptr )
    o = new Door(nullptr);

  o->_closed = _closed;
  o->_openedSpriteImage = _openedSpriteImage;
  o->_closedSpriteImage = _closedSpriteImage;

  return std::move(Object::clone(std::unique_ptr<Actor>{o}));
}

bool Door::open()
{
  if ( _closed )
  {
    setTransparent(true);
    setBlocks(false);
    setSpriteTexture(_openedSpriteImage);
    return true;
  }
  return false;
}

bool Door::close()
{
  if ( !_closed )
  {
    setTransparent(false);
    setBlocks(true);
    setSpriteTexture(_closedSpriteImage);
    return true;
  }
  return false;
}

}
