#ifndef TARGET_H
#define TARGET_H

#include <memory>
#include <vector>
#include "cocos2d.h"

namespace butcher {

class Actor;

struct Target
{
  Target();
  Target(std::shared_ptr<Actor> actor);
  Target(cocos2d::Vec2 coord);

  std::shared_ptr<Actor> first() const;
  bool isEmpty() const;

  std::vector<std::shared_ptr<Actor>> actors;
  cocos2d::Vec2 pos;
};

}

#endif // TARGET_H
