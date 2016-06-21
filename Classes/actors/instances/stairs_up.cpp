#include "stairs_up.h"
#include <actors/player.h>
#include <butcher.h>

namespace butcher {

StairsUp::StairsUp(const ActorData *data)
  : Object(data)
{
}

std::unique_ptr<Actor> StairsUp::clone(std::unique_ptr<Actor> allocated)
{
  StairsUp* o = dynamic_cast<StairsUp*>(allocated.release());
  if ( o == nullptr )
    o = new StairsUp(nullptr);

  return std::move(Object::clone(std::unique_ptr<Actor>{o}));
}

void StairsUp::onInterract(Actor *actor)
{
  if ( dynamic_cast<Player*>(actor) )
  {
    BUTCHER.goToLevel(BUTCHER.getDungeonLevel() - 1);
  }
}

}
