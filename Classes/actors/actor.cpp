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
#include <actors/instances/door.h>

namespace cc = cocos2d;

namespace butcher {

Actor::Actor(const ActorData* data)
    : _id(0)
    , _blocks(false)
    , _transparent(false)
{
  if ( data )
  {
    _id = data->id();
    setBlocks( data->blocks() );
    setTransparent( data->transparent() );
    setName( data->name()->c_str() );
    setSprite(new cc::Sprite());
    if ( data->sprite_file() )
      getSprite()->initWithFile( data->sprite_file()->c_str() );
  }
}

Actor::~Actor()
{
}

std::unique_ptr<Actor> Actor::create(const ActorData *data)
{
  if ( data == nullptr )
  {
    cc::log("Actor::create: ActorData is null!");
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
    case ActorType_Door:
      actor.reset(new Door(data));
      break;
    default:
      cc::log("Actor::create: incorrect actor type (%d)!", data->type());
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
    allocated->_effects = _effects;

    if ( allocated->getSprite() == nullptr )
      allocated->setSprite(new cc::Sprite());

    allocated->getSprite()->initWithFile( getSprite()->getResourceName() );
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

std::unique_ptr<cc::Sprite>& Actor::getSprite()
{
  return _sprite;
}

void Actor::setSpriteTexture(const std::string &fn)
{
  _sprite->setTexture(fn);
}

bool Actor::performAction(std::shared_ptr<ActorAction> action)
{
  if ( isOutOfControl() )
    return false;

  if ( !action )
    return false;

  return action->perform( shared_from_this() );
}

bool Actor::performAction(ActorAction *action)
{
  if ( isOutOfControl() )
    return false;

  if ( !action )
    return false;

  std::unique_ptr<ActorAction> a_ptr(action);
  return action->perform( shared_from_this() );
}

bool Actor::isOutOfControl()
{
  return false;
}

void Actor::onCollide(std::shared_ptr<Actor>)
{
}

void Actor::onInterract(std::shared_ptr<Actor>)
{
}

cc::Vec2 Actor::getTileCoord()
{
  if ( BUTCHER.getCurrentDungeon() == nullptr )
    return cc::Vec2::ZERO;

  return positionToTileCoord(BUTCHER.getCurrentDungeon()->map(), getPosition());
}

void Actor::setTileCoord(cc::Vec2 coord)
{
  if ( BUTCHER.getCurrentDungeon() == nullptr )
    return;

  setPosition( tileCoordToPosition(BUTCHER.getCurrentDungeon()->map(),coord) );
}

void Actor::setPosition(int x, int y, bool no_sprite_pos)
{
  setPosition(cc::Vec2(x,y), no_sprite_pos);
}

void Actor::setPosition(cc::Vec2 pos, bool no_sprite_pos)
{

  _position = pos;
  if ( !no_sprite_pos )
    _sprite->setPosition(pos);
}

cc::Vec2 Actor::getPosition() const
{
  return _position;
}

void Actor::onNextTurn()
{
  std::vector<EffectID> toRemove;

  for ( auto& e : _effects )
    if ( e.second.tick() )
      toRemove.push_back(e.second.getID());

  for ( EffectID id : toRemove )
    removeEffect(id);

  if ( !toRemove.empty() )
    notify(EventType::Modified);
}

void Actor::onDestroy(std::shared_ptr<Actor>)
{
}

void Actor::onKill(std::shared_ptr<Character>)
{
}

void Actor::onHit(std::shared_ptr<Character>)
{
}

void Actor::fadeText(const std::string &text, cc::Color4B color, float speed, bool up)
{
  if ( !getSprite() )
  {
    //cc::log("Actor::fadeText Sprite is null!");
    return;
  }

  cc::Label* label = make_label(text, color, 22, cc::Vec2(0.5, 0.5));

  cc::Size size = getSprite()->getBoundingBox().size;
  label->setPosition( size.width / 2, size.height );
  label->setGlobalZOrder( getSprite()->getGlobalZOrder() + 3 );
  getSprite()->addChild(label, 1);

  if ( up )
    label->runAction( cc::MoveBy::create(speed, cc::Vec2(0, size.height / 3)) );
  else
    label->runAction( cc::MoveBy::create(speed, cc::Vec2(0, - (size.height / 3))) );

  label->runAction( cc::FadeOut::create(speed) );
}

void Actor::setSprite(cc::Sprite *sprite)
{
  _sprite.reset(sprite);
}

void Actor::addEffect(const Effect& effect)
{
  _effects[ effect.getID() ] = effect;
}

void Actor::removeEffect(EffectID id)
{
  auto it = _effects.find(id);

  if ( it != _effects.end() )
  {
    it->second.onRemove();
    _effects.erase(it);
  }
}

void Actor::removeAllEffects()
{
  _effects.clear();
}

std::vector<Effect> Actor::getEffects()
{
  std::vector<Effect> effects;

  for ( auto& kv : _effects )
    effects.push_back(kv.second);

  return effects;
}

}
