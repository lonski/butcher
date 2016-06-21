#ifndef ACTORS_DATABASE_H
#define ACTORS_DATABASE_H

#include <string>
#include <map>
#include <data/actors_generated.h>
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
  std::unique_ptr<T> createActor(ActorID id)
  {
    auto it = _actorTemplates.find(id);
    if ( it != _actorTemplates.end() )
    {
      return std::unique_ptr<T>{ dynamic_cast<T*>( it->second->clone().release()) };
    }
    else
    {
      cocos2d::log("ActorDatabase::createActor: failed to find actor id=%u.", id);
    }

    return nullptr;
  }

private:
  std::map<ActorID, std::unique_ptr<Actor>> _actorTemplates;
};

}

#endif // ACTORS_DATABASE_H
