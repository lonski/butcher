#ifndef OBJECT_H
#define OBJECT_H

#include <actors/actor.h>

namespace butcher {

struct ActorData;

class Object : public Actor
{
public:
  Object(const ActorData* data);
  virtual int getZ() const;

  virtual Actor* clone(Actor* allocated = nullptr);
};

}

#endif // OBJECT_H
