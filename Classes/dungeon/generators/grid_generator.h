#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include <utils/grid.h>

namespace butcher {

class GridGenerator
{
public:
  GridGenerator() {}
  virtual ~GridGenerator() {}

  virtual Grid generate() = 0;
};


}

#endif // GRIDGENERATOR_H
