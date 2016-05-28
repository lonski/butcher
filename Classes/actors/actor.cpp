#include "actor.h"
#include <actors_generated.h>
#include <actors/player.h>
#include <actors/monster.h>
#include "cocos2d.h"
#include <actors/actions/actor_action.h>

using namespace cocos2d;

namespace butcher {

Actor::Actor()
    : _id(0)
    , _blocks(false)
    , _transparent(false)
    , _sprite(nullptr)
{
}

Actor::~Actor()
{
}

Actor* Actor::create(const ActorData *data)
{
  if ( data == nullptr )
  {
    log("Actor::create: ActorData is null!");
  }

  Actor* actor = nullptr;

  switch(data->type())
  {
    case ActorType_Monster:
      actor = Monster::create(data);
      break;
    case ActorType_Player:
      actor = Player::create(data);
      break;
    default:
      log("Actor::create: incorrect actor type (%d)!", data->type());
    break;
  }

  if ( actor != nullptr )
  {
    actor->_id = data->id();
    actor->setBlocks( data->blocks() );
    actor->setTransparent( data->transparent() );
    actor->setName( data->name()->c_str() );
    actor->_sprite = new Sprite();
    actor->_sprite->initWithFile( data->sprite_file()->c_str() );
  }

  return actor;
}

Actor *Actor::clone(Actor *allocated)
{
  if ( allocated )
  {
    allocated->setBlocks( blocks() );
    allocated->setTransparent( transparent() );
    allocated->setName(name());
    allocated->_id = _id;

    if ( allocated->_sprite == nullptr )
      allocated->_sprite = new Sprite();

    allocated->_sprite->initWithFile( _sprite->getResourceName() );
  }

  return allocated;
}

std::string Actor::name() const
{
    return _name;
}

void Actor::setName(const std::string &name)
{
    _name = name;
}

bool Actor::blocks() const
{
    return _blocks;
}

void Actor::setBlocks(bool blocks)
{
    _blocks = blocks;
}

bool Actor::transparent() const
{
    return _transparent;
}

void Actor::setTransparent(bool transparent)
{
  _transparent = transparent;
}

unsigned Actor::id() const
{
  return _id;
}

cocos2d::Sprite *Actor::sprite() const
{
  return _sprite;
}

bool Actor::performAction(ActorAction* action)
{
  std::unique_ptr<ActorAction> a(action);
  return a->perform(this);
}

void Actor::nextTurn()
{
}

}
