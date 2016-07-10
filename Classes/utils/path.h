#ifndef PATH_H
#define PATH_H

#include "cocos2d.h"
#include <functional>

namespace butcher {

class Path
{
public:
  Path();
  bool calculate(cocos2d::Vec2 start,
                 cocos2d::Vec2 goal,
                 std::function<bool(cocos2d::Vec2)> is_blocked_fun,
                 std::function<double(cocos2d::Vec2, cocos2d::Vec2)> moveCostFun);

  cocos2d::Vec2 walk();
  bool empty() const;

private:
  std::function<bool(cocos2d::Vec2)> _blockedFun;
  std::list<cocos2d::Vec2> _path;

  std::set<cocos2d::Vec2> getNeighbours(cocos2d::Vec2 pos);
};

}

#endif // PATH_H
