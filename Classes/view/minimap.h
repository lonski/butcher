#ifndef MINIMAP_H
#define MINIMAP_H

#include "cocos2d.h"
#include <actors/actor_id.h>

namespace butcher {

class DungeonState;

class Minimap
{
public:
  Minimap();
  ~Minimap();

  void init();
  cocos2d::Node* generate();
  cocos2d::Node* update();

private:
  std::vector< std::vector<cocos2d::Sprite*> > _tiles;
  std::vector< std::vector<cocos2d::Sprite*> > _objects;
  int _width;
  int _height;
  int _tileSize;
  int _margin;
  cocos2d::Size _visibleSize;
  DungeonState* _dungeon;
  cocos2d::Node* _sprite;
  cocos2d::Sprite* _playerSprite;

  void allocateTiles();
  cocos2d::Sprite* createTile(const std::string& fn);
  void fillTiles();
  void putObject(ActorID id);
};

}


#endif // MINIMAP_H
