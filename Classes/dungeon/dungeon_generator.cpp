#include "dungeon_generator.h"
#include <data/levels_generated.h>
#include <utils/profiler.h>

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
      if ( exitCount(cc::Vec2(x,y)) == 0 )
        growMaze(cc::Vec2(x,y));

  connectRooms();
  removeDeadEnds();

  for (int y = 1; y < _grid.height; y += 2)
    for (int x = 1; x < _grid.width; x += 2)
      if ( exitCount(cc::Vec2(x,y)) == 0 )
        growMaze(cc::Vec2(x,y));

  connectRooms();
  removeDeadEnds();

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

    int roll = cocos2d::RandomHelper::random_int(0,100);
    std::shared_ptr<Room> room;

    if ( roll <= _settings->rect_room_chance() )
      room.reset(new RectRoom(w,h));
    else
      room.reset(new PolygonRoom(w,h));

    bool overlaps = room->overlaps(_grid, cc::Vec2(x,y));

    if ( !overlaps )
    {
      room->put_into(_grid, cc::Vec2(x,y));
      _rooms.push_back(room);
    }
  }
}

void DungeonGenerator::growMaze(cc::Vec2 pos)
{
  std::vector<cc::Vec2> cells;
  cells.push_back( pos );
  _grid.set(pos, Tiles::FLOOR);

  Direction::Symbol lastDir = Direction::None;

  while( !cells.empty() )
  {
    cc::Vec2 cell = cells.back();

    std::vector<Direction::Symbol> possibleDirections;

    for (auto dir : Direction::Symbol() )
      if ( Direction::isCardinal(dir) && canCarve(cell, dir))
          possibleDirections.push_back(dir);

    if ( !possibleDirections.empty() )
    {
      Direction::Symbol dir = possibleDirections[cc::RandomHelper::random_int(0, (int)possibleDirections.size() - 1)];

      if ( std::find(possibleDirections.begin(), possibleDirections.end(), lastDir) != possibleDirections.end() &&
           cc::RandomHelper::random_int(0,100) < _settings->windy_chance() )
      {
        dir = lastDir;
      }

      lastDir = dir;

      cc::Vec2 nb = Direction::getNeighbour(cell, dir);
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


        if ( exitCount(cc::Vec2(x,y)) > 1 )
          continue;

        _grid.set(cc::Vec2(x,y), Tiles::WALL);
        done = false;
      }
    }
  }
}

bool DungeonGenerator::connectRoomAt(cc::Vec2 pos)
{
  if ( _grid.checkPattern(pos, " . "
                               "#X#"
                               " . ") ||
       _grid.checkPattern(pos, " # "
                               ".X."
                               " # ") )
  {
    _grid.set(pos, Tiles::FLOOR);
    return true;
  }

  return false;
}

int DungeonGenerator::exitCount(cocos2d::Vec2 pos)
{
  int exits = 0;

  for( auto dir : Direction::Symbol() )
    if ( Direction::isCardinal(dir) )
      if ( _grid.get(Direction::getNeighbour(pos, dir)) != Tiles::WALL)
        ++exits;

  return exits;
}

void DungeonGenerator::connectRooms()
{
  for (auto room : _rooms)
  {
    bool connected = false;
    int connectAttempts = 10;

    while ( !connected && connectAttempts-- )
    {
      connected = connectRoomAt( room->getRandomTile(Tiles::WALL) );
    }

    //recovery if failed to connect via random wall
    if ( connectAttempts == 0 )
    {
      std::vector<cc::Vec2> allWalls = room->getTiles(Tiles::WALL);
      for (cc::Vec2 w : allWalls )
      {
        connected = connectRoomAt(w);
        if ( connected )
            break;
      }
    }
  }

}

}
