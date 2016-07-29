#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>
#include <actors/amounted_item.h>
#include <actors/inventory.h>
#include <actors/craftbook.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onCollide(std::shared_ptr<Actor> obstacle);
  virtual void onKill(std::shared_ptr<Character> killed);
  virtual void onHit(std::shared_ptr<Character> hit);

  Inventory& getInventory();
  CraftBook& getCraftbook();

  virtual Damage getDamage();
  virtual int getAttribute(AttributeType type);
  virtual int takeDamage(Damage damage, std::shared_ptr<Actor> attacker);

  virtual void setExp(int exp);
  int getExpForNextLevel() const;

private:
  Inventory _inventory;
  CraftBook _craftbook;

};

}

#endif // PLAYER_H
