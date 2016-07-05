#ifndef HUDLAYER_H
#define HUDLAYER_H

#include "cocos2d.h"
#include <utils/observer.h>
#include "ui/UILoadingBar.h"

namespace butcher {

class HudLog;

class HudLayer : public cocos2d::Layer
               , public Observer
{
public:
  HudLayer();
  ~HudLayer();

  virtual bool init();

  void print(const std::string& str,
           cocos2d::Color4B color = cocos2d::Color4B(177, 0, 177, 255));

  virtual void onNotify(Subject* subject);

  CREATE_FUNC(HudLayer);

private:
  int _logCount;
  HudLog* _log;
  cocos2d::ui::LoadingBar* _expBar;
  cocos2d::ui::LoadingBar* _hpBar;
  cocos2d::Label* _hpValue;
  cocos2d::Label* _lvValue;
  cocos2d::Label* _dungeonLevelLabel;

  void showMenu(cocos2d::Ref*);
  void showInventory(cocos2d::Ref*);

  void initExpBar();
  void initHpBar();
  void initDungeonLevelCounter();
};

}

#endif // HUDLAYER_H
