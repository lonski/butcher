#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include "cocos2d.h"

namespace butcher {


namespace Direction
{
  enum Symbol
  {
    None = 0,

    NorthWest = 1,
    North     = 2,
    NorthEast = 3,
    West      = 4,

    Middle    = 5,

    East      = 6,
    SouthWest = 7,
    South     = 8,
    SouthEast = 9,

    End
  };

  inline Symbol operator++(Symbol& x) { return x = (Symbol)(std::underlying_type<Symbol>::type(x) + 1); }
  inline Symbol operator*(Symbol c) {return c;}
  inline Symbol begin(Symbol) {return Symbol::None;}
  inline Symbol end(Symbol)   {return Symbol::End;}

  cocos2d::Vec2 getNeighbour(cocos2d::Vec2 pos, Direction::Symbol direction);
  bool isCardinal(Symbol dir);
  bool isOrdinal(Symbol dir);
  std::vector<Symbol> getPerpendicular(Symbol dir);
  Symbol getOpposite(Symbol dir);

  Symbol getRandomCardinal();

  bool isDiagonal(cocos2d::Vec2 from, cocos2d::Vec2 to);
  Symbol fromPosition(cocos2d::Vec2 pos);
  cocos2d::Vec2 toPosition(Symbol s);

}

}

#endif // DIRECTIONS_H
