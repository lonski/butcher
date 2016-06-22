#include "die_action.h"
#include "cocos2d.h"
#include <butcher.h>
#include <dungeon/dungeon_state.h>

namespace cc = cocos2d;

namespace butcher {

DieAction::DieAction(std::shared_ptr<Actor> killer)
  : _killer(killer)
{
}

bool DieAction::perform(std::shared_ptr<Actor> actor)
{
  if ( !actor )
  {
    cocos2d::log("%s: actor is null", __PRETTY_FUNCTION__);
    return false;
  }

  actor->onDestroy(_killer);

  cc::Sprite* s = actor->sprite().release();
  s->runAction( cc::Sequence::create(cc::FadeOut::create(0.5), cc::RemoveSelf::create(), nullptr) );
  if ( !BUTCHER.getCurrentDungeon()->removeActor(actor, false) )
    cc::log("%s Failed to remove actor!", __PRETTY_FUNCTION__);

  return true;
}

}
