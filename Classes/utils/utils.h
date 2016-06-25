#ifndef UTILS_H
#define UTILS_H

#include "cocos2d.h"
#include <utils/directions.h>

namespace butcher {

cocos2d::Vec2 positionToTileCoord(cocos2d::TMXTiledMap* map, cocos2d::Vec2 pos);
cocos2d::Vec2 tileCoordToPosition(cocos2d::TMXTiledMap* map, cocos2d::Vec2 coord);
std::vector<std::string> explode(const std::string& str, char ch);
float calculateDistance(cocos2d::Vec2 a, cocos2d::Vec2 b);
cocos2d::Label* make_label(const std::string& text,
                           cocos2d::Color4B color,
                           int size,
                           cocos2d::Vec2 anchor = cocos2d::Vec2(0.5,0.5));

template<typename T>
static T fromStr(const std::string& s)
{
  std::istringstream is(s);
  T t;
  is >> t;
  return t;
}

template<typename T>
static std::string toStr(const T& t)
{
  std::ostringstream os;
  os << t;
  return os.str();
}

}

#endif // UTILS_H
