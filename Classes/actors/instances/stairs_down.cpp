#include "stairs_down.h"
#include <actors/player.h>
#include <butcher.h>
#include <view/loading_scene.h>

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

void StairsDown::onInterract(std::shared_ptr<Actor> actor)
{
  if ( std::dynamic_pointer_cast<Player>(actor) )
  {
    LoadingScreen::run([&](){
      BUTCHER.goToLevel(BUTCHER.getDungeonLevel() + 1);
    }, "Going down..");
  }
}

}
