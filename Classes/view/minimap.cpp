#include "minimap.h"
#include <butcher.h>
#include <dungeon/dungeon_state.h>
#include <actors/player.h>

namespace cc = cocos2d;

namespace butcher {

Minimap::Minimap()
  : _width(0)
  , _height(0)
  , _tileSize(0)
  , _margin(10)
  , _dungeon(nullptr)
  , _sprite(nullptr)
  , _playerSprite(nullptr)
{
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

Minimap::~Minimap()
{
  delete _sprite;
}

void Minimap::init()
{
  _dungeon = BUTCHER.getCurrentDungeon();
  _width = _dungeon->getMapUsableSize().width;
  _height = _dungeon->getMapUsableSize().height;
  _tileSize = std::min( (_visibleSize.width - 2*_margin) / _width,
                        (_visibleSize.height - 2*_margin) / _height );

  allocateTiles();
}

void Minimap::allocateTiles()
{
  _tiles.clear();

  for ( int y = 0; y < _height; ++y )
  {
    std::vector<cc::Sprite*> row;
    for ( int x = 0; x < _width; ++x )
    {
      row.push_back( nullptr );
    }
    _tiles.push_back(row);
  }

  _objects.clear();

  for ( int y = 0; y < _height; ++y )
  {
    std::vector<cc::Sprite*> row;
    for ( int x = 0; x < _width; ++x )
    {
      row.push_back( nullptr );
    }
    _objects.push_back(row);
  }
}

cocos2d::Sprite *Minimap::createTile(const std::string& fn)
{
  cc::Sprite* tile = cc::Sprite::create();

  if ( !fn.empty() )
  {
    tile->initWithFile(fn);

    float oWidth = tile->getBoundingBox().size.width;
    float oHeight = tile->getBoundingBox().size.height;
    float tileSize = _tileSize;

    tile->setScale(tileSize / oWidth, tileSize / oHeight);
  }

  return tile;
}

void Minimap::putObject(ActorID id)
{
  auto objVector = _dungeon->getActors([id](std::shared_ptr<Actor> a){
     return a->getID() == id;
  });
  if ( !objVector.empty() )
  {
    auto objActor = objVector.front();
    auto objCoord = objActor->getTileCoord();
    auto tile = createTile(objActor->getSprite()->getResourceName());
    tile->setPosition(objCoord.x*_tileSize, _sprite->getContentSize().height - objCoord.y*_tileSize);
    tile->setVisible(false);
    _sprite->addChild(tile,666);
    _objects[objCoord.y][objCoord.x] = tile;
  }
}

cocos2d::Node *Minimap::generate()
{
  _sprite = new cc::Sprite();
  _sprite->setContentSize( cc::Size(_width*_tileSize, _height*_tileSize));
  _sprite->setAnchorPoint(cc::Vec2(0.5, 0.5));

  _playerSprite = createTile("images/minimap_player.png");
  _sprite->addChild(_playerSprite);

  putObject(ActorID::STAIRS_DOWN);
  putObject(ActorID::STAIRS_UP);

  for ( int y = 0; y < _height; ++y )
  {
    for ( int x = 0; x < _width; ++x )
    {
      cc::Sprite* tile = nullptr;

      if ( _dungeon->isBlockedByWall(x,y)  )
      {
        tile = createTile("images/minimap_wall.png");
      }
      else
      {
        tile = createTile("images/minimap_floor.png");
      }

      tile->setVisible(false);
      tile->setOpacity(128);

      tile->setPosition(x*_tileSize, _sprite->getContentSize().height - y*_tileSize);
      _sprite->addChild(tile);
      _tiles[y][x] = tile;
    }
  }

  return _sprite;
}

cocos2d::Node* Minimap::update()
{
  cc::Vec2 playerCoord = BUTCHER.getPlayer()->getTileCoord();
  _playerSprite->setPosition(playerCoord.x*_tileSize, _sprite->getContentSize().height - playerCoord.y*_tileSize);

  for ( int y = 0; y < _height; ++y )
  {
    for ( int x = 0; x < _width; ++x )
    {
      cc::Sprite* tile = _tiles[y][x];
      if ( tile && !tile->isVisible() )
      {
        if ( _dungeon->isVisited(x,y) )
        {
          tile->setVisible(true);
          auto object = _objects[y][x];
          if ( object )
            object->setVisible(true);
        }
      }
    }
  }

  return _sprite;
}



}
