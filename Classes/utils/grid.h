#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>
#include <memory>
#include <utils/utils.h>
#include "cocos2d.h"

namespace butcher {

struct Tiles
{
  static const char INVALID = '?';
  static const char WALL = '#';
  static const char FLOOR = '.';
  static const char FoV = 'v';
  static const char FoG = 'f';
  static const char DOOR = '+';
  static const char DOOR_OPEN = '/';
  static const char CORRIDOR = ',';
};

struct Grid
{
  Grid(int w = 0, int h = 0, char c = '#')
    : width(w)
    , height(h)
  {
    tiles.assign(w*h, c);
  }

  void set(cocos2d::Vec2 pos, char c)
  {
    set(pos.x, pos.y, c);
  }

  void set(int x,int y, char c)
  {
    if ( !isValid(x,y))
    {
      cocos2d::log("Grid::set Coords are not valid! x=%d,y=%d", x,y);
      return;
    }

    tiles[y*width + x] = c;
  }

  char get(cocos2d::Vec2 pos) const
  {
    return get(pos.x, pos.y);
  }

  char get(int x, int y) const
  {
    return isValid(x,y) ? tiles[y*width + x] : Tiles::INVALID;
  }

  bool isValid(cocos2d::Vec2 pos) const
  {
    return isValid(pos.x, pos.y);
  }

  bool isValid(int x, int y) const
  {
    return x >= 0 && x < width && y >=0 && y < height;
  }

  bool checkPattern(cocos2d::Vec2 pos, const std::string &pattern) const
  {
    if ( pattern.size() < 9 )
    {
      cocos2d::log("invalid pattern size - %u", (unsigned)pattern.size());
      return false;
    }

    return (get(Direction::getNeighbour(pos, Direction::NorthWest)) == pattern[0] || pattern[0] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::North)) == pattern[1] || pattern[1] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::NorthEast)) == pattern[2] || pattern[2] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::West)) == pattern[3] || pattern[3] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::East)) == pattern[5] || pattern[5] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::SouthWest)) == pattern[6] || pattern[6] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::South)) == pattern[7] || pattern[7] == ' ') &&
           (get(Direction::getNeighbour(pos, Direction::SouthEast)) == pattern[8] || pattern[8] == ' ');

  }

  cocos2d::Vec2 findCoordWithPattern(const std::string& pattern, int tries = 100)
  {
    cocos2d::Vec2 coord = cocos2d::Vec2::ZERO;

    while( tries-- && coord == cocos2d::Vec2::ZERO )
    {
      cocos2d::Vec2 randomCoord = cocos2d::Vec2( cocos2d::RandomHelper::random_int(0, width),
                                                 cocos2d::RandomHelper::random_int(0, height) );
      if ( checkPattern(randomCoord, pattern))
        coord = randomCoord;
    }

    return coord;
  }

  void floodfill(cocos2d::Vec2 pos, char c)
  {
    set(pos, c);
    for( auto d : Direction::Symbol() )
    {
      if ( Direction::isCardinal(d) )
      {
        cocos2d::Vec2 nb = Direction::getNeighbour(pos, d);
        if ( isValid(nb) && get(nb) != Tiles::WALL && get(nb) != c)
        {
          floodfill(nb, c);
        }
      }
    }
  }

  Grid cut(int x1, int x2, int y1, int y2)
  {
    Grid g(x2 - x1, y2 - y1);

    for(int y = y1; y < y2; ++y)
      for(int x = x1; x < x2; ++x )
        g.set(x-x1,y-y1, get(x,y));

    return g;
  }

  int getCardinalExitsCount(cocos2d::Vec2 pos)
  {
    int exits = 0;

    for( auto dir : Direction::Symbol() )
      if ( Direction::isCardinal(dir) )
        if ( get(Direction::getNeighbour(pos, dir)) != Tiles::WALL)
          ++exits;

    return exits;
  }

  std::string toStr() const
  {
    std::string str;
    int x(0);
    for(char c : tiles)
    {
      ++x;
      str += c;
      if(x == width)
      {
        str += "\n";
        x = 0;
      }
    }
    return str;
  }

  cocos2d::Vec2 findFirst(char tile)
  {
    for(int y=0; y<height; ++y)
      for(int x=0; x<width; ++x)
        if (get(x,y) == tile)
          return cocos2d::Vec2(x,y);

    return cocos2d::Vec2::ZERO;
  }

  std::string tiles;
  int width;
  int height;
};

}

#endif // GRID_H
