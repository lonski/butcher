#include "shot_action.h"
#include <actors/player.h>
#include <actors/actions/attack_action.h>
#include <utils/path.h>
#include <dungeon/dungeon_state.h>
#include <butcher.h>
#include <data/actors_database.h>
#include <actors/monster.h>

namespace cc = cocos2d;

namespace butcher {

ShotAction::ShotAction(Target target)
  : _target(target)
{
}

ActorAction::Result ShotAction::perform(std::shared_ptr<Actor> performer)
{
  _performer = performer;
  if ( !performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  std::shared_ptr<Character> shotter = std::dynamic_pointer_cast<Character>(performer);

  if ( !shotter )
  {
    cocos2d::log("%s: shotter is not a character.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  if ( !shotter->canShootAt(_target.pos) )
  {
    std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(shotter);
    if ( p && p->isUsingRangedWeapon() && !hasAmmo())
      shotter->fadeText("Out of ammo!", cc::Color4B::RED);

    return ActorAction::Result::NOK;
  }


  std::shared_ptr<Character> victim = std::dynamic_pointer_cast<Character>(_target.first());
  if ( !victim )
  {
    cocos2d::log("%s: victim is not a character.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  if ( !takeAmmo() )
    return ActorAction::Result::NOK;

  cc::Vec2 shotTarget = victim->getPosition();

  DungeonState* dungeon = BUTCHER.getCurrentDungeon();

  DirectPath path;
  path.calculate(shotter->getTileCoord(), _target.pos, [dungeon](cocos2d::Vec2 pos){

    std::shared_ptr<Actor> blockingActor;
    bool blocked = dungeon->isBlocked(pos, &blockingActor);

    std::shared_ptr<Character> c = blockingActor ? std::dynamic_pointer_cast<Character>(blockingActor)
                                                 : nullptr;

    bool blockedByCharacter = blocked && c != nullptr;
    bool blockedByObject = blocked && !blockedByCharacter;

    //10% chance to block hit another character on the way
    return blockedByObject || (blockedByCharacter && cc::RandomHelper::random_int(0,100) <= 10 );

  }, false);

  if (path.empty())
  {
    cocos2d::log("%s: path is empty.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  cc::Vec2 pos = cc::Vec2::ZERO;
  while ( !path.empty() )
  {
    pos = path.walk();
  }

  //obstacle on the path
  if ( pos != victim->getTileCoord() )
  {
    auto characterOnTile = dungeon->getActorsAt(pos, [](std::shared_ptr<Actor> a){
      return std::dynamic_pointer_cast<Character>(a) != nullptr;
    });

    if ( !characterOnTile.empty() )
    {
      _target = Target(characterOnTile.front());
      victim = std::dynamic_pointer_cast<Character>(_target.first());
    }

    shotTarget = tileCoordToPosition(dungeon->map(), _target.pos);
  }

  runAnimation(shotter, shotTarget);

  return performer->performAction( new AttackAction(_target));
}

void ShotAction::runAnimation(std::shared_ptr<Character> shotter, cc::Vec2 shotTarget)
{
  std::shared_ptr<Actor> bullet = BUTCHER.actorsDatabase().createActor<Actor>(getAmmoId());
  if ( bullet )
  {
    bullet->setPosition(_performer->getPosition());
    auto view = shotter->getSprite()->getParent();
    if ( view )
    {
      auto sprite = bullet->getSprite().release();
      sprite->setScale(0.3);
      view->addChild(sprite);
      sprite->runAction(
            cc::Sequence::create(cc::MoveTo::create(0.2f, shotTarget), cc::RemoveSelf::create(), nullptr) );
    }
  }
}

bool ShotAction::takeAmmo()
{
  std::shared_ptr<Player> shotter = std::dynamic_pointer_cast<Player>(_performer);

  if ( !shotter )
    return true; //only player has to use ammo

  AmountedItem wpn = shotter->getInventory().equipped(ItemSlotType::WEAPON);
  bool taken = shotter->getInventory().removeItem(wpn.item->getAmmoId(), 1);
  if ( !taken )
    shotter->fadeText("Out of ammo!", cc::Color4B::RED);

  return taken;
}

bool ShotAction::hasAmmo()
{
  std::shared_ptr<Player> shotter = std::dynamic_pointer_cast<Player>(_performer);
  if ( !shotter )
    return true;

  auto item = shotter->getInventory().getItem(getAmmoId());

  return item.amount > 0;
}

ActorID ShotAction::getAmmoId()
{
  std::shared_ptr<Monster> mob = std::dynamic_pointer_cast<Monster>(_performer);
  if ( mob )
    return mob->getAmmoID();

  std::shared_ptr<Player> shotter = std::dynamic_pointer_cast<Player>(_performer);

  if ( !shotter )
    return ActorID::BONE; //TODO: change to arrow

  AmountedItem wpn = shotter->getInventory().equipped(ItemSlotType::WEAPON);

  if ( !wpn.item )
    return ActorID::INVALID;

  return wpn.item->getAmmoId();
}

}
