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
  enum ID {
    PlayerID = 1,
    CowID,
    StairsDownID,
    StairsUpID,
  };

  Actor(const ActorData *data);
  virtual ~Actor() = 0;

  static Actor* create(const ActorData* data);
  virtual Actor* clone(Actor* allocated = nullptr);

  unsigned id() const;
  virtual int getZ() const = 0;

  std::string name() const;
  void setName(const std::string &name);

  bool blocks() const;
  void setBlocks(bool blocks);

  bool transparent() const;
  void setTransparent(bool transparent);

  cocos2d::Vec2 getTileCoord();
  void setTileCoord(cocos2d::Vec2 coord);
  void setPosition(int x, int y, bool no_sprite_pos = false);
  void setPosition(cocos2d::Vec2 pos, bool no_sprite_pos = false);
  cocos2d::Vec2 getPosition() const;

  std::unique_ptr<cocos2d::Sprite>& sprite();

  virtual bool performAction(ActorAction* action);
  virtual bool collide(Actor* obstacle);
  virtual void nextTurn();

  virtual void onDie();

  void fadeText(const std::string& text, cocos2d::Color4B color = cocos2d::Color4B::BLACK);

private:
  unsigned _id;
  std::string _name;
  bool _blocks;
  bool _transparent;
  cocos2d::Vec2 _position;

  std::unique_ptr<cocos2d::Sprite> _sprite;

};

}

#endif // ACTOR_H
