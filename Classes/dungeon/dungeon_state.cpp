#include "dungeon_state.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_layer.h>
#include <utils/utils.h>
#include <algorithm>
#include <fov/permissive-fov-cpp.h>
#include <utils/profiler.h>
#include <actors/object.h>

namespace cc = cocos2d;

namespace butcher {

DungeonState::DungeonState()
  : _map(nullptr)
  , _tiles(nullptr)
  , _meta(nullptr)
  , _currentView(nullptr)
  , _fovMask("circle.mask")
{
}

cocos2d::TMXTiledMap *DungeonState::map() const
{
    return _map;
}

bool DungeonState::setMap(cocos2d::TMXTiledMap *map)
{
  _map = map;

  cc::TMXObjectGroup* objectGroup = _map->getObjectGroup("Objects");
  if(objectGroup == nullptr)
  {
      cc::log("%s: Tile map has no objects object layer.", __FUNCTION__);
      return false;
  }

  _tiles = _map->getLayer("Background");

  if ( _tiles == nullptr )
  {
    cc::log("%s: Tile map has no layer 'Background'.", __FUNCTION__);
    return false;
  }

  _meta = _map->getLayer("Meta");

  if ( _meta == nullptr )
  {
    cc::log("%s: Tile map has no layer 'Meta'.", __FUNCTION__);
    return false;
  }

  _meta->setVisible(false);

  for ( int y = 0; y < _map->getMapSize().height; ++y)
  {
    for ( int x = 0; x < _map->getMapSize().width; ++x)
    {
      cc::Sprite* s = _tiles->getTileAt(cc::Vec2(x,y));
      if ( s )
        s->setVisible(false);
    }
  }

  //Load actor spawns
  cc::ValueVector objects = objectGroup->getObjects();
  for ( cc::Value o : objects )
  {
    cc::ValueMap obj = o.asValueMap();
    if ( obj["name"].asString() == "actor_spawn" && obj.find("id") != obj.end())
    {
      spawn(obj["id"].asInt(), cc::Vec2(obj["x"].asInt(), obj["y"].asInt()));
    }
  }

  spawnPlayer();

  //init explored map
  _exploredMask = Grid(_map->getMapSize().width, _map->getMapSize().height, '0');

  return true;
}

bool DungeonState::setMap(const std::string& fn)
{
  if ( !cc::FileUtils::getInstance()->isFileExist(fn) )
  {
    cc::log("DungeonState::setMap: file does not exists - %s", fn.c_str());
    return false;
  }

  cc::TMXTiledMap* map = new cc::TMXTiledMap();
  map->initWithTMXFile(fn);

  return setMap(map);
}

void DungeonState::spawnActors(DungeonLayer *view)
{
  _currentView = view;

  computeFov(BUTCHER.getPlayer()->getTileCoord().x,
             BUTCHER.getPlayer()->getTileCoord().y);

  for ( auto a : _actors )
  {
    view->addChild(a->sprite().get(), a->getZ());

    a->sprite()->setVisible( isInFov(a->getTileCoord()));
  }

}

bool DungeonState::removeActor(Actor *actor, bool remove_node_child)
{
  auto it = std::find_if(_actors.begin(), _actors.end(), [actor](std::shared_ptr<Actor> a){
     return a.get() == actor;
  });

  if ( it != _actors.end() )
  {
    if ( remove_node_child )
      _currentView->removeChild( (*it)->sprite().get() );

    _actors.erase(it);
    return true;
  }

  return false;
}

void DungeonState::spawnPlayer()
{
  std::shared_ptr<Player> player = BUTCHER.getPlayer();

  cc::ValueMap spawnPoint = _map->getObjectGroup("Objects")->getObject("SpawnPoint");
  cc::Vec2 pos =  tileCoordToPosition(_map, cc::Vec2(spawnPoint["x"].asInt(), spawnPoint["y"].asInt()));
  player->setPosition(pos);

  _actors.insert( player );
}

void DungeonState::spawn(int id, cc::Vec2 coord)
{
  Actor* actor = BUTCHER.actorsDatabase().createActor<Actor>(id);
  if ( actor )
  {
    cc::Vec2 pos = tileCoordToPosition(_map, coord);
    actor->setPosition(pos);
    _actors.insert( std::shared_ptr<Actor>(actor) );
  }
  else
  {
    cc::log("DungeonState::spawn: Failed to create actor id=%d.", id);
  }
}

void DungeonState::nextTurn()
{
  computeFov(BUTCHER.getPlayer()->getTileCoord().x,
             BUTCHER.getPlayer()->getTileCoord().y);

  for(auto a : _actors)
  {
    a->nextTurn();
    a->sprite()->setVisible( isInFov(a->getTileCoord()) );
  }

}

bool DungeonState::isBlocked(cc::Vec2 tileCoord, Actor** blocking_actor)
{
  bool blocked = false;

  if ( tileCoord.x >= _map->getMapSize().width || tileCoord.y >= _map->getMapSize().height
       || tileCoord.x < 0 || tileCoord.y < 0 )
  {
    cc::log("%s out of range: x=%f y=%f", __PRETTY_FUNCTION__, tileCoord.x, tileCoord.y);
    return true;
  }

  //Check if tile is enterable
  unsigned gid = _meta->getTileGIDAt(tileCoord);
  if ( gid )
  {
    auto properties = _map->getPropertiesForGID(gid).asValueMap();
    if (!properties.empty())
    {
      auto collision = properties["Collidable"].asString();
      blocked = ("True" == collision);
    }
  }

  //Check for blocking actor
  if (!blocked)
  {
    for(std::shared_ptr<Actor> a : _actors)
    {
      cc::Vec2 tc = positionToTileCoord(_map, a->getPosition());
      if ( tc == tileCoord && a->blocks() )
      {
        blocked = true;
        if ( blocking_actor != nullptr )
          *blocking_actor = a.get();
        break;
      }
    }
  }

  return blocked;
}

bool DungeonState::isOpaque(cc::Vec2 tileCoord)
{
  Actor* blocking_actor = nullptr;
  bool blocked = isBlocked(tileCoord, &blocking_actor);

  return blocking_actor ? !blocking_actor->transparent() : blocked;
}

bool DungeonState::isOpaque(int x, int y)
{
  return isOpaque(cc::Vec2(x,y));
}

void DungeonState::visit(int x, int y)
{
  cc::Vec2 coord(x,y);
  if ( _exploredMask.get(x,y) != Tiles::FoV )
  {    
    //Mark as in FoV
    _exploredMask.set(x,y,Tiles::FoV);

    //Light tile
    cc::Sprite* s = _tiles->getTileAt(coord);
    if (s)
    {
      if ( !s->isVisible() )
        s->setVisible(true);

      s->setOpacity(255);
    }
  }
}

bool DungeonState::isInFov(cocos2d::Vec2 tileCoord)
{
  cc::Sprite* s = _tiles->getTileAt(tileCoord);
  return s && s->isVisible() && s->getOpacity() == 255;
}

void DungeonState::computeFov(int x, int y)
{
  //draw FoG
  for ( int y = 0; y < _map->getMapSize().height; ++y)
  {
    for ( int x = 0; x < _map->getMapSize().width; ++x)
    {
      cc::Vec2 coord(x,y);
      if ( _exploredMask.get(x,y) == Tiles::FoV )
      {
        //Mark as in FoG
        _exploredMask.set(x,y,Tiles::FoG);

        cc::Sprite* s = _tiles->getTileAt(coord);
        if ( s && s->isVisible() )
        {
          s->setOpacity(100);
        }
      }
    }
  }

  //draw FoV
  permissive::fov(x, y, _fovMask, *this);
}

}
