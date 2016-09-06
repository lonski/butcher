#include "dungeon_state.h"
#include <butcher.h>
#include <actors/player.h>
#include <view/dungeon_layer.h>
#include <utils/utils.h>
#include <algorithm>
#include <lib/permissive-fov-cpp.h>
#include <utils/profiler.h>
#include <actors/object.h>
#include <actors/monster.h>
#include <utils/profiler.h>
#include <actors/instances/waypoint.h>

namespace cc = cocos2d;

namespace butcher {

DungeonState::DungeonState(int level)
  : _map(nullptr)
  , _tiles(nullptr)
  , _meta(nullptr)
  , _currentView(nullptr)
  , _fovMask("circle.mask")
  , _turnsOnLeave(0)
  , _level(level)
{
  _turnsOnLeave = BUTCHER.getTurnCounter();
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
      addActor( static_cast<ActorID>(obj["id"].asInt()),
                cc::Vec2(obj["x"].asInt(), obj["y"].asInt()) );
    }
  }

  addPlayer();

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

void DungeonState::setMapUsableSize(int width, int height)
{
  _usableMapSize.width = width;
  _usableMapSize.height = height;
}

cocos2d::Size DungeonState::getMapUsableSize() const
{
  return _usableMapSize;
}

void DungeonState::onEnter(DungeonLayer *view)
{
  _lastPlayerPosition = BUTCHER.getPlayer()->getTileCoord();

  BUTCHER.setAnimationSuspended(true);

  unsigned long long diff = BUTCHER.getTurnCounter() - _turnsOnLeave;

  enum {
    MAX_UPDATE_TURNS = 100
  };

  if ( diff > MAX_UPDATE_TURNS )
  {
    _turnsOnLeave = _turnsOnLeave + diff - MAX_UPDATE_TURNS;
    diff = MAX_UPDATE_TURNS;
  }

  for( int y = 0; y < _exploredMask.height; ++y )
    for( int x = 0; x < _exploredMask.width; ++x )
      if ( _exploredMask.get(x,y) == Tiles::FoV )
        _exploredMask.set(x,y, Tiles::FoG);

  for (; _turnsOnLeave < BUTCHER.getTurnCounter(); ++_turnsOnLeave )
    nextTurn();

  BUTCHER.setAnimationSuspended(false);

  _currentView = view;
  addPlayer();
  spawnActors();
}

void DungeonState::onExit()
{
  removeActor( BUTCHER.getPlayer() );  
  _turnsOnLeave = BUTCHER.getTurnCounter();
}

void DungeonState::spawnActors()
{
  computeFov(BUTCHER.getPlayer()->getTileCoord().x,
             BUTCHER.getPlayer()->getTileCoord().y);

  for ( auto a : _actors )
  {
    _currentView->addChild(a->getSprite().get(), a->getZ());

    a->getSprite()->setVisible( isInFov(a->getTileCoord()));
  }

}

bool DungeonState::removeActor(std::shared_ptr<Actor> actor, bool remove_node_child)
{
  auto it = std::find_if(_actors.begin(), _actors.end(), [actor](std::shared_ptr<Actor> a){
     return a.get() == actor.get();
  });

  if ( it != _actors.end() )
  {
    if ( remove_node_child )
      _currentView->removeChild( (*it)->getSprite().get() );

    _actors.erase(it);
    return true;
  }

  return false;
}

void DungeonState::addActor(std::shared_ptr<Actor> actor)
{
  _actors.insert(actor);
}

void DungeonState::addPlayer()
{
  std::shared_ptr<Player> player = BUTCHER.getPlayer();

  _actors.insert( player );
}

