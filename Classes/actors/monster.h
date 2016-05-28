#ifndef MONSTER_H
#define MONSTER_H

#include <actors/character.h>

namespace butcher {

struct ActorData;

class Monster : public Character
{
public:
  static Monster* create(const ActorData* data, Monster* allocated = nullptr);
  virtual Actor* clone(Actor* allocated = nullptr);

  virtual void nextTurn();

};

}


#endif // MONSTER_H
