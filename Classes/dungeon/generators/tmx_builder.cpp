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
          _tiles->setTileGID((int)TileGID::Left, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::Right, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X."
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::Top, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::Bottom, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::TopRight, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               ".X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::BottomRight, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::BottomLeft, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X."
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::TopLeft, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::VPassage, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::HPassage, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X#"
                               " . ", neighbours) )
          _tiles->setTileGID((int)TileGID::TopDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" . "
                               "#X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::BottomDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               ".X#"
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::RightDeadEnd, cc::Vec2(x,y));
        else if ( checkPattern(" # "
                               "#X."
                               " # ", neighbours) )
          _tiles->setTileGID((int)TileGID::LeftDeadEnd, cc::Vec2(x,y));

        else
          _tiles->setTileGID((int)TileGID::Mid, cc::Vec2(x,y));

        //Spawn mob
        if ( mobs_count < max_mobs && cc::RandomHelper::random_int(0,100) < 3 )
        {
          objects.push_back( addActorSpawn(2, y, x) );
          ++mobs_count;
        }

      }
    }
  }

  //spawn stairs up
  char tile = 'X';
  do
  {
    int y = cc::RandomHelper::random_int(0,grid.height);
    int x = cc::RandomHelper::random_int(0,grid.width);
    tile =  grid.get(x,y);
    if ( tile == Tiles::FLOOR )
    {
      objects.push_back( addActorSpawn(4, y, x) );
    }
  }
  while ( tile != Tiles::FLOOR );

  //spawn stairs down
  tile = 'X';
  do
  {
    int y = cc::RandomHelper::random_int(0,grid.height);
    int x = cc::RandomHelper::random_int(0,grid.width);
    tile =  grid.get(x,y);
    if ( tile == Tiles::FLOOR )
    {
      objects.push_back( addActorSpawn(3, y, x) );
    }
  }
  while ( tile != Tiles::FLOOR );


  objectGroup->setObjects(objects);

  cc::log("%s: Generated %d mob spawns.", __PRETTY_FUNCTION__, mobs_count);

  return map;
}

cocos2d::Value TMXBuilder::addActorSpawn(int id, int y, int x)
{
  cc::ValueMap spawn;
  spawn["name"] = cc::Value("actor_spawn");
  spawn["id"] = cc::Value(id);
  spawn["x"] = cc::Value(x);
  spawn["y"] = cc::Value(y);

  return cc::Value(spawn);
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
