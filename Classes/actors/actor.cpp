#include "actor.h"
#include <data/actors_generated.h>
#include <actors/player.h>
#include <actors/monster.h>
#include <actors/object.h>
#include <actors/item.h>
#include "cocos2d.h"
#include <actors/actions/actor_action.h>
#include <utils/utils.h>
#include <butcher.h>
#include <dungeon/dungeon_state.h>
#include <actors/instances/stairs_down.h>
#include <actors/instances/stairs_up.h>

namespace butcher {

Actor::Actor(const ActorData* data)
    : _id(0)
    , _blocks(false)
    , _transparent(false)
    , _sprite(nullptr)
{
  if ( data )
  {
    _id = data->id();
    setBlocks( data->blocks() );
    setTransparent( data->transparent() );
    setName( data->name()->c_str() );
    _sprite.reset(new cocos2d::Sprite());
    _sprite->initWithFile( data->sprite_file()->c_str() );
  }
}

Actor::~Actor()
{
}

std::unique_ptr<Actor> Actor::create(const ActorData *data)
{
  if ( data == nullptr )
  {
    cocos2d::log("Actor::create: ActorData is null!");
  }

  std::unique_ptr<Actor> actor;

  switch(data->type())
  {
    case ActorType_Object:
      actor.reset(new Object(data));
      break;
    case ActorType_Monster:
      actor.reset(new Monster(data));
      break;
    case ActorType_Player:
      actor.reset(new Player(data));
      break;
    case ActorType_StairsDown:
      actor.reset(new StairsDown(data));
      break;
    case ActorType_StairsUp:
      actor.reset(new StairsUp(data));
      break;
    case ActorType_Item:
      actor.reset(new Item(data));
      break;
    default:
      cocos2d::log("Actor::create: incorrect actor type (%d)!", data->type());
    break;
  }

  return actor;
}

std::unique_ptr<Actor> Actor::clone(std::unique_ptr<Actor> allocated)
{
  if ( allocated )
  {
    allocated->setBlocks( isBlocking() );
    allocated->setTransparent( isTransparent() );
    allocated->setName(getName());
    allocated->_id = _id;

    if ( allocated->_sprite == nullptr )
      allocated->_sprite.reset(new cocos2d::Sprite());

    allocated->_sprite->initWithFile( _sprite->getResourceName() );
  }

  return std::move(allocated);
}

std::string Actor::getName() const
{
    return _name;
}

void Actor::setName(const std::string &name)
{
    _name = name;
}

bool Actor::isBlocking() const
{
    return _blocks;
}

void Actor::setBlocks(bool blocks)
{
    _blocks = blocks;
}

bool Actor::isTransparent() const
{
    return _transparent;
}

void Actor::setTransparent(bool transparent)
{
  _transparent = transparent;
}

ActorID Actor::getID() const
{
  return static_cast<ActorID>(_id);
}

std::unique_ptr<cocos2d::Sprite>& Actor::getSprite()
{
  return _sprite;
}

bool Actor::performAction(const ActorAction& action)
{
  return action.perform( shared_from_this() );
}

void Actor::onCollide(std::shared_ptr<Actor>)
{
}

void Actor::onInterract(std::shared_ptr<Actor>)
{
  cocos2d::log("%s Not implemented.", __PRETTY_FUNCTION__);
}

cocos2d::Vec2 Actor::getTileCoord()
{
  if ( BUTCHER.getCurrentDungeon() == nullptr )
    return cocos2d::Vec2::ZERO;

  return positionToTileCoord(BUTCHER.getCurrentDungeon()->map(), getPosition());
}

void Actor::setTileCoord(cocos2d::Vec2 coord)
{
  if ( BUTCHER.getCurrentDungeon() == nullptr )
    return;

  setPosition( tileCoordToPosition(BUTCHER.getCurrentDungeon()->map(),coord) );
}

void Actor::setPosition(int x, int y, bool no_sprite_pos)
{
  setPosition(cocos2d::Vec2(x,y), no_sprite_pos);
}

void Actor::setPosition(cocos2d::Vec2 pos, bool no_sprite_pos)
{

  _position = pos;
  if ( !no_sprite_pos )
    _sprite->setPosition(pos);
}

cocos2d::Vec2 Actor::getPosition() const
{
  return _position;
}

void Actor::nextTurn()
{
}

void Actor::onDestroy(std::shared_ptr<Actor>)
{
}

void Actor::onKill(std::shared_ptr<Character>)
{
}

void Actor::fadeText(const std::string &text, cocos2d::Color4B color)
{
  if ( !getSprite() )
  {
    //cocos2d::log("Actor::fadeText Sprite is null!");
    return;
  }

  cocos2d::Label* label = make_label(text, color, 22);

  cocos2d::Size size = getSprite()->getBoundingBox().size;
  label->setPosition( size.width / 2, size.height );

  getSprite()->addChild(label, 1);

  label->runAction( cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, size.height / 3)) );
  label->runAction( cocos2d::FadeOut::create(0.5) );
}

}
