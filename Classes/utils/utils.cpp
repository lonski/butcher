#include "utils.h"

using namespace cocos2d;

namespace butcher {

Vec2 positionToTileCoord(TMXTiledMap *map, Vec2 pos)
{
    if ( !map )
        return Vec2::ZERO;

    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y)
            / map->getTileSize().height/ Director::getInstance()->getContentScaleFactor();

    return Vec2(x, y);
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

float calculateDistance(Vec2 a, Vec2 b)
{
  int dx = b.x - a.x;
  int dy = b.y - a.y;

  return sqrtf(dx*dx + dy*dy);
}

}
