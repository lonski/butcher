#ifndef UTILS_H
#define UTILS_H

#include "cocos2d.h"

namespace butcher {

cocos2d::Vec2 positionToTileCoord(cocos2d::TMXTiledMap* map, cocos2d::Vec2 pos);

}

#endif // UTILS_H
