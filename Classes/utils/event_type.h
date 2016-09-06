#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include <string>

namespace butcher {

enum class EventType {
  None,
  Modified,
  LevelUP,
  Moved,
  ItemDestroyed,
  GameOver
};

struct EventData
{
  EventData(EventType t, const std::string& param = "")
    : id(t)
    , param(param)
  {}

  EventType id;
  std::string param;
};

}

#endif // EVENT_TYPE_H
