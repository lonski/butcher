#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>
#include <actors/amounted_item.h>
#include <actors/inventory.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onCollide(std::shared_ptr<Actor> obstacle);
  virtual void onKill(std::shared_ptr<Character> killed);

  Inventory& getInventory();

  virtual Damage getDamage();
  virtual int getAttribute(AttributeType type);

  virtual void setExp(int exp);
  int getExpForNextLevel() const;

private:
  Inventory _inventory;

};

}

#endif // PLAYER_H
