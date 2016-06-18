#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <string>
#include <dungeon/generators/grid_generator.h>
#include "cocos2d.h"

namespace butcher {

class DungeonGenerator : public GridGenerator
{
public:
  virtual Grid generate(const LevelData* data);


private:
  struct Room
  {
    Room(int x, int y, int w, int h)
      : width(w)
      , height(h)
    {
      x1 = x - width / 2;
      x2 = x1 + width;
      y1 = y - height / 2;
      y2 = y1 + height;
    }

    bool intersects(const Room& room)
    {
      return x1 <= room.x2 && x2 >= room.x1 && y1 <= room.y2 && y2 >= room.y1;
    }

    void put_into(Grid& grid)
    {
      for( int x = x1; x < x2; ++x )
        for ( int y = y1; y < y2; ++y)
          grid.set(x,y,Tiles::FLOOR);
    }

    cocos2d::Vec2 getRandomWall() const
    {
      cocos2d::Vec2 point;

      if ( cocos2d::RandomHelper::random_int(0,1) == 0 ) // horizontal point
      {
        point.x = x1 + cocos2d::RandomHelper::random_int(1, width - 1);
        if ( cocos2d::RandomHelper::random_int(0,1) == 0 )
          point.y = y1 - 1;
        else
          point.y = y2;
      }
      else // vertical point
      {
        point.y = y1 + cocos2d::RandomHelper::random_int(1, height - 1);
        if ( cocos2d::RandomHelper::random_int(0,1) == 0 )
          point.x = x1 - 1;
        else
          point.x = x2;
      }

      return point;
    }

    int x1, x2, y1, y2;
    int width, height;
  };

  std::vector<Room> _rooms;
  Grid _grid;
  const LevelData* _settings;

  void putRooms();
  void growMaze(cocos2d::Vec2 pos);
  bool canCarve(cocos2d::Vec2 pos, Direction::Symbol dir);
  void removeDeadEnds();
  void connectRooms();
};

}

#endif // DUNGEON_GENERATOR_H
