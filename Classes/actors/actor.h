#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <memory>
#include <actors/actor_id.h>
#include <utils/observer.h>
#include "cocos2d.h"

namespace butcher {

struct ActorData;
class ActorAction;
class Character;

class Actor : public std::enable_shared_from_this<Actor>
            , public Subject
{
public:
  Actor(const ActorData *data);
  virtual ~Actor() = 0;

  static std::unique_ptr<Actor> create(const ActorData* data);
  virtual std::unique_ptr<Actor> clone(std::unique_ptr<Actor> allocated = nullptr);

  ActorID getID() const;
  virtual int getZ() const = 0;

  std::string getName() const;
  void setName(const std::string& name);

  bool isBlocking() const;
  void setBlocks(bool blocks);

  bool isTransparent() const;
  void setTransparent(bool transparent);

  cocos2d::Vec2 getTileCoord();
  void setTileCoord(cocos2d::Vec2 coord);
  void setPosition(int x, int y, bool no_sprite_pos = false);
  void setPosition(cocos2d::Vec2 pos, bool no_sprite_pos = false);
  cocos2d::Vec2 getPosition() const;

  std::unique_ptr<cocos2d::Sprite>& getSprite();

  virtual bool performAction(const ActorAction& action);
  virtual void nextTurn();

  /**
   * @brief Function is launched when actor tries to enter tile occubied by another actor
   * @param actor - an actor who is preveinting to enter tile
   */
  virtual void onCollide(std::shared_ptr<Actor> obstacle);

  /**
   * @brief Function is launched when actor enters a tile with another actors
   * @param actor - an actor who is entering the tile
   */
  virtual void onInterract(std::shared_ptr<Actor> actor);

  /**
   * @brief Function is launched when actor is destroyed(killed).
   * @param destroyer - actor who did destroy us.
   */
  virtual void onDestroy(std::shared_ptr<Actor> destroyer);

  /**
   * @brief Function is launched when actor caused other character's death
   * @param killed - a character who has been killed
   */
  virtual void onKill(std::shared_ptr<Character> killed);

  void fadeText(const std::string& text, cocos2d::Color4B color = cocos2d::Color4B::BLACK);

protected:
  virtual void setSprite(cocos2d::Sprite* sprite);

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
