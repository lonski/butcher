#include "die_action.h"
#include "cocos2d.h"
#include <butcher.h>
#include <dungeon/dungeon_state.h>

using namespace cocos2d;

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

  Sprite* s = actor->sprite().release();
  s->runAction( Sequence::create(FadeOut::create(0.5), RemoveSelf::create(), nullptr) );
  BUTCHER.currentDungeon()->removeActor(actor, false);

  return true;
}

}