void DungeonState::addActor(ActorID id, cc::Vec2 coord)
{
  //On first level theare are no stairs. Only waypoint.
  if ( id == ActorID::STAIRS_UP && getLevel() == 1 )
    return;

  std::shared_ptr<Actor> actor{ BUTCHER.actorsDatabase().createActor<Actor>(id) };
  if ( actor )
  {
    cc::Vec2 pos = tileCoordToPosition(_map, coord);
    actor->setPosition(pos);
    addActor( actor );

    std::shared_ptr<Waypoint> waypoint = std::dynamic_pointer_cast<Waypoint>(actor);
    if ( waypoint && BUTCHER.getPlayer()->knowsWaypoint(getLevel()) )
      waypoint->activate();
    else if ( waypoint )
      cc::log("players does not know waypoint on level %d", getLevel());

  }
  else
  {
    cc::log("DungeonState::spawn: Failed to create actor id=%d.", id);
  }
}

std::vector<std::shared_ptr<Actor> > DungeonState::getActorsAt(cocos2d::Vec2 coord, std::function<bool(std::shared_ptr<Actor>)> filterFn)
{
  std::vector<std::shared_ptr<Actor> > actors;

  for ( auto a : _actors )
    if ( a->getTileCoord() == coord && filterFn(a) )
      actors.push_back(a);

  return actors;
}

std::vector<std::shared_ptr<Actor> > DungeonState::getActors(std::function<bool (std::shared_ptr<Actor>)> filter)
{
  std::vector<std::shared_ptr<Actor> > actors;

  for ( auto a : _actors )
    if ( filter(a) )
      actors.push_back(a);

  return actors;
}

int DungeonState::getLevel() const
{
  return _level;
}

void DungeonState::nextTurn()
{
  if ( _lastPlayerPosition != BUTCHER.getPlayer()->getTileCoord() )
  {
    _lastPlayerPosition = BUTCHER.getPlayer()->getTileCoord();
    computeFov(BUTCHER.getPlayer()->getTileCoord().x,
               BUTCHER.getPlayer()->getTileCoord().y);
  }

  for(auto a : _actors)
  {
    a->onNextTurn();
    a->getSprite()->setVisible( isInFov(a->getTileCoord()) );
  }


}

bool DungeonState::isBlocked(cc::Vec2 tileCoord, std::shared_ptr<Actor>* blocking_actor)
{
  bool blocked = isBlockedByWall(tileCoord.x, tileCoord.y);

  //Check for blocking actor
  if (!blocked)
  {
    for(std::shared_ptr<Actor> a : _actors)
    {
      cc::Vec2 tc = positionToTileCoord(_map, a->getPosition());
      if ( tc == tileCoord && a->isBlocking() )
      {
        blocked = true;
        if (blocking_actor)
          *blocking_actor = a;
        break;
      }
    }
  }

  return blocked;
}

bool DungeonState::isBlockedByWall(int x, int y)
{
  bool blocked = false;

  if ( x >= _map->getMapSize().width || y >= _map->getMapSize().height
       || x < 0 || y < 0 )
  {
    cc::log("%s out of range: x=%d y=%d", __PRETTY_FUNCTION__, x, y);
    return true;
  }

  //Check if tile is enterable
  unsigned gid = _meta->getTileGIDAt( cc::Vec2(x,y) );
  if ( gid )
  {
    auto properties = _map->getPropertiesForGID(gid).asValueMap();
    if (!properties.empty())
    {
      auto collision = properties["Collidable"].asString();
      blocked = ("True" == collision);
    }
  }

  return blocked;
}

bool DungeonState::isOpaque(cc::Vec2 tileCoord)
{
  std::shared_ptr<Actor> blocking_actor;
  bool blocked = isBlocked(tileCoord, &blocking_actor);

  return blocking_actor ? !blocking_actor->isTransparent() : blocked;
}

bool DungeonState::isOpaque(int x, int y)
{
  return isOpaque(cc::Vec2(x,y));
}

bool DungeonState::isVisited(int x, int y)
{
  char tile = _exploredMask.get(x,y);
  return tile == Tiles::FoG || tile == Tiles::FoV;
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
  return _exploredMask.get(tileCoord) == Tiles::FoV;
}

bool DungeonState::isAnyMobInFov()
{
  for( std::shared_ptr<Actor> a : _actors )
  {
    std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(a);
    if ( mob && isInFov(mob->getTileCoord()) )
      return true;
  }
  return false;
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
