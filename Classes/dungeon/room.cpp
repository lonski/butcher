#include "room.h"

namespace butcher {

Room::Room(int w, int h)
  : width(w)
  , height(h)
{}

void Room::put_into(Grid &grid, cocos2d::Vec2 pos)
{
  _pos = pos;

  for ( int y = pos.y; y < pos.y + height; ++y)
    for( int x = pos.x; x < pos.x + width; ++x )
      if ( _shape.get(x - pos.x, y - pos.y) != Tiles::INVALID )
        grid.set(x,y, _shape.get(x - pos.x, y - pos.y));
}

cocos2d::Vec2 Room::getRandomWall() const
{
  std::vector<cocos2d::Vec2> walls;
  for (int y = 0; y < _shape.height; ++y)
    for ( int x = 0; x < _shape.width; ++x)
      if ( _shape.get(x,y) == Tiles::WALL )
        walls.push_back(cocos2d::Vec2(_pos.x+x,_pos.y+y));

  return walls[ cocos2d::RandomHelper::random_int(0, (int)walls.size() - 1) ];
}

bool Room::overlaps(const Grid &grid, cocos2d::Vec2 pos)
{
  for( int y = pos.y; y < pos.y + height; ++y )
    for( int x = pos.x; x < pos.x + width; ++x)
      if ( grid.get(x,y) != Tiles::WALL )
        return true;

  return false;
}

RectRoom::RectRoom(int w, int h)
  : Room(w,h)
{
  _shape = Grid(width, height, Tiles::FLOOR);

  for ( int x = 0; x < _shape.width; ++x )
  {
    _shape.set(x, 0, Tiles::WALL);
    _shape.set(x, height - 1, Tiles::WALL);
  }

  for ( int y = 0; y < _shape.height; ++y )
  {
    _shape.set(0, y, Tiles::WALL);
    _shape.set(width - 1, y, Tiles::WALL);
  }
}

CircleRoom::CircleRoom(int w)
  : Room(w,w)
{
  _shape = Grid(width, height, Tiles::INVALID);

  int circle_radius = std::min(width/2-1, height/2-1);

  for (int i = 0; i <= 2*circle_radius; i++)
  {
      for (int j = 0; j <= 2*circle_radius; j++)
      {
           float distance_to_centre = sqrt((i - circle_radius)*(i - circle_radius) + (j - circle_radius)*(j - circle_radius));
          if (distance_to_centre > circle_radius-0.5 && distance_to_centre < circle_radius+0.5)
          {
               _shape.set(i,j,Tiles::WALL);
          }
          else
          {
               _shape.set(i,j,Tiles::FLOOR);
          }
      }
  }
}
}
