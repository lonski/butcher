#include "move_action.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

MoveAction::MoveAction(Direction direction)
  : _state( BUTCHER.getCurrentDungeon() )
  , _direction(direction)
{

}

bool MoveAction::perform(Actor *actor)
{
  if ( _direction == Direction::None )
    return false;

  cc::Vec2 pos = actor->getPosition();
  cc::TMXTiledMap* map = _state->map();

  switch(_direction)
  {
    case Direction::East: pos.x += map->getTileSize().width; break;
    case Direction::West: pos.x -= map->getTileSize().width; break;
    case Direction::North: pos.y -= map->getTileSize().height; break;
    case Direction::South: pos.y += map->getTileSize().height; break;
    case Direction::SouthWest: {
      pos.y += map->getTileSize().height;
      pos.x -= map->getTileSize().width;
    }break;
    case Direction::SouthEast: {
      pos.y += map->getTileSize().height;
      pos.x += map->getTileSize().width;
    }break;
    case Direction::NorthEast: {
      pos.y -= map->getTileSize().height;
      pos.x += map->getTileSize().width;
    }break;
    case Direction::NorthWest: {
      pos.y -= map->getTileSize().height;
      pos.x -= map->getTileSize().width;
    }break;
    default:;
  }


  if ( !validatePosition(pos) )
  {
    cc::log("MoveAction: invalid position!");
    return false;
  }

  Actor* blocking_actor = nullptr;
  if ( _state->isBlocked(positionToTileCoord(map,pos), &blocking_actor) )
  {
    if ( blocking_actor != nullptr )
      actor->collide(blocking_actor);

    return false;
  }

  actor->setPosition(pos, true);

  cc::MoveTo* move_action = cc::MoveTo::create(0.1, pos);
  actor->sprite()->runAction( move_action );

  return true;
}

bool MoveAction::validatePosition(cc::Vec2 pos)
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


