#include "stairs_up.h"
#include <actors/player.h>
#include <butcher.h>
#include <view/loading_scene.h>

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

void StairsUp::onInterract(std::shared_ptr<Actor> actor)
{
  if ( std::dynamic_pointer_cast<Player>(actor) && BUTCHER.getDungeonLevel() > 1 )
  {
    LoadingScreen::run([&](){
      BUTCHER.goToLevel(BUTCHER.getDungeonLevel() - 1);
    }, "Going up..");
  }
}

}
