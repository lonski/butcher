#ifndef STAIRS_UP_H
#define STAIRS_UP_H

#include <actors/object.h>

namespace butcher {

class StairsUp : public Object
{
public:
  StairsUp(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onInterract(std::shared_ptr<Actor> actor);

};

}

#endif // STAIRS_UP_H
