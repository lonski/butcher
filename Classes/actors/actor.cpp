#include "actor.h"
#include <actors_generated.h>
#include <actors/player.h>
#include <actors/monster.h>
#include "cocos2d.h"
#include <actors/actions/actor_action.h>
#include <utils/utils.h>
#include <butcher.h>
#include <dungeon/dungeon_state.h>

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
    actor->_sprite.reset(new Sprite());
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
      allocated->_sprite.reset(new Sprite());

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

std::unique_ptr<Sprite>& Actor::sprite()
{
  return _sprite;
}

bool Actor::performAction(ActorAction* action)
{
  std::unique_ptr<ActorAction> a(action);
  return a->perform(this);
}

bool Actor::collide(Actor* obstacle)
{
  return true;
}

Vec2 Actor::getTileCoord()
{
  if ( BUTCHER.currentDungeon() == nullptr )
    return 0;

  return positionToTileCoord(BUTCHER.currentDungeon()->map(), getPosition());
}

void Actor::setPosition(int x, int y, bool no_sprite_pos)
{
  setPosition(Vec2(x,y), no_sprite_pos);
}

void Actor::setPosition(Vec2 pos, bool no_sprite_pos)
{
  _position = pos;
  if ( !no_sprite_pos )
    _sprite->setPosition(pos);
}

Vec2 Actor::getPosition() const
{
  return _position;
}

void Actor::nextTurn()
{
}

void Actor::onDie()
{
}

void Actor::fadeText(const std::string &text, Color4B color)
{
  Label* label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 18);
  label->setTextColor(color);

  Size size = sprite()->getBoundingBox().size;
  label->setPosition( size.width / 2, size.height );

  sprite()->addChild(label, 1);

  label->runAction( MoveBy::create(0.5, Vec2(0, size.height / 3)) );
  label->runAction( FadeOut::create(0.5) );
}

}
