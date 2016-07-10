#include "path.h"
#include <utils/directions.h>

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

Path::Path()
{
}

bool Path::calculate(cocos2d::Vec2 start, cocos2d::Vec2 goal,
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
    cc::log("visiting %f %f", current.x, current.y);
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

cocos2d::Vec2 Path::walk()
{
  if (empty())
    return cc::Vec2::ZERO;

  cc::Vec2 next = _path.front();
  _path.pop_front();

  return next;
}

bool Path::empty() const
{
  return _path.empty();
}

std::set<cocos2d::Vec2> Path::getNeighbours(cocos2d::Vec2 pos)
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



}
