#include "loading_scene.h"
#include <butcher.h>
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

LoadingScreen::LoadingScreen()
{

}

bool LoadingScreen::init(const std::string& msg)
{
  if ( !Layer::init() )
    return false;

  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  auto loading = make_label(msg, cc::Color4B::ORANGE, 24);
  loading->setPosition(cc::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

  addChild(loading, 1);

  scheduleUpdate();

  return true;
}

void LoadingScreen::run(std::function<void ()> fun, const std::string &msg)
{
  auto layer = new LoadingScreen();

  if (layer->init(msg))
  {
    layer->autorelease();
    layer->_function = fun;

    auto scene = cc::Scene::create();
    scene->addChild(layer);
    cc::Director::getInstance()->replaceScene( scene );
  }
  else
  {
      delete layer;
  }
}

void LoadingScreen::update(float)
{
  _function();
}

}
