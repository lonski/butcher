#ifndef STAIRS_UP_H
#define STAIRS_UP_H

#include <actors/object.h>

namespace butcher {

class StairsUp : public Object
{
public:
  StairsUp(const ActorData* data);
  virtual Actor* clone(Actor* allocated = nullptr);

  virtual void onInterract(Actor* actor);

};

}

#endif // STAIRS_UP_H