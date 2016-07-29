#ifndef CRAFT_VIEW_H
#define CRAFT_VIEW_H

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"

namespace butcher {

class Player;

class CraftView : public cocos2d::Layer
{
public:
  CraftView(std::shared_ptr<Player> player);

  static cocos2d::Scene* createScene(std::shared_ptr<Player> player);
  static CraftView* create(std::shared_ptr<Player> player);
  virtual bool init();

private:
  std::shared_ptr<Player> _player;

  cocos2d::Vec2 _origin;
  cocos2d::Size _visibleSize;
  float _margin;

};

}


#endif // CRAFT_VIEW_H
