#include "die_action.h"
#include "cocos2d.h"
#include <butcher.h>
#include <dungeon/dungeon_state.h>

namespace butcher {

DieAction::DieAction()
{

}

bool DieAction::perform(Actor *actor)
{
  if ( !actor )
  {
    cocos2d::log("%s: actor is null", __PRETTY_FUNCTION__);
    return false;
  }

  actor->onDie();
  BUTCHER.currentDungeon()->removeActor(actor);

  return true;
}

}
