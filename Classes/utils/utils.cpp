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

}
