#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>
#include <actors/item.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual Actor* clone(Actor* allocated = nullptr);
  virtual void onCollide(Actor* obstacle);

private:
  typedef std::map<ActorID, AmountedItem> Inventory;

  Inventory _inventory;


};

}

#endif // PLAYER_H
