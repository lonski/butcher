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

  virtual Actor* clone(Actor* allocated = nullptr);

  virtual void nextTurn();
  virtual void onDestroy(Actor* destroyer);

private:
  Ai _ai;
  std::vector<DropRule> _dropRules;

};

}


#endif // MONSTER_H
