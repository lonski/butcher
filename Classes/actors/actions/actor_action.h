#ifndef ACTOR_ACTION_H
#define ACTOR_ACTION_H

#include <memory>

namespace butcher {

class Actor;

class ActorAction
{
public:
    virtual bool perform(std::shared_ptr<Actor> actor) const = 0;

};

}

#endif // ACTOR_ACTION_H
