#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <actors/actor.h>
#include <actors/attribute_type.h>

namespace butcher {

class Character : public Actor
{
public:
    Character();
    virtual ~Character() = 0;

    static Character* create(const ActorData* data, Character* allocated = nullptr);
    virtual Actor* clone(Actor* allocated = nullptr);

    int level() const;
    void setLevel(int level);

    int exp() const;
    void setExp(int exp);

    int attribute(AttributeType type);
    void setAttribute(AttributeType type, int value);

    int hp() const;
    void setHp(int hp);

    int takeDamage(int damage, Actor* attacker);

private:
    int _level;
    int _exp;
    int _hp;
    std::map<AttributeType, int> _attributes;
};

}

#endif // CHARACTER_H
