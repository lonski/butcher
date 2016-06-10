#include "stairs_up.h"
#include <actors/player.h>
#include <butcher.h>

namespace butcher {

StairsUp::StairsUp(const ActorData *data)
  : Object(data)
{
}

Actor *StairsUp::clone(Actor *allocated)
{
  StairsUp* o = dynamic_cast<StairsUp*>(allocated);
  if ( o == nullptr )
    o = new StairsUp(nullptr);

  Object::clone(o);

  return o;
}

void StairsUp::onInterract(Actor *actor)
{
  if ( dynamic_cast<Player*>(actor) )
  {
    BUTCHER.goToLevel(BUTCHER.getDungeonLevel() - 1);
  }
}

}
