#include "tmx_builder.h"
#include <dungeon/tile_gid.h>
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher
{

TMXBuilder::TMXBuilder()
{
}

cocos2d::TMXTiledMap* TMXBuilder::build(const Grid &grid)
{
  _grid = grid;

  static const std::string tmx_template = "dungeons/empty_cave.tmx";

  if ( !cc::FileUtils::getInstance()->isFileExist(tmx_template) )
  {
    cc::log("%s: Failed to open file %s.", __FUNCTION__, tmx_template.c_str());
    return nullptr;
  }

  //Init Empty map
  cc::TMXTiledMap* map = new cc::TMXTiledMap();
  map->initWithTMXFile(tmx_template);

  //Read layers
  _tiles = map->getLayer("Background");
  if ( _tiles == nullptr )
  {
    cc::log("%s: Tile map has no layer 'Background'.", __FUNCTION__);
    return nullptr;
  }

  _objects = map->getLayer("Foreground");

  if ( _objects == nullptr )
  {
    cc::log("%s: Tile map has no layer 'Foreground'.", __FUNCTION__);
    return false;
  }

  _meta = map->getLayer("Meta");
  if ( _meta == nullptr )
  {
    cc::log("%s: Tile map has no layer 'Meta'.", __FUNCTION__);
    return nullptr;
  }

  cc::TMXObjectGroup* objectGroup = map->getObjectGroup("Objects");
  if(objectGroup == nullptr)
  {
      cc::log("%s: Tile map has no objects object layer.", __FUNCTION__);
      return nullptr;
  }

  int spawnX(0), spawnY(0);

  //Fill layers
  std::map<Direction, char> neighbours;
  cc::ValueVector objects;
  int mobs_count(0);
  int max_mobs(100);

  for(int y=0; y < grid.height; ++y)
  {
    for(int x=0; x < grid.width; ++x)
    {
      char tile = grid.get(x,y);
      if ( tile == Tiles::WALL )
      {
        _meta->setTileGID((int)MetaTileGID::Collidable, cc::Vec2(x,y));
      }
      else if ( tile == Tiles::FLOOR )
      {
        neighbours[Direction::North] = grid.get(x,y-1);
        neighbours[Direction::NorthEast] = grid.get(x+1,y-1);
        neighbours[Direction::East] = grid.get(x+1,y);
        neighbours[Direction::SouthEast] = grid.get(x+1,y+1);
        neighbours[Direction::South] = grid.get(x,y+1);
        neighbours[Direction::SouthWest] = grid.get(x-1,y+1);
        neighbours[Direction::West] = grid.get(x-1,y);
        neighbours[Direction::NorthWest] = grid.get(x-1,y-1);

        if ( checkPattern(" . "
                          "#X."
                          " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::Left, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::Right, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X."
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::Top, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::Bottom, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::TopRight, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::BottomRight, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::BottomLeft, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X."
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::TopLeft, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::VPassage, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::HPassage, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::TopDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::BottomDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::RightDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)CaveTileGID::LeftDeadEnd, cc::Vec2(x,y));

        else
          _tiles->setTileGID((int)CaveTileGID::Mid, cc::Vec2(x,y));

        //Spawn player
        if (spawnX == 0 && x < 80 && x > 20 && y < 80 && y > 20)
        {
          spawnX = x;
          spawnY = y;
        }

        //Spawn mob
        if ( mobs_count < max_mobs && cc::RandomHelper::random_int(0,100) < 3 )
        {
          cc::Vec2 pos = tileCoordToPosition(map, cc::Vec2(x,y));
          cc::ValueMap mobSpawn;
          mobSpawn["name"] = cc::Value("actor_spawn");
          mobSpawn["id"] = cc::Value(2);
          mobSpawn["x"] = cc::Value(x);
          mobSpawn["y"] = cc::Value(y);
          objects.push_back( cc::Value(mobSpawn) );
          ++mobs_count;
        }


      }
    }
  }

  //spawn Player
  cc::Vec2 pos = tileCoordToPosition(map, cc::Vec2(spawnX,spawnY));
  cc::ValueMap playerSpawn;
  playerSpawn["name"] = cc::Value("SpawnPoint");
  playerSpawn["x"] = cc::Value(spawnX);
  playerSpawn["y"] = cc::Value(spawnY);
  objects.push_back( cc::Value(playerSpawn) );

  //spawn stairs up
  _objects->setTileGID((int)CaveTileGID::StairsUp, cc::Vec2(spawnX, spawnY));

  //spawn stairs down
  char tile = 'X';
  do
  {
    int y = cc::RandomHelper::random_int(0,grid.height);
    int x = cc::RandomHelper::random_int(0,grid.width);
    tile =  grid.get(x,y);
    if ( tile == Tiles::FLOOR )
    {
      _objects->setTileGID((int)CaveTileGID::StairsDown, cc::Vec2(x,y));
    }
  }
  while ( tile != Tiles::FLOOR );


  objectGroup->setObjects(objects);

  cc::log("%s: Generated %d mob spawns.", __PRETTY_FUNCTION__, mobs_count);

  return map;
}

bool TMXBuilder::checkPattern(const std::string &pattern, std::map<Direction, char> n)
{
  if ( pattern.size() < 9 )
  {
    cc::log("invalid pattern size - %u", (unsigned)pattern.size());
    return false;
  }

  return (n[Direction::NorthWest] == pattern[0] || pattern[0] == ' ') &&
         (n[Direction::North] == pattern[1] || pattern[1] == ' ') &&
         (n[Direction::NorthEast] == pattern[2] || pattern[2] == ' ') &&
         (n[Direction::West] == pattern[3] || pattern[3] == ' ') &&
         (n[Direction::East] == pattern[5] || pattern[5] == ' ') &&
         (n[Direction::SouthWest] == pattern[6] || pattern[6] == ' ') &&
         (n[Direction::South] == pattern[7] || pattern[7] == ' ') &&
         (n[Direction::SouthEast] == pattern[8] || pattern[8] == ' ');

}

}
