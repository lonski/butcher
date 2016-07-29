#include "craft_view.h"

namespace cc = cocos2d;

namespace butcher {

CraftView::CraftView(std::shared_ptr<Player> player)
  : _player(player)
  , _margin(5.0f)
{
  _origin = cc::Director::getInstance()->getVisibleOrigin();
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

cocos2d::Scene *CraftView::createScene(std::shared_ptr<Player> player)
{
  cc::Scene* new_scene = cocos2d::Scene::create();
  auto cv = CraftView::create(player);

  new_scene->addChild(cv);
  return new_scene;
}

CraftView *CraftView::create(std::shared_ptr<Player> player)
{
  CraftView *pRet = new(std::nothrow) CraftView(player);
  if (pRet && pRet->init())
  {
      pRet->autorelease();
      return pRet;
  }
  else
  {
      delete pRet;
      pRet = nullptr;
      return nullptr;
  }
}

bool CraftView::init()
{
  if ( !cc::Layer::init() )
    return false;

  return true;
}

}
