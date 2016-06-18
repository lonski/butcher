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
  void setMapTemplate(const std::string& fn);

private:
  cocos2d::TMXLayer*    _tiles;
  cocos2d::TMXLayer*    _meta;
  cocos2d::TMXLayer*    _objects;
  Grid _grid;
  std::string _mapTemplateFn;

  cocos2d::Value addActorSpawn(int id, int y, int x);
};

}

#endif // TMX_BUILDER_H
