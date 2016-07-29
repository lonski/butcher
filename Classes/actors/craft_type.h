#ifndef CRAFT_TYPE_H
#define CRAFT_TYPE_H

#include <map>

namespace butcher {

enum class CraftType {
  NONE,
  SMITHING,
  TANNING,
  ALCHEMY,
  ENGINEERING
};

struct CraftTypeLevelTreshold
{
  CraftTypeLevelTreshold(CraftType t, int l, int pts)
    : type(t)
    , level(l)
    , pointsNeeded(pts)
  {}

  CraftType type;
  int level;
  int pointsNeeded;
};

static std::vector<CraftTypeLevelTreshold> craftLevelTresholds = {
  CraftTypeLevelTreshold(CraftType::SMITHING, 1, 3),
  CraftTypeLevelTreshold(CraftType::SMITHING, 2, 9),

  CraftTypeLevelTreshold(CraftType::TANNING, 1, 3),
  CraftTypeLevelTreshold(CraftType::TANNING, 2, 9),

  CraftTypeLevelTreshold(CraftType::ALCHEMY, 1, 3),
  CraftTypeLevelTreshold(CraftType::ALCHEMY, 2, 9),

  CraftTypeLevelTreshold(CraftType::ENGINEERING, 1, 3),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 2, 9)
};

static inline int getCraftLevelPointsNeeded(CraftType t, int level)
{
  for ( auto lt : craftLevelTresholds )
  {
    if ( lt.type == t && lt.level == level )
      return lt.pointsNeeded;
  }
  return -1;
}

}

#endif // CRAFT_TYPE_H
