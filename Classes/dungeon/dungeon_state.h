#ifndef DUNGEONSTATE_H
#define DUNGEONSTATE_H

#include "cocos2d.h"

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

  void nextTurn();

  bool isBlocked(cocos2d::Vec2 tileCoord, Actor** blocking_actor = nullptr);

private:
  cocos2d::TMXTiledMap* _map;
  cocos2d::TMXLayer*    _tiles;
  cocos2d::TMXLayer*    _meta;

  std::set< std::shared_ptr<Actor> > _actors;

  void spawnPlayer();
  void spawn(int id, int x, int y);

};

}

#endif // DUNGEONSTATE_H
