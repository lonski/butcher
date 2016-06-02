#include "dungeon_state.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_layer.h>
#include <utils/utils.h>
#include <algorithm>
#include <fov/permissive-fov-cpp.h>

using namespace cocos2d;

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

  TMXObjectGroup* objectGroup = _map->getObjectGroup("Objects");
  if(objectGroup == nullptr)
  {
      log("%s: Tile map has no objects object layer.", __FUNCTION__);
      return false;
  }

  _tiles = _map->getLayer("Background");

  if ( _tiles == nullptr )
  {
    log("%s: Tile map has no layer 'Background'.", __FUNCTION__);
    return false;
  }

  _meta = _map->getLayer("Meta");

  if ( _meta == nullptr )
  {
    log("%s: Tile map has no layer 'Meta'.", __FUNCTION__);
    return false;
  }

  _meta->setVisible(false);

  for ( int y = 0; y < _map->getMapSize().height; ++y)
  {
    for ( int x = 0; x < _map->getMapSize().width; ++x)
    {
      //_fov->setTileGID( (int)GID::Unexplored, Vec2(x,y));
      Sprite* s = _tiles->getTileAt(Vec2(x,y));
      if ( s )
        s->setVisible(false);
    }
  }

  //Load actor spawns
  ValueVector objects = objectGroup->getObjects();
  for ( Value o : objects )
  {
    ValueMap obj = o.asValueMap();
    if ( obj["name"].asString() == "mob_spawn" && obj.find("mob") != obj.end())
    {
      spawn(obj["mob"].asInt(), obj["x"].asInt(), obj["y"].asInt());
    }
  }

  //init explored map
  int cnt = _map->getMapSize().height * _map->getMapSize().width;
  _exploredMask = std::string(cnt, '0' );

  return true;
}

bool DungeonState::setMap(const std::string& fn)
{
  if ( !FileUtils::getInstance()->isFileExist(fn) )
  {
    log("DungeonState::setMap: file does not exists - %s", fn.c_str());
    return false;
  }

  TMXTiledMap* map = new TMXTiledMap();
  map->initWithTMXFile(fn);

  return setMap(map);
}

void DungeonState::spawnActors(DungeonLayer *view)
{
  _currentView = view;

  spawnPlayer();

  for ( auto a : _actors )
    view->addChild(a->sprite().get());

  computeFov(BUTCHER.getPlayer()->getTileCoord().x,
             BUTCHER.getPlayer()->getTileCoord().y);

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

  ValueMap spawnPoint = _map->getObjectGroup("Objects")->getObject("SpawnPoint");
  player->setPosition( spawnPoint["x"].asInt(), spawnPoint["y"].asInt() );

  _actors.insert( player );
}

void DungeonState::nextTurn()
{
  computeFov(BUTCHER.getPlayer()->getTileCoord().x,
             BUTCHER.getPlayer()->getTileCoord().y);

  for(auto a : _actors)
    a->nextTurn();

  for(auto a : _actors)
    a->sprite()->setVisible( isInFov(a->getTileCoord()) );

}

bool DungeonState::isBlocked(Vec2 tileCoord, Actor** blocking_actor)
{
  bool blocked = false;

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
      Vec2 tc = positionToTileCoord(_map, a->getPosition());
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

bool DungeonState::isOpaque(Vec2 tileCoord)
{
  Actor* blocking_actor = nullptr;
  bool blocked = isBlocked(tileCoord, &blocking_actor);

  return blocking_actor ? !blocking_actor->transparent() : blocked;
}

bool DungeonState::isOpaque(int x, int y)
{
  return isOpaque(Vec2(x,y));
}

void DungeonState::visit(int x, int y)
{
  Vec2 coord(x,y);

  size_t loc = y*_map->getMapSize().width + x;
  if ( loc < _exploredMask.size() )
  {
    //Mark as visited
    _exploredMask[loc] = '1';

    //Light tile
    Sprite* s = _tiles->getTileAt(coord);
    if (s)
    {
      if ( !s->isVisible() )
      {
        s->setVisible(true);
        s->setOpacity(0);
        s->runAction( FadeIn::create(0.2));
      }

      s->setOpacity(255);
    }
  }
}

bool DungeonState::isInFov(cocos2d::Vec2 tileCoord)
{
  Sprite* s = _tiles->getTileAt(tileCoord);
  return s && s->isVisible() && s->getOpacity() == 255;
}

void DungeonState::spawn(int id, int x, int y)
{
  Actor* actor = BUTCHER.actorsDatabase().createActor<Actor>(id);
  if ( actor )
  {
    actor->setPosition(x,y);
    _actors.insert( std::shared_ptr<Actor>(actor) );
  }
  else
  {
    log("DungeonState::spawn: Failed to create actor id=%d.", id);
  }
}

void DungeonState::computeFov(int x, int y)
{
  //draw FoG
  for ( int y = 0; y < _map->getMapSize().height; ++y)
  {
    for ( int x = 0; x < _map->getMapSize().width; ++x)
    {
      Vec2 coord(x,y);
      if ( _exploredMask[ y*_map->getMapSize().width + x ] == '1' )
      {
        Sprite* s = _tiles->getTileAt(coord);
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
