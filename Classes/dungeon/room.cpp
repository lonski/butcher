#include "room.h"

namespace cc = cocos2d;

namespace butcher {

Rect::Rect(int w, int h, cocos2d::Vec2 c)
  : width(w)
  , height(h)
  , coord(c)
{
}

cocos2d::Vec2 Rect::getRandomCoord(int margin) const
{
  return cc::Vec2( cc::RandomHelper::random_int( (int)coord.x + margin, (int)coord.x + width - 1 - margin),
                   cc::RandomHelper::random_int( (int)coord.y + margin, (int)coord.y + height - 1 - margin) );
}

int Rect::getAreaSize() const
{
  return width * height;
}

int Rect::getFloorSize(const Grid& gridPlacedOn) const
{
  int floors = 0;

  for (int y = coord.y - 1; y < coord.y + height + 1; ++y)
    for (int x = coord.x - 1; x < coord.x + width + 1; ++x)
      if ( gridPlacedOn.get(x,y) == Tiles::FLOOR )
        ++floors;

  return floors;
}

cocos2d::Vec2 Rect::getRandomFloorCoord(const Grid &gridPlacedOn) const
{
  cc::Vec2 coord = cc::Vec2::ZERO;

  int tries = 50;
  while ( coord == cc::Vec2::ZERO && tries-- )
  {
    cc::Vec2 randomCoord = getRandomCoord();
    if ( gridPlacedOn.get(randomCoord) == Tiles::FLOOR )
      coord = randomCoord;
  }

  return coord;
}

cocos2d::Vec2 Rect::findCoordWithPattern(const Grid &gridPlacedOn, const std::string &pattern, int tries)
{
  cocos2d::Vec2 coord = cocos2d::Vec2::ZERO;

  while( tries-- && coord == cocos2d::Vec2::ZERO )
  {
    cocos2d::Vec2 randomCoord = getRandomFloorCoord(gridPlacedOn);
    if ( gridPlacedOn.checkPattern(randomCoord, pattern))
      coord = randomCoord;
  }

  return coord;
}

bool Rect::place(Grid &grid, char c)
{
  if (coord.x < 1 || coord.y < 1 || coord.x + width > grid.width - 1 || coord.y + height > grid.height - 1)
    return false;

  for (int y = coord.y; y < coord.y + height; ++y)
    for (int x = coord.x; x < coord.x + width; ++x)
    {
      if (grid.get(x, y) != Tiles::INVALID)
        return false; // the area already used
    }

  for (int y = coord.y - 1; y < coord.y + height + 1; ++y)
    for (int x = coord.x - 1; x < coord.x + width + 1; ++x)
    {
      if (x == coord.x - 1 || y == coord.y - 1 || x == coord.x + width || y == coord.y + height)
        grid.set(x, y, Tiles::WALL);
      else
        grid.set(x, y, c);
    }

  return true;
}

cocos2d::Vec2 Rect::getCenter() const
{
  return cc::Vec2(coord.x + width / 2, coord.y + height / 2);
}

}
