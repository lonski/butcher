#ifndef ITEM_H
#define ITEM_H

#include <actors/actor.h>

namespace butcher {

struct ActorData;

class Item : public Actor
{
public:
  Item(const ActorData* data);

  virtual int getZ() const;

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);
};

}
#endif // ITEM_H
