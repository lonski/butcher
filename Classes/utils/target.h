#ifndef TARGET_H
#define TARGET_H

#include <vector>

namespace butcher {

class Actor;

struct Target
{
  Target();
  Target(Actor* actor);

  Actor* first();

  std::vector<Actor*> actors;
  int x;
  int y;
};

}

#endif // TARGET_H
