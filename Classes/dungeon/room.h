#ifndef ROOM_H
#define ROOM_H

#include <utils/grid.h>

namespace butcher {

struct Room
{
  Room(int w, int h);

  virtual void put_into(Grid& grid, cocos2d::Vec2 pos);
  virtual cocos2d::Vec2 getRandomWall() const;
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

}

#endif // ROOM_H
