#include "maze_dungeon_generator.h"
#include <data/levels_generated.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

namespace butcher {

bool MazeDungeonGenerator::generate(DungeonDescription& description)
{
  if ( description.settings == nullptr )
  {
    cc::log("%s LevelData is null!", __PRETTY_FUNCTION__);
    return false;
  }

  _data = &description;
  _data->grid = Grid( _data->settings->width(), _data->settings->height());
  _data->rooms.clear();

  putRooms();

  for (int y = 1; y < _data->grid.height; y += 2)
    for (int x = 1; x < _data->grid.width; x += 2)
      if ( exitCount(cc::Vec2(x,y)) == 0 )
        growMaze(cc::Vec2(x,y));

  connectRooms();
  removeDeadEnds();

  for (int y = 1; y < _data->grid.height; y += 2)
    for (int x = 1; x < _data->grid.width; x += 2)
      if ( exitCount(cc::Vec2(x,y)) == 0 )
        growMaze(cc::Vec2(x,y));

  connectRooms();
  removeDeadEnds();

  return true;
}

void MazeDungeonGenerator::putRooms()
{
  int minRoomSize = _data->settings->min_room_size();
  int maxRoomSize = _data->settings->max_room_size();

  int maxRooms = _data->settings->max_rooms();
  while ( maxRooms-- )
  {
    int w = cocos2d::RandomHelper::random_int(minRoomSize, maxRoomSize);
    int h = cocos2d::RandomHelper::random_int(minRoomSize, maxRoomSize);
    int x = cocos2d::RandomHelper::random_int(w / 2 + 2, _data->grid.width - w / 2 - 2);
    int y = cocos2d::RandomHelper::random_int(h / 2 + 2, _data->grid.height - h / 2 - 2);

    int roll = cocos2d::RandomHelper::random_int(0,100);
    std::shared_ptr<Room> room;

    if ( roll <= _data->settings->rect_room_chance() )
      room.reset(new RectRoom(w,h));
    else
      room.reset(new PolygonRoom(w,h));

    bool overlaps = room->overlaps(_data->grid, cc::Vec2(x,y));

    if ( !overlaps )
    {
      room->put_into(_data->grid, cc::Vec2(x,y));
      _data->rooms.push_back(room);
    }
  }
}

void MazeDungeonGenerator::growMaze(cc::Vec2 pos)
{
  std::vector<cc::Vec2> cells;
  cells.push_back( pos );
  _data->grid.set(pos, Tiles::FLOOR);

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
           cc::RandomHelper::random_int(0,100) < _data->settings->windy_chance() )
      {
        dir = lastDir;
      }

      lastDir = dir;

      cc::Vec2 nb = Direction::getNeighbour(cell, dir);
      _data->grid.set(nb, Tiles::FLOOR);
      cells.push_back(nb);
    }
    else
      cells.pop_back();
  }
}

bool MazeDungeonGenerator::canCarve(cocos2d::Vec2 pos, Direction::Symbol dir)
{
  cc::Vec2 toCarve = Direction::getNeighbour(pos, dir);

  if ( !_data->grid.isValid(toCarve) )
    return false;

  if ( _data->grid.get(toCarve) == Tiles::FLOOR )
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

  return _data->grid.checkPattern(toCarve, pattern);
}

void MazeDungeonGenerator::removeDeadEnds()
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


        if ( exitCount(cc::Vec2(x,y)) > 1 )
          continue;

        _data->grid.set(cc::Vec2(x,y), Tiles::WALL);
        done = false;
      }
    }
  }
}

bool MazeDungeonGenerator::connectRoomAt(cc::Vec2 pos)
{
  if ( _data->grid.checkPattern(pos, " . "
                               "#X#"
                               " . ") ||
       _data->grid.checkPattern(pos, " # "
                               ".X."
                               " # ") )
  {
    _data->grid.set(pos, Tiles::FLOOR);
    return true;
  }

  return false;
}

int MazeDungeonGenerator::exitCount(cocos2d::Vec2 pos)
{
  int exits = 0;

  for( auto dir : Direction::Symbol() )
    if ( Direction::isCardinal(dir) )
      if ( _data->grid.get(Direction::getNeighbour(pos, dir)) != Tiles::WALL)
        ++exits;

  return exits;
}

void MazeDungeonGenerator::connectRooms()
{
  for (auto room : _data->rooms)
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
