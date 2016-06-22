#ifndef TARGET_H
#define TARGET_H

#include <memory>
#include <vector>

namespace butcher {

class Actor;

struct Target
{
  Target();
  Target(std::shared_ptr<Actor> actor);

  std::shared_ptr<Actor> first() const;
  bool isEmpty() const;

  std::vector<std::shared_ptr<Actor>> actors;
  int x;
  int y;
};

}

#endif // TARGET_H
