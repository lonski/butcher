#include "stairs_down.h"
#include <actors/player.h>
#include <butcher.h>

namespace butcher {

StairsDown::StairsDown(const ActorData *data)
  : Object(data)
{

}

std::unique_ptr<Actor> StairsDown::clone(std::unique_ptr<Actor> allocated)
{
  StairsDown* o = dynamic_cast<StairsDown*>(allocated.release());
  if ( o == nullptr )
    o = new StairsDown(nullptr);

  return std::move(Object::clone(std::unique_ptr<Actor>{o}));
}

void StairsDown::onInterract(Actor *actor)
{
  if ( dynamic_cast<Player*>(actor) )
  {
    BUTCHER.goToLevel(BUTCHER.getDungeonLevel() + 1);
  }
}

}
