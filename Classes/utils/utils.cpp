#include "utils.h"

namespace cc = cocos2d;

namespace butcher {

cc::Vec2 positionToTileCoord(cc::TMXTiledMap *map, cc::Vec2 pos)
{
    if ( !map )
        return cc::Vec2::ZERO;

    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y)
            / map->getTileSize().height;

    return cc::Vec2(x, y);
}

cc::Vec2 tileCoordToPosition(cc::TMXTiledMap *map, cc::Vec2 coord)
{
    if ( !map )
        return cc::Vec2::ZERO;

    int x = coord.x * map->getTileSize().width;
    int y = (map->getMapSize().height*map->getTileSize().height - map->getTileSize().height * coord.y) ;

    return cc::Vec2(x + map->getTileSize().height/2, y - map->getTileSize().width/2);
}

std::vector<std::string> explode(const std::string &str, char ch)
{
  std::vector<std::string> result;
  std::string line;

  for (auto s : str)
  {
    if (s == ch)
    {
      if (!line.empty())
      {
        result.push_back(line);
        line.clear();
      }
    }
    else
    {
      line += s;
    }
  }

  if (!line.empty())
  {
    result.push_back(line);
    line.clear();
  }

  return result;
}

float calculateDistance(cc::Vec2 a, cc::Vec2 b)
{
  int dx = b.x - a.x;
  int dy = b.y - a.y;

  return sqrtf(dx*dx + dy*dy);
}

}
