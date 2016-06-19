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

  std::string tiles;
  int width;
  int height;
};

}

#endif // GRID_H
