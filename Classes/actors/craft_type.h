#ifndef CRAFT_TYPE_H
#define CRAFT_TYPE_H

#include <map>

namespace butcher {

enum class CraftType {
  NONE,
  SMITHING,
  TANNING,
  COOKING,
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
  CraftTypeLevelTreshold(CraftType::SMITHING, 1, 0),
  CraftTypeLevelTreshold(CraftType::SMITHING, 2, 2),
  CraftTypeLevelTreshold(CraftType::SMITHING, 3, 6),
  CraftTypeLevelTreshold(CraftType::SMITHING, 4, 12),
  CraftTypeLevelTreshold(CraftType::SMITHING, 5, 20),
  CraftTypeLevelTreshold(CraftType::SMITHING, 6, 30),
  CraftTypeLevelTreshold(CraftType::SMITHING, 7, 42),
  CraftTypeLevelTreshold(CraftType::SMITHING, 8, 56),
  CraftTypeLevelTreshold(CraftType::SMITHING, 9, 72),
  CraftTypeLevelTreshold(CraftType::SMITHING, 10, 90),

  CraftTypeLevelTreshold(CraftType::TANNING, 1, 0),
  CraftTypeLevelTreshold(CraftType::TANNING, 2, 2),
  CraftTypeLevelTreshold(CraftType::TANNING, 3, 6),
  CraftTypeLevelTreshold(CraftType::TANNING, 4, 12),
  CraftTypeLevelTreshold(CraftType::TANNING, 5, 20),
  CraftTypeLevelTreshold(CraftType::TANNING, 6, 30),
  CraftTypeLevelTreshold(CraftType::TANNING, 7, 42),
  CraftTypeLevelTreshold(CraftType::TANNING, 8, 56),
  CraftTypeLevelTreshold(CraftType::TANNING, 9, 72),
  CraftTypeLevelTreshold(CraftType::TANNING, 10, 90),

  CraftTypeLevelTreshold(CraftType::COOKING, 1, 0),
  CraftTypeLevelTreshold(CraftType::COOKING, 2, 2),
  CraftTypeLevelTreshold(CraftType::COOKING, 3, 6),
  CraftTypeLevelTreshold(CraftType::COOKING, 4, 12),
  CraftTypeLevelTreshold(CraftType::COOKING, 5, 20),
  CraftTypeLevelTreshold(CraftType::COOKING, 6, 30),
  CraftTypeLevelTreshold(CraftType::COOKING, 7, 42),
  CraftTypeLevelTreshold(CraftType::COOKING, 8, 56),
  CraftTypeLevelTreshold(CraftType::COOKING, 9, 72),
  CraftTypeLevelTreshold(CraftType::COOKING, 10, 90),

  CraftTypeLevelTreshold(CraftType::ENGINEERING, 1, 0),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 2, 2),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 3, 6),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 4, 12),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 5, 20),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 6, 30),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 7, 42),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 8, 56),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 9, 72),
  CraftTypeLevelTreshold(CraftType::ENGINEERING, 10, 90),
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

static std::map<int, int> pointsPerLevel = {
  {1,1},
  {2,1},
  {3,2},
  {4,2},
  {5,3},
  {6,3},
  {7,4},
  {8,4},
  {9,5},
  {10,5},
  {11,6},
  {12,6},
  {13,7},
  {14,7},
  {15,8},
  {16,8},
  {17,9},
  {18,9},
  {19,10},
  {20,10}
};

static inline int getCraftPointsOnLevel(int level)
{
  auto it = pointsPerLevel.find(level);
  return it != pointsPerLevel.end() ? it->second : pointsPerLevel[20];
}

}

#endif // CRAFT_TYPE_H
