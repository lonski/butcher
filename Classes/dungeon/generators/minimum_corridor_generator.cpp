#include "minimum_corridor_generator.h"
#include <utils/directions.h>
#include <dungeon/dungeon_description.h>
#include <data/levels_generated.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

MinimumCorridorGenerator::MinimumCorridorGenerator()
  : _data(nullptr)
{
}

bool MinimumCorridorGenerator::generate(DungeonDescription &dsc)

{
  _data = &dsc;
  _data->grid = Grid(_data->settings->width(),
                     _data->settings->height(),
                     Tiles::INVALID);

  // place the first room in the center
  if (!makeRoom( _data->grid.width / 2,
                 _data->grid.height / 2,
                 Direction::getRandomCardinal(), true))
  {
    cc::log("Unable to place the first room.");
    return false;
  }

  //place rooms and corridors
  for (int i = 1; i < _data->settings->max_features(); ++i)
  {
    if (!createFeature())
    {
      cc::log("Unable to place more features (placed %d).", i);
      break;
    }
  }

  removeDeadEnds();

  for (char& tile : _data->grid.tiles)
  {
    if (tile == Tiles::INVALID)
      tile = Tiles::WALL;
    else if (tile == Tiles::CORRIDOR)
      tile = Tiles::FLOOR;
    else if (tile == Tiles::DOOR)
      tile = Tiles::FLOOR;
  }

  //cc::log("%s", _data->grid.toStr().c_str());

  return true;
}

void MinimumCorridorGenerator::removeDeadEnds()
{
  bool done = false;
  while(!done)
  {
    done = true;

    for (int y = 1; y < _data->grid.height; ++y)
    {
      for (int x = 1; x < _data->grid.width; ++x)
      {
        if ( _data->grid.get(x,y) == Tiles::WALL)
          continue;

        if ( _data->grid.getCardinalExitsCount(cc::Vec2(x,y)) > 1 )
          continue;

        _data->grid.set(cc::Vec2(x,y), Tiles::WALL);
        done = false;
      }
    }
  }
}

bool MinimumCorridorGenerator::makeRoom(int x, int y, Direction::Symbol dir, bool firstRoom)
{
  std::shared_ptr<Rect> room( new Rect( cc::RandomHelper::random_int(_data->settings->min_room_size(), _data->settings->max_room_size()),
                                        cc::RandomHelper::random_int(_data->settings->min_room_size(), _data->settings->max_room_size())));

  if (dir == Direction::North)
  {
    room->coord.x = x - room->width / 2;
    room->coord.y = y - room->height;
  }

  else if (dir == Direction::South)
  {
    room->coord.x = x - room->width / 2;
    room->coord.y = y + 1;
  }

  else if (dir == Direction::West)
  {
    room->coord.x = x - room->width;
    room->coord.y = y - room->height / 2;
  }

  else if (dir == Direction::East)
  {
    room->coord.x = x + 1;
    room->coord.y = y - room->height / 2;
  }

  if (room->place(_data->grid, Tiles::FLOOR))
  {
    _data->rooms.emplace_back(room);

    if (dir != Direction::South || firstRoom) // north side
      _exits.emplace_back(Rect{ room->width, 1, cc::Vec2(room->coord.x, room->coord.y - 1) });
    if (dir != Direction::North || firstRoom) // south side
      _exits.emplace_back(Rect{ room->width, 1, cc::Vec2(room->coord.x, room->coord.y + room->height) });
    if (dir != Direction::East || firstRoom) // west side
      _exits.emplace_back(Rect{ 1, room->height, cc::Vec2(room->coord.x - 1, room->coord.y) });
    if (dir != Direction::West || firstRoom) // east side
      _exits.emplace_back(Rect{ 1, room->height, cc::Vec2(room->coord.x + room->width, room->coord.y) });

    return true;
  }

  return false;
}

bool MinimumCorridorGenerator::createFeature()
{
  for (int i = 0; i < 1000; ++i)
  {
    if (_exits.empty())
      break;

    // choose a random side of a random room or corridor
    int r = cc::RandomHelper::random_int(0, (int)_exits.size() - 1);
    int x = cc::RandomHelper::random_int((int)_exits[r].coord.x, (int)_exits[r].coord.x + (int)_exits[r].width - 1);
    int y = cc::RandomHelper::random_int((int)_exits[r].coord.y, (int)_exits[r].coord.y + (int)_exits[r].height - 1);

    for (auto d : Direction::Symbol() )
    {
      if ( Direction::isCardinal(d) && createFeature(x, y, d))
      {
        _exits.erase(_exits.begin() + r);
        return true;
      }
    }
  }

  return false;
}

