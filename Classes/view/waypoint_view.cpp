#include "waypoint_view.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/waypoints.h>
#include <view/loading_scene.h>

namespace cc = cocos2d;

namespace butcher {

WaypointView::WaypointView()
  : _margin(5.f)
{
  _origin = cc::Director::getInstance()->getVisibleOrigin();
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

cocos2d::Scene *WaypointView::createScene()
{
  cc::Scene* new_scene = cocos2d::Scene::create();
  auto cv = WaypointView::create();
  cv->addComponents();
  new_scene->addChild(cv);
  return new_scene;
}

WaypointView *WaypointView::create()
{
  WaypointView *pRet = new(std::nothrow) WaypointView();
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

bool WaypointView::init()
{
  if ( !cc::Layer::init() )
    return false;

  return true;
}

void WaypointView::addComponents()
{
  //layout
  cc::ui::Layout* waypointsLayout = cc::ui::Layout::create();
  waypointsLayout->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  waypointsLayout->setPosition(_origin);
  waypointsLayout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  waypointsLayout->setContentSize(cc::Size(_visibleSize.width, _visibleSize.height));
  waypointsLayout->setBackGroundImageScale9Enabled(true);
  waypointsLayout->setBackGroundImage("images/inv_border_fill.png");

  //close btn
  cc::ui::Button* closeBtn = cc::ui::Button::create();
  closeBtn->loadTextures("images/x_btn.png", "images/x_btn_click.png", "");
  closeBtn->setAnchorPoint(cc::Vec2(1,1));
  closeBtn->setPosition(cc::Vec2(_origin.x + _visibleSize.width, _origin.y + _visibleSize.height));
  closeBtn->addTouchEventListener([=](Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
      cc::Director::getInstance()->popScene();

  });

  //waypoints list
  cc::ui::ListView* waypointList = cc::ui::ListView::create();

  for ( int wpLevel :  BUTCHER.getPlayer()->getWaypoints() )
  {
    cc::ui::Button* btn = makeListItem(wpLevel);

    btn->addTouchEventListener([=](Ref*, cc::ui::Widget::TouchEventType type){
      if ( type == cc::ui::Widget::TouchEventType::ENDED )
        LoadingScreen::run([=](){
          BUTCHER.goToLevel(wpLevel, ActorID::WAYPOINT);
        }, "Teleporting..");
    });

    waypointList->pushBackCustomItem( btn );

  }

  waypointList->setItemsMargin(_margin);

  waypointList->setGravity(cc::ui::ListView::Gravity::CENTER_HORIZONTAL);
  waypointList->setContentSize(cc::Size(waypointsLayout->getContentSize().width - 4*_margin,
                                       waypointsLayout->getContentSize().height - 4*_margin));
  waypointList->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  waypointList->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, _margin*2, _margin*2));

  waypointsLayout->addChild(waypointList);

  addChild(waypointsLayout);
  addChild(closeBtn);
}

cocos2d::ui::Button *WaypointView::makeListItem(int level)
{
  cc::ui::Button* btn = cc::ui::Button::create();
  btn->loadTextures("images/item_btn.png", "images/item_btn_click.png", "images/button_disabled.png");

  cc::Sprite* sprite = cc::Sprite::create();
  sprite->initWithFile("images/portal.png");
  sprite->setAnchorPoint(cc::Vec2(-0.2,0.5));
  sprite->setPosition(_margin, btn->getBoundingBox().size.height / 2);
  btn->addChild(sprite);

  auto it = waypoints.find(level);

  cc::Label* label = make_label(it->second, cc::Color4B::WHITE, 22, cc::Vec2(0,-0.2));
  label->setPosition(sprite->getPositionX() + sprite->getBoundingBox().size.width * 1.4, btn->getBoundingBox().size.height / 2);
  btn->addChild(label);

  auto levelLabel = make_label("Dungeon level: " + toStr(level), cc::Color4B::GREEN, 16, cc::Vec2(0,0));
  levelLabel->setPosition(label->getPositionX(), label->getPositionY() - label->getBoundingBox().size.height*0.7);
  btn->addChild(levelLabel);

  return btn;
}


}
