#ifndef DOORCLOSED_H
#define DOORCLOSED_H

#include <actors/object.h>

namespace butcher {

class Door : public Object
{
public:
  Door(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  bool open();
  bool close();

private:
  bool _closed;
  std::string _openedSpriteImage;
  std::string _closedSpriteImage;
};

}

#endif // DOORCLOSED_H
