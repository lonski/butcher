#ifndef GRID_H
#define GRID_H

#include <string>

namespace butcher {

struct Tiles
{
  static const char INVALID = '?';
  static const char WALL = '#';
  static const char FLOOR = '.';
  static const char FoV = 'v';
  static const char FoG = 'f';
};

struct Grid
{
  Grid(int w = 0, int h = 0, char c = '#')
    : width(w)
    , height(h)
  {
    tiles.assign(w*h, c);
  }

  void set(int x,int y, char c)
  {
    tiles[y*width + x] = c;
  }

  char get(int x, int y) const
  {
    return isValid(x,y) ? tiles[y*width + x] : Tiles::INVALID;
  }

  bool isValid(int x, int y) const
  {
    return x >= 0 && x < width && y >=0 && y < height;
  }

  std::string tiles;
  int width;
  int height;
};

}

#endif // GRID_H
