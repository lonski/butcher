#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include <utils/grid.h>

namespace butcher {

struct LevelData;

class GridGenerator
{
public:
  GridGenerator() {}
  virtual ~GridGenerator() {}

  virtual Grid generate(const LevelData* data) = 0;
};


}

#endif // GRIDGENERATOR_H
