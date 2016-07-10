#include "directions.h"
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

namespace Direction {

cocos2d::Vec2 getNeighbour(cocos2d::Vec2 pos, Symbol direction)
{
  cocos2d::Vec2 nb = pos;

  switch( direction )
  {
    case Direction::North:
      --nb.y;
      break;
    case Direction::South:
      ++nb.y;
      break;
    case Direction::East:
      ++nb.x;
      break;
    case Direction::West:
      --nb.x;
      break;
    case Direction::NorthEast:
      --nb.y;
      ++nb.x;
      break;
    case Direction::NorthWest:
      --nb.y;
      --nb.x;
      break;
    case Direction::SouthEast:
      ++nb.y;
      ++nb.x;
      break;
    case Direction::SouthWest:
      ++nb.y;
      --nb.x;
      break;
    default:;
  }

  return nb;
}

bool isCardinal(Symbol dir)
{
  switch ( dir )
  {
    case Direction::North:
    case Direction::South:
    case Direction::West:
    case Direction::East:
      return true;

    default:
      return false;
  }
}

bool isOrdinal(Symbol dir)
{
  switch ( dir )
  {
    case Direction::NorthEast:
    case Direction::NorthWest:
    case Direction::SouthEast:
    case Direction::SouthWest:
      return true;

    default:
      return false;
  }
}

std::vector<Symbol> getPerpendicular(Symbol dir)
{
  switch(dir)
  {
    case North:
    case South:
      return std::vector<Symbol>{ West, East };

    case West:
    case East:
      return std::vector<Symbol>{ North, South };

    case NorthWest:
    case SouthEast:
      return std::vector<Symbol>{ NorthEast, SouthWest };

    case NorthEast:
    case SouthWest:
      return std::vector<Symbol>{ NorthWest, SouthEast };

    default:
      return std::vector<Symbol>();
  }
}

Symbol getOpposite(Symbol dir)
{
  switch(dir)
  {
    case North:
      return South;
    case South:
      return North;
    case West:
      return East;
    case East:
      return West;
    case NorthWest:
      return SouthEast;
    case NorthEast:
      return SouthWest;
    case SouthEast:
      return NorthWest;
    case SouthWest:
      return NorthEast;
    default:
      return None;
  }
}

Symbol getRandomCardinal()
{
  switch(cc::RandomHelper::random_int(0,3))
  {
    case 0:
      return North;
    case 1:
      return South;
    case 2:
      return West;
    case 3:
      return East;
    default:;
  }

  return None;
}

bool isDiagonal(cocos2d::Vec2 from, cocos2d::Vec2 to)
{
  cc::Vec2 diff = to - from;
  return diff.x !=0 && diff.y != 0;
}

Symbol fromPosition(cocos2d::Vec2 pos)
{
  if ( pos.x > 0 && pos.y == 0 )
    return East;

  if ( pos.x < 0 && pos.y == 0 )
    return West;

  if ( pos.x == 0 && pos.y > 0 )
    return South;

  if ( pos.x == 0 && pos.y < 0 )
    return North;

  if ( pos.x > 0 && pos.y > 0 )
    return SouthEast;

  if ( pos.x < 0 && pos.y > 0 )
    return SouthWest;

  if ( pos.x > 0 && pos.y < 0 )
    return NorthEast;

  if ( pos.x < 0 && pos.y < 0 )
    return NorthWest;

  return Middle;
}


}

}
