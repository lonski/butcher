#include "game_menu.h"
#include <butcher.h>

namespace cc = cocos2d;

namespace butcher {

GameMenu::GameMenu()
{
}

void GameMenu::newGame(Ref *)
{
  BUTCHER.startNewGame();
}

void GameMenu::quitGame(cocos2d::Ref *)
{
  cc::Director::getInstance()->end();
}

void GameMenu::backToGame(cocos2d::Ref *)
{
  cc::Director::getInstance()->popScene();
}

cocos2d::Scene *GameMenu::createScene(bool gameRunning)
{
  cc::Scene* new_scene = cocos2d::Scene::create();
  auto gm = GameMenu::create();
  gm->addComponents(gameRunning);
  new_scene->addChild(gm);
  return new_scene;
}

bool GameMenu::init()
{
  if ( !cc::Layer::init() )
    return false;

  return true;
}

void GameMenu::addComponents(bool gameRunning)
{
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::Sprite* logo = cc::Sprite::create();
  logo->initWithFile("images/logo.png");
  logo->setAnchorPoint(cc::Vec2(0.5,1));
  logo->setPosition( origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 20 );
  addChild(logo,1 );


  cc::MenuItemSprite* playBtn = nullptr;

  if ( gameRunning )
  {
    playBtn = cc::MenuItemImage::create("images/btn_back.png",
                                        "images/btn_back_click.png",
                                        CC_CALLBACK_1(GameMenu::backToGame, this));
  }
  else
  {
    playBtn = cc::MenuItemImage::create("images/btn_new_game.png",
                                        "images/btn_new_game_click.png",
                                        CC_CALLBACK_1(GameMenu::newGame, this));
  }

  auto quitBtn = cc::MenuItemImage::create("images/btn_quit.png",
                                           "images/btn_quit_click.png",
                                           CC_CALLBACK_1(GameMenu::quitGame, this));
  quitBtn->setPosition(0, playBtn->getPositionY() - playBtn->getBoundingBox().size.height*1.2);
  quitBtn->setAnchorPoint( cc::Vec2(0.5, 0.5) );

  auto menu = cc::Menu::create(playBtn, quitBtn, NULL);
  menu->setAnchorPoint(cc::Vec2(0.5,1));
  menu->setPosition( logo->getPositionX(), logo->getPositionY() - logo->getBoundingBox().size.height * 2 );
  this->addChild(menu, 1);
}

}
