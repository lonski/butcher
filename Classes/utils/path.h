#ifndef PATH_H
#define PATH_H

#include "cocos2d.h"
#include <functional>
#include <utils/directions.h>

namespace butcher {

class IPath
{
public:
  virtual cocos2d::Vec2 walk() = 0;
  virtual bool empty() const = 0;
};

class AStarPath : public IPath
{
public:
  AStarPath();
  bool calculate(cocos2d::Vec2 start,
                 cocos2d::Vec2 goal,
                 std::function<bool(cocos2d::Vec2)> is_blocked_fun,
                 std::function<double(cocos2d::Vec2, cocos2d::Vec2)> moveCostFun = [](cocos2d::Vec2 from, cocos2d::Vec2 to)
                                                                                   {
                                                                                     return Direction::isDiagonal(from, to) ? 1.5 : 1;
                                                                                   });

  virtual cocos2d::Vec2 walk();
  virtual bool empty() const;

private:
  std::function<bool(cocos2d::Vec2)> _blockedFun;
  std::list<cocos2d::Vec2> _path;

  std::set<cocos2d::Vec2> getNeighbours(cocos2d::Vec2 pos);
};

class DirectPath : public IPath
{
public:
  DirectPath();

  bool calculate(cocos2d::Vec2 start, cocos2d::Vec2 goal,
                 std::function<bool(cocos2d::Vec2)> is_blocked_fun,
                 bool clearOnFail = true);

  virtual cocos2d::Vec2 walk();
  virtual bool empty() const;

private:
  std::list<cocos2d::Vec2> _path;
  std::function<bool(cocos2d::Vec2)> _blockedFun;
  float _tg;

  cocos2d::Vec2 calculateNextPoint(cocos2d::Vec2 previous, cocos2d::Vec2 end);

};

}

#endif // PATH_H
