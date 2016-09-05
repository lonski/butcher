#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

namespace butcher {

enum class EventType {
  None,
  Modified,
  LevelUP,
  Moved,
  WeaponCracked,
  GameOver
};

}

#endif // EVENT_TYPE_H
