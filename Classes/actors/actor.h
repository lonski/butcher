#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include "cocos2d.h"

namespace butcher {

struct ActorData;
class ActorAction;

class Actor
{
public:
    Actor();
    virtual ~Actor() = 0;

    static Actor* create(const ActorData* data);
    virtual Actor* clone(Actor* allocated = nullptr);

    std::string name() const;
    void setName(const std::string &name);

    bool blocks() const;
    void setBlocks(bool blocks);

    bool transparent() const;
    void setTransparent(bool transparent);

    unsigned id() const;

    cocos2d::Sprite* sprite() const;

    bool performAction(ActorAction* action);

    virtual void nextTurn();

private:
    unsigned _id;
    std::string _name;
    bool _blocks;
    bool _transparent;

    cocos2d::Sprite* _sprite;

};

}

#endif // ACTOR_H
