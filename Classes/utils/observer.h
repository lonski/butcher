#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>
#include <utils/event_type.h>

namespace butcher {

class Subject;

class Observer
{
public:
  virtual ~Observer() {}
  virtual void onNotify(Subject* subject, const EventData& event) = 0;
};

class Subject
{
public:
  Subject();
  virtual ~Subject();
  virtual void addObserver(Observer* observer);
  virtual void removeObserver(Observer* observer);
  virtual void notify(const EventData& event);

private:
  std::set<Observer*> _observers;

};

}

#endif // OBSERVER_H
