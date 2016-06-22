#include "hud_layer.h"
#include <butcher.h>
#include <dungeon/tmx_builder.h>
#include <actors/player.h>
#include <utils/utils.h>
#include <dungeon/dungeon_state.h>
#include <view/loading_scene.h>
#include <dungeon/dungeon_generator.h>
#include <view/hud_log.h>

namespace cc = cocos2d;

namespace butcher {

HudLayer::HudLayer()
  : _log(nullptr)
{
}

HudLayer::~HudLayer()
{
}

bool HudLayer::init()
{
  if (Layer::init())
  {
    removeAllChildren();

    auto menuBtn = cc::MenuItemImage::create(  "button/menu.png",
                                           "button/menu_click.png",
                                           CC_CALLBACK_1(HudLayer::showMenu, this));

    cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
    auto visibleSize = cc::Director::getInstance()->getVisibleSize();

    int margin = 10;
    menuBtn->setAnchorPoint(cc::Vec2(0,0));
    menuBtn->setPosition(cc::Vec2(origin.x + visibleSize.width - menuBtn->getBoundingBox().size.width - margin, origin.y + margin));

    auto menu = cc::Menu::create(menuBtn, NULL);
    menu->setPosition(cc::Vec2::ZERO);
    this->addChild(menu, 1);

    _log = new HudLog;
    addChild(_log, 1);
  }

  return true;
}

void HudLayer::print(const std::string &str, cocos2d::Color4B color)
{
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  _log->addMessage(str, color);

  _log->setAnchorPoint( cc::Vec2(0, 0) );
  _log->setPosition( origin.x + 16, origin.y + visibleSize.height - 16 );
}

void HudLayer::showMenu(Ref *)
{
  LoadingScreen::run([&](){
    BUTCHER.goToLevel(BUTCHER.getDungeonLevel() + 1);
  }, "Generating level..");
}

}
