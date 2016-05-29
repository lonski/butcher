#include "dungeon_state.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_layer.h>
#include <utils/utils.h>
#include <algorithm>

using namespace cocos2d;

namespace butcher {

DungeonState::DungeonState()
  : _map(nullptr)
  , _tiles(nullptr)
  , _meta(nullptr)
  , _currentView(nullptr)
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

  setMap(map);
}

void DungeonState::spawnActors(DungeonLayer *view)
{
  _currentView = view;

  spawnPlayer();

  for ( auto a : _actors )
    view->addChild(a->sprite());
}

bool DungeonState::removeActor(Actor *actor)
{
  auto it = std::find_if(_actors.begin(), _actors.end(), [actor](std::shared_ptr<Actor> a){
     return a.get() == actor;
  });

  if ( it != _actors.end() )
  {
    _currentView->removeChild( (*it)->sprite());
    _actors.erase(it);
    return true;
  }

  return false;
}

void DungeonState::spawnPlayer()
{
  std::shared_ptr<Player> player = BUTCHER.getPlayer();

  ValueMap spawnPoint = _map->getObjectGroup("Objects")->getObject("SpawnPoint");
  player->sprite()->setPosition( spawnPoint["x"].asInt(), spawnPoint["y"].asInt() );

  _actors.insert( player );
}

void DungeonState::nextTurn()
{
  for(auto a : _actors)
    a->nextTurn();
}

bool DungeonState::isBlocked(Vec2 tileCoord, Actor** blocking_actor)
{
  bool blocked = false;

  //Check if tile is enterable
  unsigned gid = _meta->tileGIDAt(tileCoord);
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
      Vec2 tc = positionToTileCoord(_map, a->sprite()->getPosition());
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

void DungeonState::spawn(int id, int x, int y)
{
  Actor* actor = BUTCHER.actorsDatabase().createActor<Actor>(id);
  if ( actor )
  {
    actor->sprite()->setPosition(x,y);
    _actors.insert( std::shared_ptr<Actor>(actor) );
  }
  else
  {
    log("DungeonState::spawn: Failed to create actor id=%d.", id);
  }
}

}
