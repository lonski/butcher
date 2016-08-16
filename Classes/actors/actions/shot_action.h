#ifndef SHOTACTION_H
#define SHOTACTION_H

#include <actors/actions/actor_action.h>
#include <utils/target.h>

namespace butcher {

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

};

}

#endif // SHOTACTION_H
