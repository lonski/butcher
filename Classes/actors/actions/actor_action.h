#ifndef ACTOR_ACTION_H
#define ACTOR_ACTION_H

namespace butcher {

class Actor;

class ActorAction
{
public:
    virtual bool perform(Actor* actor) = 0;

};

}

#endif // ACTOR_ACTION_H
