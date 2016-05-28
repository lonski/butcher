#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  static Player* create(const ActorData* data, Player* allocated = nullptr);
  virtual Actor* clone(Actor* allocated = nullptr);

};

}

#endif // PLAYER_H
