#ifndef STAIRSDOWN_H
#define STAIRSDOWN_H

#include <actors/object.h>

namespace butcher {

class StairsDown : public Object
{
public:
  StairsDown(const ActorData* data);
  virtual Actor* clone(Actor* allocated = nullptr);

  virtual void onInterract(Actor* actor);

};

}

#endif // STAIRSDOWN_H
