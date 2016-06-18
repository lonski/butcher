#ifndef UTILS_H
#define UTILS_H

#include "cocos2d.h"
#include <utils/directions.h>

namespace butcher {

cocos2d::Vec2 positionToTileCoord(cocos2d::TMXTiledMap* map, cocos2d::Vec2 pos);
cocos2d::Vec2 tileCoordToPosition(cocos2d::TMXTiledMap* map, cocos2d::Vec2 coord);
std::vector<std::string> explode(const std::string& str, char ch);
float calculateDistance(cocos2d::Vec2 a, cocos2d::Vec2 b);

}

#endif // UTILS_H
