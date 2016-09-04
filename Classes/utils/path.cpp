#include "path.h"
#include <utils/directions.h>
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

template<typename T, typename priority_t>
struct PriorityQueue {
  typedef std::pair<priority_t, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
                 std::greater<PQElement>> elements;

  inline bool empty() const { return elements.empty(); }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

inline double heuristic(cc::Vec2 a, cc::Vec2 b)
{
  int x1(a.x), y1(a.y), x2(b.x), y2(b.y);
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

AStarPath::AStarPath()
{
}

bool AStarPath::calculate(cocos2d::Vec2 start, cocos2d::Vec2 goal,
                     std::function<bool (cocos2d::Vec2)> is_blocked_fun,
                     std::function<double(cocos2d::Vec2, cocos2d::Vec2)> moveCostFun)
{
  _blockedFun = is_blocked_fun;
  _path.clear();

  PriorityQueue<cc::Vec2, double> frontier;
  frontier.put(start,0);

  std::map<cc::Vec2, cc::Vec2> cameFrom;
  cameFrom[start] = start;

  std::map<cc::Vec2, double> costSoFar;
  costSoFar[start] = 0;

  bool found = false;

  while(!frontier.empty())
  {
    cc::Vec2 current = frontier.get();
    if ( current == goal )
    {
      found = true;
      break;
    }

    for( cc::Vec2 next : getNeighbours(current) )
    {
      double newCost = costSoFar[current] + moveCostFun(current, next);

      if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next])
      {
        costSoFar[next] = newCost;
        cameFrom[next] = current;

        double priority = newCost + heuristic(next, goal);
        frontier.put(next, priority);
      }
    }
  }

  if (!found)
    return false;

  //construct path
  cc::Vec2 current = goal;
  _path.push_front(current);
  while (current != start)
  {
    current =  cameFrom[current];
    _path.push_front(current);
  }

  return true;
}

cocos2d::Vec2 AStarPath::walk()
{
  if (empty())
    return cc::Vec2::ZERO;

  cc::Vec2 next = _path.front();
  _path.pop_front();

  return next;
}

bool AStarPath::empty() const
{
  return _path.empty();
}

std::set<cocos2d::Vec2> AStarPath::getNeighbours(cocos2d::Vec2 pos)
{
  std::set<cocos2d::Vec2> nb;

  for ( Direction::Symbol s : Direction::Symbol() )
  {
    if ( Direction::isCardinal(s) || Direction::isOrdinal(s) )
    {
      cc::Vec2 currentNb = Direction::getNeighbour(pos, s);
      if ( !_blockedFun(currentNb) )
      {
        nb.insert(currentNb);
      }
    }
  }

  return nb;
}

DirectPath::DirectPath()
  : _tg(0.f)
{
}

bool DirectPath::calculate(cocos2d::Vec2 start, cocos2d::Vec2 goal, std::function<bool (cocos2d::Vec2)> is_blocked_fun, bool clearOnFail)
{
  bool r = true;

  _blockedFun = is_blocked_fun;
  _path.clear();
  _tg = tangens(start, goal);

  cc::Vec2 current = start;
  _path.push_back(current);

  while ( current != goal )
  {
    current = calculateNextPoint(current, goal);

    _path.push_back(current);

    bool blocked = _blockedFun(current);
    if ( blocked )
    {
      r = false;
      if ( clearOnFail )
        _path.clear();
      break;
    }
  }

  return r;
}

cocos2d::Vec2 DirectPath::walk()
{
  if (empty())
    return cc::Vec2::ZERO;

  cc::Vec2 next = _path.front();
  _path.pop_front();

  return next;
}

bool DirectPath::empty() const
{
  return _path.empty();
}

cocos2d::Vec2 DirectPath::calculateNextPoint(cc::Vec2 previous, cocos2d::Vec2 end)
{
  cc::Vec2 dp = end - previous;
  cc::Vec2 next = previous;

  if ( dp != cc::Vec2::ZERO && std::abs(dp.y) > std::abs(dp.x) )
  {
    if( dp.y != 0) dp.y > 0 ? ++next.y : --next.y;
    next.x += ((int)(next.y / _tg) - (int)(previous.y / _tg));
  }
  else if ( dp != cc::Vec2::ZERO  && std::abs(dp.y) < std::abs(dp.x) )
  {
    if( dp.x != 0) dp.x > 0 ? ++next.x : --next.x;
    next.y += ((int)(next.x * _tg) - (int)(previous.x * _tg));
  }
  else
  {
    if( dp.x != 0) dp.x > 0 ? ++next.x : --next.x;
    if( dp.y != 0) dp.y > 0 ? ++next.y : --next.y;
  }

  return next;
}



}
