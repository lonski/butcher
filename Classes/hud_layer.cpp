#include "hud_layer.h"
#include <butcher.h>

using namespace cocos2d;

namespace butcher {

HudLayer::HudLayer()
{

}

bool HudLayer::init()
{
    if (Layer::init())
    {

        auto menuBtn = MenuItemImage::create(  "button/menu.png",
                                               "button/menu_click.png",
                                               CC_CALLBACK_1(HudLayer::showMenu, this));

        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        int margin = 10;
        menuBtn->setAnchorPoint(Vec2(0,0));
        menuBtn->setPosition(Vec2(origin.x + Director::getInstance()->getVisibleSize().width - menuBtn->getBoundingBox().size.width - margin, origin.y + margin));

        auto menu = Menu::create(menuBtn, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        auto menuBtn2 = MenuItemImage::create(  "button/menu.png",
                                               "button/menu_click.png",
                                               CC_CALLBACK_1(HudLayer::showMenu2, this));

        menuBtn2->setAnchorPoint(Vec2(0,0));
        menuBtn2->setPosition(Vec2(origin.x + Director::getInstance()->getVisibleSize().width - menuBtn2->getBoundingBox().size.width * 2 - margin, origin.y + margin));

        auto menu2 = Menu::create(menuBtn2, NULL);
        menu2->setPosition(Vec2::ZERO);
        this->addChild(menu2, 1);
    }

    return true;
}

void HudLayer::showMenu(Ref *)
{
    log("go to next level");
    Butcher::getInstance().goToNextLevel();
    log("went to level %d", Butcher::getInstance().dungeonLevel());
}

void HudLayer::showMenu2(Ref *)
{
  log("go to prev level");
  Butcher::getInstance().goToLevel(Butcher::getInstance().dungeonLevel() - 1);
  log("went to level %d", Butcher::getInstance().dungeonLevel());
}

}
