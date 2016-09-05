#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <actors/actor.h>
#include <actors/attribute_type.h>
#include <utils/damage.h>

namespace butcher {

class Character : public Actor
{
public:
    Character(const ActorData* data);
    virtual ~Character() = 0;

    virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);
    virtual int getZ() const;

    virtual int getLevel() const;
    virtual void setLevel(int level);

    virtual int getExp() const;
    virtual void setExp(int exp);

    virtual int getAttribute(AttributeType type);
    virtual void setAttribute(AttributeType type, int value);

    virtual int getHp() const;
    virtual void setHp(int hp);

    virtual int getMaxHp() const;
    virtual void setMaxHp(int maxHp);

    virtual int takeDamage(Damage damage, std::shared_ptr<Actor> attacker);

    virtual Damage getDamage();
    virtual void setDamage(const Damage &damage);

    virtual bool canShootAt(cocos2d::Vec2 coord);

    virtual bool isOutOfControl();
    virtual bool isAlive() const;

    virtual void onDestroy(std::shared_ptr<Actor> killer);

    virtual void addEffect(const Effect& effect);

protected:
    int _level;
    int _exp;
    int _hp;
    int _maxHp;
    Damage _damage;
    std::map<AttributeType, int> _attributes;

    int agregateModifiers(std::function<int(Modifier)> filter) const;
};

}

#endif // CHARACTER_H
