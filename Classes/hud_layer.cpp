#include "hud_layer.h"
#include <butcher.h>
#include <gui/fade_text.h>
#include <actors/player.h>

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
  //log("go to prev level");
  // Butcher::getInstance().goToLevel(Butcher::getInstance().dungeonLevel() - 1);
  //log("went to level %d", Butcher::getInstance().dungeonLevel());
  //FadeText text;
  //text.run("Lol", BUTCHER.getPlayer()->getPosition());

  //Label* label = Label::createWithTTF("trololo", "fonts/arial.ttf", 12);


  //label->runAction( FadeOut::create(3) );

  //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
  //label->setPosition( BUTCHER.getPlayer()->getPosition() );

  Label* label = Label::createWithTTF("-10", "fonts/Marker Felt.ttf", 16);
  label->setTextColor(Color4B::RED);
  label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
  label->setPosition(32, 64);

  //label->setPosition( Vec2(origin.x + Director::getInstance()->getVisibleSize().width - menuBtn2->getBoundingBox().size.width * 2 - margin, origin.y + margin) );

  BUTCHER.getPlayer()->sprite()->addChild(label, 1);

  label->runAction( MoveBy::create(0.5, Vec2(0,12)) );
  label->runAction( FadeOut::create(0.5) );

}

}
