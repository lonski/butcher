#ifndef UTILS_H
#define UTILS_H

#include "cocos2d.h"

namespace butcher {

cocos2d::Vec2 positionToTileCoord(cocos2d::TMXTiledMap* map, cocos2d::Vec2 pos);
std::vector<std::string> explode(const std::string& str, char ch);

}

#endif // UTILS_H
