#ifndef DUNGEONSTATE_H
#define DUNGEONSTATE_H

#include "cocos2d.h"
#include <lib/permissive-fov-cpp.h>
#include <dungeon/tile_gid.h>
#include <utils/grid.h>
#include <actors/actor_id.h>

namespace butcher {

class Actor;
class Player;
class DungeonLayer;

/**
 * @brief This class represents single dungeon level.
 *        It holds map state - field of view, fog of war,
 *        actors (mobs, objects).
 */
class DungeonState
{
public:

  DungeonState();

  cocos2d::TMXTiledMap* map() const;

  bool setMap(cocos2d::TMXTiledMap* map);
  bool setMap(const std::string& fn);
  void setMapUsableSize(int width, int height);
  cocos2d::Size getMapUsableSize() const;

  void onEnter(DungeonLayer* view);
  void onExit();

  bool removeActor(std::shared_ptr<Actor> actor, bool remove_node_child = true);

  void nextTurn();

  bool isBlocked(cocos2d::Vec2 tileCoord, std::shared_ptr<Actor>* blocking_actor = nullptr);
  bool isBlockedByWall(int x, int y);
  bool isOpaque(cocos2d::Vec2 tileCoord);
  bool isOpaque(int x, int y);
  bool isVisited(int x, int y);

  void visit(int x, int y);
  bool isInFov(cocos2d::Vec2 tileCoord);

  std::vector< std::shared_ptr<Actor> > getActorsAt(cocos2d::Vec2 coord,
                                                    std::function<bool(std::shared_ptr<Actor>)> filterFn = [](std::shared_ptr<Actor>){return true;});
  std::vector< std::shared_ptr<Actor> > getActors(std::function<bool(std::shared_ptr<Actor>)> filter);

private:
  cocos2d::TMXTiledMap* _map;
  cocos2d::TMXLayer*    _tiles;
  cocos2d::TMXLayer*    _meta;
  DungeonLayer* _currentView;
  permissive::maskT _fovMask;

  Grid _exploredMask;
  std::set< std::shared_ptr<Actor> > _actors;

  cocos2d::Size _usableMapSize;

  void addPlayer();
  void addActor(ActorID id, cocos2d::Vec2 coord);
  void spawnActors();

  void computeFov(int x, int y);

};

}

#endif // DUNGEONSTATE_H
