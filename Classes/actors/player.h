#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual Actor* clone(Actor* allocated = nullptr);
  virtual void onCollide(Actor* obstacle);

};

}

#endif // PLAYER_H
