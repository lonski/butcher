#ifndef TRAP_H
#define TRAP_H

#include <memory>
#include <actors/item.h>

namespace butcher {

class Trap : public Item
{
public:
  Trap(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onInterract(std::shared_ptr<Actor> actor);
};

}

#endif // TRAP_H
