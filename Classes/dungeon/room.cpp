#include "room.h"

namespace cc = cocos2d;

namespace butcher {

Room::Room(int w, int h)
  : width(w)
  , height(h)
{
  if ( width % 2 == 0 )
    width++;
  if ( height % 2 == 0 )
    height++;
}

void Room::put_into(Grid &grid, cocos2d::Vec2 pos)
{
  _pos = pos;

  for ( int y = pos.y; y < pos.y + height; ++y)
    for( int x = pos.x; x < pos.x + width; ++x )
      if ( _shape.get(x - pos.x, y - pos.y) != Tiles::INVALID )
        grid.set(x,y, _shape.get(x - pos.x, y - pos.y));

//  Grid tmp = grid;
//  cc::log("%s", tmp.toStr().c_str());

//  std::vector<cc::Vec2> allWalls = getTiles(Tiles::WALL);
//  for (cc::Vec2 w : allWalls )
//  {
//    tmp.set(w, '@');
//  }
//  cc::log("%s", tmp.toStr().c_str());
}

cocos2d::Vec2 Room::getRandomTile(char c) const
{
  std::vector<cocos2d::Vec2> walls;
  for (int y = 0; y < _shape.height; ++y)
    for ( int x = 0; x < _shape.width; ++x)
      if ( _shape.get(x,y) == c )
        walls.push_back(cocos2d::Vec2(_pos.x+x,_pos.y+y));

  return walls[ cocos2d::RandomHelper::random_int(0, (int)walls.size() - 1) ];
}

std::vector<cocos2d::Vec2> Room::getTiles(char c) const
{
  std::vector<cocos2d::Vec2> tiles;

  for (int y = 0; y < _shape.height; ++y)
    for ( int x = 0; x < _shape.width; ++x)
      if ( _shape.get(x,y) == c)
        tiles.push_back(cocos2d::Vec2(_pos.x+x,_pos.y+y));

  return tiles;
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

PolygonRoom::PolygonRoom(int w, int h)
  : Room(std::max(w, 10), std::max(h, 10))
{
  int maxAttempts = 10;
  do
   generateShape();
  while ( !validateShape() && maxAttempts-- );

  strip();
}

std::vector<cocos2d::Vec2> PolygonRoom::generateShape()
{
  _shape = Grid(width, height, Tiles::INVALID);

  std::vector<cc::Vec2> points{
    cc::Vec2( cc::RandomHelper::random_int(1, width - 1), 0 ),
    cc::Vec2( 0, cc::RandomHelper::random_int(1, height - 1)),
    cc::Vec2( cc::RandomHelper::random_int(1, width - 1), height - 1 ),
    cc::Vec2( width - 1, cc::RandomHelper::random_int(1, height - 1))
  };
  points.push_back( points[0] );

  cc::Vec2 previous(cc::Vec2::ZERO);
  for ( auto current : points )
  {
    if ( previous.isZero() )
    {
      previous = current;
      continue;
    }

    //connect previous point to current
    _shape.set(previous, Tiles::WALL);
    while(previous != current)
    {
      cc::Vec2 ds = current - previous;

      int d = cc::RandomHelper::random_int(0,1);

      if ( (int)ds.x != 0 && d == 0)
        ds.x > 0 ? previous.x++ : previous.x--;

      if ( (int)ds.y != 0 && d == 1)
        ds.y > 0 ? previous.y++ : previous.y--;

      _shape.set(previous, Tiles::WALL);
    }
  }

  _shape.floodfill( cc::Vec2( width / 2, height / 2), Tiles::FLOOR );

  return points;
}

bool PolygonRoom::validateShape()
{
  for( int x = 0; x < width; ++x )
  {
    if ( _shape.get(x, 0) == Tiles::FLOOR )
      return false;

    if ( _shape.get(x, height - 1) == Tiles::FLOOR )
      return false;
  }

  for( int y = 0; y < height; ++y )
  {
    if ( _shape.get(0, y) == Tiles::FLOOR )
      return false;

    if ( _shape.get(width - 1, y) == Tiles::FLOOR )
      return false;
  }

  return true;
}

void PolygonRoom::strip()
{
  for (int y = 0; y < _shape.height; ++y)
    for ( int x = 0; x < _shape.width; ++x)
      if ( _shape.get(x,y) == Tiles::WALL )
      {
        int floors = 0;
        for ( auto d : Direction::Symbol() )
          if ( _shape.get( Direction::getNeighbour(cc::Vec2(x,y),d) ) == Tiles::FLOOR )
            ++floors;

        if ( floors == 0 )
          _shape.set(x,y, Tiles::INVALID);
      }
}

}
