#include "profiler.h"
#include "cocos2d.h"

namespace butcher {

Profiler::Profiler()
{
  _start = clock();
  _last = _start;
}

double Profiler::count(Scope scope)
{
  _end = clock();

  double diffticks = _end - (scope == START ? _start : _last);
  double diffms = diffticks / (CLOCKS_PER_SEC / 1000);

  _last = _end;

  return diffms;
}

double Profiler::log(const std::string& logInfo, Scope scope)
{
  double diffms = count(scope);

  cocos2d::log("Profiler: %fms -> %s", diffms, logInfo.c_str());

  return diffms;
}

}
