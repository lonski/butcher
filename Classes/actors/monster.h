#ifndef MONSTER_H
#define MONSTER_H

#include <actors/character.h>
#include <actors/ai/ai.h>
#include <actors/drop_rule.h>

namespace butcher {

struct ActorData;

class Monster : public Character
{
public:
  Monster(const ActorData* data);

  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  virtual void nextTurn();
  virtual void onDestroy(std::shared_ptr<Actor> destroyer);

protected:
  virtual void setSprite(cocos2d::Sprite* sprite);

private:
  std::unique_ptr<Ai> _ai;
  std::vector<DropRule> _dropRules;

};

}


#endif // MONSTER_H
