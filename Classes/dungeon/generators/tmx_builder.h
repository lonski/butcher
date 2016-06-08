#ifndef TMX_BUILDER_H
#define TMX_BUILDER_H

#include "cocos2d.h"
#include <utils/grid.h>
#include <utils/directions.h>

namespace butcher
{

class TMXBuilder
{
public:
  TMXBuilder();

  cocos2d::TMXTiledMap *build(const Grid& grid);

private:
  cocos2d::TMXLayer*    _tiles;
  cocos2d::TMXLayer*    _meta;
  Grid _grid;

  bool checkPattern(const std::string& pattern, std::map<Direction, char>);
};

}

#endif // TMX_BUILDER_H
