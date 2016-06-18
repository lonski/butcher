#include "dungeon_generator.h"
#include <data/levels_generated.h>

namespace cc = cocos2d;

namespace butcher {

Grid DungeonGenerator::generate(const LevelData* data)
{
  if ( data == nullptr )
  {
    cc::log("%s LevelData is null!", __PRETTY_FUNCTION__);
    return Grid();
  }

  _settings = data;
  _grid = Grid(_settings->width(), _settings->height());

  putRooms();

  for (int y = 1; y < _grid.height; y += 2)
    for (int x = 1; x < _grid.width; x += 2)
      if (_grid.checkPattern(cc::Vec2(x,y), "#########") )
        growMaze(cc::Vec2(x,y));

  connectRooms();
  removeDeadEnds();

  cc::log("%s", _grid.toStr().c_str());

  return _grid;
}

void DungeonGenerator::putRooms()
{
  int minRoomSize = _settings->min_room_size();
  int maxRoomSize = _settings->max_room_size();

  int maxRooms = _settings->max_rooms();
  while ( maxRooms-- )
  {
    int w = cocos2d::RandomHelper::random_int(minRoomSize, maxRoomSize);
    int h = cocos2d::RandomHelper::random_int(minRoomSize, maxRoomSize);
    int x = cocos2d::RandomHelper::random_int(w / 2 + 2, _grid.width - w / 2 - 2);
    int y = cocos2d::RandomHelper::random_int(h / 2 + 2, _grid.height - h / 2 - 2);

    Room room(x,y,w,h);

    bool intersects = false;
    for (auto r : _rooms)
    {
      intersects = room.intersects(r);
      if ( intersects )
        break;
    }

    if ( !intersects )
    {
      room.put_into(_grid);
      _rooms.push_back(room);
    }
  }
}

void DungeonGenerator::growMaze(cc::Vec2 pos)
{
  std::vector<cc::Vec2> cells;
  cells.push_back( pos );
  _grid.set(pos, Tiles::FLOOR);

  while( !cells.empty() )
  {
    cc::Vec2 cell = cells.back();

    std::vector<Direction::Symbol> possibleDirections;

    for (auto dir : Direction::Symbol() )
      if ( Direction::isCardinal(dir) && canCarve(cell, dir))
          possibleDirections.push_back(dir);

    if ( !possibleDirections.empty() )
    {
      int i = cc::RandomHelper::random_int(0, (int)possibleDirections.size() - 1);
      cc::Vec2 nb = Direction::getNeighbour(cell, possibleDirections[i]);
      _grid.set(nb, Tiles::FLOOR);
      cells.push_back(nb);
    }
    else
      cells.pop_back();
  }
}

bool DungeonGenerator::canCarve(cocos2d::Vec2 pos, Direction::Symbol dir)
{
  cc::Vec2 toCarve = Direction::getNeighbour(pos, dir);

  if ( !_grid.isValid(toCarve) )
    return false;

  if ( _grid.get(toCarve) == Tiles::FLOOR )
    return false;

  std::string pattern = "###"
                        "#X#"
                        "###";
  switch(dir)
  {
    case Direction::South:
      pattern = "   "
                "#X#"
                "###";
    break;
    case Direction::North:
      pattern = "###"
                "#X#"
                "   ";
    break;
    case Direction::West:
      pattern = "## "
                "#X "
                "## ";
    break;
    case Direction::East:
      pattern = " ##"
                " X#"
                " ##";
    break;
    default:;
  }

  return _grid.checkPattern(toCarve, pattern);
}

void DungeonGenerator::removeDeadEnds()
{
  bool done = false;
  while(!done)
  {
    done = true;

    for (int y = 1; y < _grid.height; ++y)
    {
      for (int x = 1; x < _grid.width; ++x)
      {
        if ( _grid.get(x,y) == Tiles::WALL)
          continue;

        int exits = 0;
        for( auto dir : Direction::Symbol() )
          if ( Direction::isCardinal(dir) )
            if ( _grid.get(Direction::getNeighbour(cc::Vec2(x,y), dir)) != Tiles::WALL)
              ++exits;

        if ( exits > 1 )
          continue;

        _grid.set(cc::Vec2(x,y), Tiles::WALL);
        done = false;
      }
    }
  }
}

void DungeonGenerator::connectRooms()
{
  for ( Room& r : _rooms )
  {
    bool connected = false;
    while ( !connected || cc::RandomHelper::random_int(0,100) <= _settings->multiple_room_exit_chance() )
    {
      cc::Vec2 hole = r.getRandomWall();

      if ( _grid.checkPattern(hole, " . "
                                    "#X#"
                                    " . ") ||
           _grid.checkPattern(hole, " # "
                                    ".X."
                                    " # ") )
      {
        _grid.set(hole, Tiles::FLOOR);
        connected = true;
      }
    }
  }
}

}
