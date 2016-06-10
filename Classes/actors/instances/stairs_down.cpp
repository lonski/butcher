#include "stairs_down.h"
#include <actors/player.h>
#include <butcher.h>

namespace butcher {

StairsDown::StairsDown(const ActorData *data)
  : Object(data)
{

}

Actor* StairsDown::clone(Actor *allocated)
{
  StairsDown* o = dynamic_cast<StairsDown*>(allocated);
  if ( o == nullptr )
    o = new StairsDown(nullptr);

  Object::clone(o);

  return o;
}

void StairsDown::onInterract(Actor *actor)
{
  if ( dynamic_cast<Player*>(actor) )
  {
    BUTCHER.goToNextLevel();
  }
}

}
