#ifndef HUDLAYER_H
#define HUDLAYER_H

#include "cocos2d.h"
#include <utils/observer.h>
#include "ui/UILoadingBar.h"
#include <view/minimap.h>

namespace butcher {

class HudLog;
class Player;

class HudLayer : public cocos2d::Layer
               , public Observer
{
public:
  HudLayer();
  ~HudLayer();

  virtual bool init();

  void print(const std::string& str,
           cocos2d::Color4B color = cocos2d::Color4B(177, 0, 177, 255));

  virtual void onNotify(Subject* subject, const EventData& event);
  void updateMinimap();

  CREATE_FUNC(HudLayer);

private:
  int _margin;
  cocos2d::Vec2 _origin;
  cocos2d::Size _visibleSize;

  int _logCount;
  HudLog* _log;
  cocos2d::ui::LoadingBar* _expBar;
  cocos2d::ui::LoadingBar* _hpBar;
  cocos2d::Label* _hpValue;
  cocos2d::Label* _lvValue;
  cocos2d::Label* _dungeonLevelLabel;
  Minimap _minimap;
  cocos2d::Node* _minimapSprite;
  cocos2d::MenuItemImage* _wpnSwitchButton;
  cocos2d::Sprite* _wpnSwitchWpnSprite;
  cocos2d::Sprite* _wpnSwitchArrowSprite;

  void showMenu(cocos2d::Ref*);
  void showInventory(cocos2d::Ref*);
  void showCraftbook(cocos2d::Ref*);
  void showMinimap(cocos2d::Ref*);

  void weaponSwitch(cocos2d::Ref*);

  void initExpBar();
  void initHpBar();
  void initDungeonLevelCounter();
  void updateExpBar(Player* player);
  void updateHpBar(Player* player);
  void initMinimap();
  void initMenu();
  void initHudLog();
  void initQuickButtons();
  void updateQuickWeaponSwitch();
};

}

#endif // HUDLAYER_H
