#ifndef DUNGEONSTATE_H
#define DUNGEONSTATE_H

#include "cocos2d.h"
#include <fov/permissive-fov-cpp.h>
#include <dungeon/tile_gid.h>
#include <utils/grid.h>

namespace butcher {

class Actor;
class Player;
class DungeonLayer;

class DungeonState
{
public:
  DungeonState();

  cocos2d::TMXTiledMap* map() const;

  bool setMap(cocos2d::TMXTiledMap* map);
  bool setMap(const std::string& fn);

  void spawnActors(DungeonLayer* view);
  bool removeActor(Actor* actor, bool remove_node_child = true);

  void nextTurn();

  bool isBlocked(cocos2d::Vec2 tileCoord, Actor** blocking_actor = nullptr);
  bool isOpaque(cocos2d::Vec2 tileCoord);
  bool isOpaque(int x, int y);

  void visit(int x, int y);
  bool isInFov(cocos2d::Vec2 tileCoord);

private:
  cocos2d::TMXTiledMap* _map;
  cocos2d::TMXLayer*    _tiles;
  cocos2d::TMXLayer*    _objects;
  cocos2d::TMXLayer*    _meta;
  DungeonLayer* _currentView;
  permissive::maskT _fovMask;

  Grid _exploredMask;
  std::set< std::shared_ptr<Actor> > _actors;

  void spawnPlayer();
  void spawn(int id, cocos2d::Vec2 coord);

  void computeFov(int x, int y);

};

}

#endif // DUNGEONSTATE_H
