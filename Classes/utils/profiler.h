#ifndef PROFILER_H
#define PROFILER_H

#include <ctime>
#include <string>

namespace butcher {

class Profiler
{
public:
  enum Scope {
    START,
    LAST_READ
  };

  Profiler();

  double count(Scope scope = START);
  double log(const std::string& logInfo, Scope scope = START);

private:
  std::clock_t _start;
  std::clock_t _last;
  std::clock_t _end;

};

}

#endif // PROFILER_H
