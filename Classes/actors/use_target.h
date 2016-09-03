#ifndef USE_TARGET_H
#define USE_TARGET_H

#include <string>

namespace butcher {

enum class UseTarget {
  None = 0,
  Self = 1,
  Weapon = 2,
  Floor  = 3,
  Range  = 4
};

static inline std::string UseTarget2Str(UseTarget t)
{
  switch ( t )
  {
    case UseTarget::Self:
      return "Self";
    case UseTarget::Weapon:
      return "Weapon";
    case UseTarget::Floor:
      return "Ground";
    case UseTarget::Range:
      return "Throw";
    default:;
  }

  return "";
}

}

#endif // USE_TARGET_H
