#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>

namespace butcher {

class Subject;

class Observer
{
public:
  virtual ~Observer() {}
  virtual void onNotify(Subject* subject) = 0;
};

class Subject
{
public:
  Subject();
  virtual ~Subject();
  virtual void addObserver(Observer* observer);
  virtual void removeObserver(Observer* observer);
  virtual void notify();

private:
  std::set<Observer*> _observers;

};

}

#endif // OBSERVER_H
