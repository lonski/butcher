#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>
#include <actors/amounted_item.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);
  virtual void onCollide(std::shared_ptr<Actor> obstacle);

private:
  typedef std::map<ActorID, AmountedItem> Inventory;

  Inventory _inventory;


};

}

#endif // PLAYER_H
