#include "move_action.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <utils/utils.h>
#include <actors/instances/stairs_up.h>

namespace cc = cocos2d;

namespace butcher {

MoveAction::MoveAction(Direction::Symbol direction)
  : _state( BUTCHER.getCurrentDungeon() )
  , _direction(direction)
  , _dd(cc::Vec2::ZERO)
{

}

MoveAction::MoveAction(cocos2d::Vec2 diff)
  : _state( BUTCHER.getCurrentDungeon() )
  , _direction(Direction::Symbol::None)
  , _dd(diff)
{
}

ActorAction::Result MoveAction::perform(std::shared_ptr<Actor> actor)
{
  if ( _direction == Direction::None && _dd == cc::Vec2::ZERO )
    return ActorAction::Result::NOK;

  cc::Vec2 pos = actor->getTileCoord();
  cc::TMXTiledMap* map = _state->map();

  if ( _direction != Direction::Symbol::None )
  {
    switch(_direction)
    {
      case Direction::East: pos.x += 1; break;
      case Direction::West: pos.x -= 1; break;
      case Direction::North: pos.y -= 1; break;
      case Direction::South: pos.y += 1; break;
      case Direction::SouthWest: {
        pos.y += 1;
        pos.x -= 1;
      }break;
      case Direction::SouthEast: {
        pos.y += 1;
        pos.x += 1;
      }break;
      case Direction::NorthEast: {
        pos.y -= 1;
        pos.x += 1;
      }break;
      case Direction::NorthWest: {
        pos.y -= 1;
        pos.x -= 1;
      }break;
      default:;
    }
  }
  else
  {
    pos += _dd;
  }

  auto pixelPos = tileCoordToPosition(map, pos);
  if ( !validatePosition(pixelPos) )
  {
    cc::log("MoveAction: invalid position!");
    return ActorAction::Result::NOK;
  }

  std::shared_ptr<Actor> blocking_actor;
  if ( _state->isBlocked(pos, &blocking_actor) )
  {
    if ( blocking_actor != nullptr )
      actor->onCollide(blocking_actor);

    return ActorAction::Result::NOK;
  }

  actor->setPosition(pixelPos, !BUTCHER.isAnimationSuspended());

  if ( !BUTCHER.isAnimationSuspended() )
  {
    cc::MoveTo* action = cc::MoveTo::create(0.1, pixelPos);
    BUTCHER.pushOngoingAction(action);

    actor->getSprite()->runAction(cc::Sequence::create(action , cc::CallFunc::create([action](){
                                      BUTCHER.removeOngoingAction(action);
                                    }), nullptr ));

  }

  for ( auto a : _state->getActorsAt(actor->getTileCoord()) )
    if ( a != actor )
      a->onInterract(actor);

  return ActorAction::Result::OK;
}

bool MoveAction::validatePosition(cc::Vec2 pos) const
{
  cc::TMXTiledMap* map = _state->map();

  if (pos.x <= (map->getMapSize().width * map->getTileSize().width) &&
      pos.y <= (map->getMapSize().height * map->getTileSize().height) &&
      pos.y >= 0 &&
      pos.x >= 0 )
  {
    return true;
  }

  return false;
}

}


