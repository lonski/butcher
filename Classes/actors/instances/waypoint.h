#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <actors/object.h>

namespace butcher {

class Waypoint : public Object
{
public:
  Waypoint(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void onInterract(std::shared_ptr<Actor> actor);

  void activate();

private:
  bool _activated;
  std::string _openedSpriteImage;
  std::string _closedSpriteImage;


};

}

#endif // WAYPOINT_H
