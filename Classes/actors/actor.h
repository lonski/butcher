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

    unsigned id() const;

    std::string name() const;
    void setName(const std::string &name);

    bool blocks() const;
    void setBlocks(bool blocks);

    bool transparent() const;
    void setTransparent(bool transparent);

    cocos2d::Vec2 getTileCoord();

    cocos2d::Sprite* sprite() const;

    virtual bool performAction(ActorAction* action);
    virtual bool collide(Actor* obstacle);
    virtual void nextTurn();

    virtual void onDie();

private:
    unsigned _id;
    std::string _name;
    bool _blocks;
    bool _transparent;

    cocos2d::Sprite* _sprite;

};

}

#endif // ACTOR_H
