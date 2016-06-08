#ifndef ACTORS_DATABASE_H
#define ACTORS_DATABASE_H

#include <string>
#include <map>
#include <actors_generated.h>
#include <actors/actor.h>
#include "cocos2d.h"

namespace butcher {

class Actor;

class ActorDatabase
{
public:
  ActorDatabase();
  bool load(const std::string& fn);

  template<typename T>
  T* createActor(unsigned id)
  {
    T* t = nullptr;

    auto it = _actorTemplates.find(id);
    if ( it != _actorTemplates.end() )
    {
      Actor* actor = it->second->clone();
      t = dynamic_cast<T*>(actor);

      if ( t == nullptr )
      {
        cocos2d::log("ActorDatabase::createActor: failed to cast actor id=%u.", id);
        delete actor;
      }
    }
    else
    {
      cocos2d::log("ActorDatabase::createActor: failed to find actor id=%u.", id);
    }

    return t;
  }

private:
  std::map<unsigned, Actor*> _actorTemplates;
};

}

#endif // ACTORS_DATABASE_H
