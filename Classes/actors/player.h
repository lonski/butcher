#ifndef PLAYER_H
#define PLAYER_H

#include <actors/character.h>
#include <actors/amounted_item.h>
#include <actors/inventory.h>
#include <actors/craftbook.h>
#include <data/save_generated.h>
#include <utils/target.h>

namespace butcher {

struct ActorData;

class Player : public Character
{
public:
  Player(const ActorData* data);

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);
  void load(const SaveData* data);

  virtual void onCollide(std::shared_ptr<Actor> obstacle);
  virtual void onKill(std::shared_ptr<Character> killed);
  virtual void onHit(std::shared_ptr<Character> hit);
  virtual void onNextTurn();
  virtual void onDestroy(std::shared_ptr<Actor> killer);

  Inventory& getInventory();
  CraftBook& getCraftbook();

  virtual Damage getDamage();
  virtual int getAttribute(AttributeType type);
  virtual int takeDamage(Damage damage, std::shared_ptr<Actor> attacker);

  virtual void setExp(int exp);
  int getExpForNextLevel() const;

  void giveLevelUpBonuses();

  virtual bool canShootAt(cocos2d::Vec2 coord);
  bool isUsingRangedWeapon();

  void scheduleAction(std::shared_ptr<ActorAction> action);
  bool hasScheduledAction() const;
  ActorAction::Result triggerScheduledAction(Target target);

  virtual bool isOutOfControl();

  bool knowsWaypoint(int level) const;
  void addWaypoint(int level);
  std::vector<int> getWaypoints() const;
  int getHighestWaypoint() const;

  ActorID getQuickSwitchWeaponID() const;
  std::shared_ptr<Item> getQuickSwitchWeapon();
  void setQuickSwitchWeapon(ActorID itemId);

  virtual ActorAction::Result performAction(std::shared_ptr<ActorAction> action);
  virtual ActorAction::Result performAction(ActorAction* action);

private:
  Inventory _inventory;
  CraftBook _craftbook;
  std::set<int> _waypoints;
  ActorID _quickSwitchWeapon;

  std::shared_ptr<ActorAction> _scheduledAction;

  int getExpForLevel(int level) const;

};


}

#endif // PLAYER_H
