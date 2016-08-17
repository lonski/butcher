#ifndef SHOTACTION_H
#define SHOTACTION_H

#include <actors/actions/actor_action.h>
#include <utils/target.h>
#include <actors/actor_id.h>

namespace butcher {

class Character;

class ShotAction : public ActorAction
{
public:
  ShotAction(Target target);
  virtual bool perform(std::shared_ptr<Actor> performer);

private:
  Target _target;
  std::shared_ptr<Actor> _performer;

  bool takeAmmo();
  bool hasAmmo();
  ActorID getAmmoId();

  void runAnimation(std::shared_ptr<Character> shotter, cocos2d::Vec2 shotTarget);
};

}

#endif // SHOTACTION_H
