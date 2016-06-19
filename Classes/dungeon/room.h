#ifndef ROOM_H
#define ROOM_H

#include <utils/grid.h>
#include "cocos2d.h"

namespace butcher {

struct Room
{
  Room(int w, int h);

  virtual void put_into(Grid& grid, cocos2d::Vec2 pos);
  virtual cocos2d::Vec2 getRandomTile(char c) const;
  virtual std::vector<cocos2d::Vec2> getTiles(char c) const;
  virtual bool overlaps(const Grid& grid, cocos2d::Vec2 pos);

protected:
  int width, height;
  cocos2d::Vec2 _pos;
  Grid _shape;
};

struct RectRoom : Room
{
  RectRoom(int w, int h);
};

struct CircleRoom : Room
{
  CircleRoom(int w);
};

struct PolygonRoom : Room
{
  PolygonRoom(int w, int h);

protected:
  std::vector<cocos2d::Vec2> generateShape();
  bool validateShape();
  void strip();
};

}

#endif // ROOM_H
