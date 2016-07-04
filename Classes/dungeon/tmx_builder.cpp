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

  if ( !cc::FileUtils::getInstance()->isFileExist(_mapTemplateFn) )
  {
    cc::log("%s: Failed to open file %s.", __FUNCTION__, _mapTemplateFn.c_str());
    return nullptr;
  }

  //Init Empty map
  cc::TMXTiledMap* map = new cc::TMXTiledMap();
  map->initWithTMXFile(_mapTemplateFn);

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

  //Fill layers
  for(int y=0; y < grid.height; ++y)
  {
    for(int x=0; x < grid.width; ++x)
    {
      char tile = grid.get(x,y);
      if ( tile == Tiles::WALL )
      {
        _meta->setTileGID((int)MetaTileGID::Collidable, cc::Vec2(x,y));
        _tiles->setTileGID((int)TileGID::Wall, cc::Vec2(x,y));
      }
      else if ( tile == Tiles::FLOOR )
      {
        cc::Vec2 pos(x,y);

        if ( grid.checkPattern( pos, " . "
                                     "#X."
                                     " . ") )
          _tiles->setTileGID((int)TileGID::Left, cc::Vec2(x,y));
        else if ( grid.checkPattern(pos, " . "
                                         ".X#"
                                         " . ") )
          _tiles->setTileGID((int)TileGID::Right, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          ".X."
                                          " . ") )
          _tiles->setTileGID((int)TileGID::Top, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " . "
                                          ".X."
                                          " # ") )
          _tiles->setTileGID((int)TileGID::Bottom, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          ".X#"
                                          " . ") )
          _tiles->setTileGID((int)TileGID::TopRight, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " . "
                                          ".X#"
                                          " # ") )
          _tiles->setTileGID((int)TileGID::BottomRight, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " . "
                                          "#X."
                                          " # ") )
          _tiles->setTileGID((int)TileGID::BottomLeft, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          "#X."
                                          " . ") )
          _tiles->setTileGID((int)TileGID::TopLeft, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " . "
                                          "#X#"
                                          " . ") )
          _tiles->setTileGID((int)TileGID::VPassage, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          ".X."
                                          " # ") )
          _tiles->setTileGID((int)TileGID::HPassage, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          "#X#"
                                          " . ") )
          _tiles->setTileGID((int)TileGID::TopDeadEnd, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " . "
                                          "#X#"
                                          " # ") )
          _tiles->setTileGID((int)TileGID::BottomDeadEnd, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          ".X#"
                                          " # ") )
          _tiles->setTileGID((int)TileGID::RightDeadEnd, cc::Vec2(x,y));
        else if ( grid.checkPattern( pos, " # "
                                          "#X."
                                          " # ") )
          _tiles->setTileGID((int)TileGID::LeftDeadEnd, cc::Vec2(x,y));

        else
          _tiles->setTileGID((int)TileGID::Mid, cc::Vec2(x,y));

      }
    }
  }

  return map;
}

void TMXBuilder::setMapTemplate(const std::string &fn)
{
  _mapTemplateFn = fn;
}

}