bool MinimumCorridorGenerator::createFeature(int x, int y, Direction::Symbol dir)
{
  int dx = 0;
  int dy = 0;

  if (dir == Direction::North)
    dy = 1;
  else if (dir == Direction::South)
    dy = -1;
  else if (dir == Direction::West)
    dx = 1;
  else if (dir == Direction::East)
    dx = -1;

  if ( _data->grid.get(x + dx, y + dy) != Tiles::FLOOR &&
       _data->grid.get(x + dx, y + dy) != Tiles::CORRIDOR)
    return false;

  if (cc::RandomHelper::random_int(0,100) < _data->settings->room_chance())
  {
    if (makeRoom(x, y, dir))
    {
      _data->grid.set(x, y, Tiles::DOOR);
      return true;
    }
  }
  else
  {
    if (makeCorridor(x, y, dir))
    {
      if (_data->grid.get(x + dx, y + dy) == Tiles::FLOOR)
        _data->grid.set(x, y, Tiles::DOOR);
      else // don't place a door between corridors
        _data->grid.set(x, y, Tiles::CORRIDOR);

      return true;
    }
  }

  return false;
}

bool MinimumCorridorGenerator::makeCorridor(int x, int y, Direction::Symbol dir)
{
  Rect corridor;
  corridor.coord = cc::Vec2(x, y);

  if (cc::RandomHelper::random_int(0,1)) // horizontal corridor
  {
    corridor.width = cc::RandomHelper::random_int(_data->settings->min_corridor_length(),
                                                  _data->settings->max_corridor_length());
    corridor.height = 1;

    if (dir == Direction::North)
    {
      corridor.coord.y = y - 1;

      if (cc::RandomHelper::random_int(0,1)) // west
        corridor.coord.x = x - corridor.width + 1;
    }

    else if (dir == Direction::South)
    {
      corridor.coord.y = y + 1;

      if (cc::RandomHelper::random_int(0,1)) // west
        corridor.coord.x = x - corridor.width + 1;
    }

    else if (dir == Direction::West)
      corridor.coord.x = x - corridor.width;

    else if (dir == Direction::East)
      corridor.coord.x = x + 1;
  }

  else // vertical corridor
  {
    corridor.width = 1;
    corridor.height = cc::RandomHelper::random_int(_data->settings->min_corridor_length(),
                                                   _data->settings->max_corridor_length());

    if (dir == Direction::North)
      corridor.coord.y = y - corridor.height;

    else if (dir == Direction::South)
      corridor.coord.y = y + 1;

    else if (dir == Direction::West)
    {
      corridor.coord.x = x - 1;

      if (cc::RandomHelper::random_int(0,1)) // north
        corridor.coord.y = y - corridor.height + 1;
    }

    else if (dir == Direction::East)
    {
      corridor.coord.x = x + 1;

      if (cc::RandomHelper::random_int(0,1)) // north
        corridor.coord.y = y - corridor.height + 1;
    }
  }

  if (corridor.place(_data->grid, Tiles::CORRIDOR))
  {
    if (dir != Direction::South && corridor.width != 1) // north side
      _exits.emplace_back(Rect{ corridor.width, 1, cc::Vec2(corridor.coord.x, corridor.coord.y - 1) });
    if (dir != Direction::North && corridor.width != 1) // south side
      _exits.emplace_back(Rect{ corridor.width, 1, cc::Vec2(corridor.coord.x, corridor.coord.y + corridor.height) });
    if (dir != Direction::East && corridor.height != 1) // west side
      _exits.emplace_back(Rect{ 1, corridor.height, cc::Vec2(corridor.coord.x - 1, corridor.coord.y) });
    if (dir != Direction::West && corridor.height != 1) // east side
      _exits.emplace_back(Rect{ 1, corridor.height, cc::Vec2(corridor.coord.x + corridor.width, corridor.coord.y) });

    return true;
  }

  return false;
}

}
