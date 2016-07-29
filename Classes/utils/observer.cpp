#include "observer.h"

namespace butcher {

Subject::Subject()
{
}

Subject::~Subject()
{
}

void Subject::addObserver(Observer* observer)
{
  _observers.insert( observer );
}

void Subject::removeObserver(Observer *observer)
{
  _observers.erase( observer );
}

void Subject::notify(EventType event)
{
  for ( auto o : _observers )
    o->onNotify(this, event);
}

}
