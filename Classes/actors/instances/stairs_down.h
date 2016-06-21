#ifndef STAIRSDOWN_H
#define STAIRSDOWN_H

#include <actors/object.h>

namespace butcher {

class StairsDown : public Object
{
public:
  StairsDown(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onInterract(Actor* actor);

};

}

#endif // STAIRSDOWN_H
