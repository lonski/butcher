#include "hud_layer.h"
#include <butcher.h>
#include <dungeon/tmx_builder.h>
#include <actors/player.h>
#include <utils/utils.h>
#include <dungeon/dungeon_state.h>
#include <view/loading_scene.h>
#include <dungeon/generators/maze_dungeon_generator.h>
#include <view/hud_log.h>
#include <actors/craftbook.h>
#include <utils/profiler.h>
#include <actors/actions/equip_action.h>
#include <actors/actions/pickup_action.h>

namespace cc = cocos2d;

namespace butcher {

HudLayer::HudLayer()
  : _margin(10)
  , _log(nullptr)
  , _expBar(nullptr)
  , _hpBar(nullptr)
  , _hpValue(nullptr)
  , _lvValue(nullptr)
  , _dungeonLevelLabel(nullptr)
  , _minimapSprite(nullptr)
  , _wpnSwitchButton(nullptr)
{
  _origin = cc::Director::getInstance()->getVisibleOrigin();
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

HudLayer::~HudLayer()
{
}

bool HudLayer::init()
{
  if (Layer::init())
  {
    removeAllChildren();

    initMenu();
    initHudLog();
    initHpBar();
    initExpBar();
    initDungeonLevelCounter();
    initMinimap();
    initQuickButtons();

    updateQuickWeaponSwitch();

    onNotify(BUTCHER.getPlayer().get(), EventType::Modified);
  }

  return true;
}

void HudLayer::initMenu()
{
  auto menuBtn = cc::MenuItemImage::create("images/btn_menu.png",
                                           "images/btn_menu_click.png",
                                           CC_CALLBACK_1(HudLayer::showMenu, this));
  menuBtn->setAnchorPoint(cc::Vec2(0,0));
  menuBtn->setPosition(cc::Vec2(_origin.x + _visibleSize.width - menuBtn->getBoundingBox().size.width - _margin, _origin.y + _margin));

  auto invBtn = cc::MenuItemImage::create("images/btn_inv.png",
                                           "images/btn_inv_click.png",
                                           CC_CALLBACK_1(HudLayer::showInventory, this));
  invBtn->setAnchorPoint(cc::Vec2(0,0));
  invBtn->setPosition(cc::Vec2(menuBtn->getPositionX() - menuBtn->getBoundingBox().size.width*1.1,menuBtn->getPositionY()));

  auto craftBtn = cc::MenuItemImage::create("images/btn_craft.png",
                                           "images/btn_craft_click.png",
                                           CC_CALLBACK_1(HudLayer::showCraftbook, this));
  craftBtn->setAnchorPoint(cc::Vec2(0,0));
  craftBtn->setPosition(cc::Vec2(invBtn->getPositionX() - invBtn->getBoundingBox().size.width*1.1,invBtn->getPositionY()));

  auto minimapBtn = cc::MenuItemImage::create("images/btn_minimap.png",
                                           "images/btn_minimap_click.png",
                                           CC_CALLBACK_1(HudLayer::showMinimap, this));
  minimapBtn->setAnchorPoint(cc::Vec2(0,0));
  minimapBtn->setPosition(cc::Vec2(craftBtn->getPositionX() - craftBtn->getBoundingBox().size.width*1.1,craftBtn->getPositionY()));

  auto menu = cc::Menu::create(menuBtn, invBtn, craftBtn, minimapBtn, NULL);
  menu->setPosition(cc::Vec2::ZERO);
  this->addChild(menu, 1);
}

void HudLayer::initHudLog()
{
  _log = new HudLog;
  addChild(_log, 1);
}

void HudLayer::initQuickButtons()
{
  _wpnSwitchButton = cc::MenuItemImage::create("images/wpn_switch_btn.png",
                                               "images/wpn_switch_btn_click.png",
                                               "images/wpn_switch_btn_disabled.png",
                                               CC_CALLBACK_1(HudLayer::weaponSwitch, this));
  _wpnSwitchButton->setAnchorPoint(cc::Vec2(1,0.5));
  _wpnSwitchButton->setPosition(cc::Vec2(0, _origin.y + 64));
  _wpnSwitchButton->setEnabled(false);

  //weapon sprite
   _wpnSwitchWpnSprite = cc::Sprite::create();
  _wpnSwitchWpnSprite->setScale(0.7);
  _wpnSwitchWpnSprite->setPosition(_wpnSwitchButton->getBoundingBox().size.width / 2, _wpnSwitchButton->getBoundingBox().size.height / 2);
  _wpnSwitchButton->addChild(_wpnSwitchWpnSprite);

  //arrows sprite
  _wpnSwitchArrowSprite = cc::Sprite::create();
  _wpnSwitchArrowSprite->initWithFile("images/wpn_switch_arrows.png");
  _wpnSwitchArrowSprite->setPosition(_wpnSwitchButton->getBoundingBox().size.width / 2, _wpnSwitchButton->getBoundingBox().size.height / 2 );
  _wpnSwitchButton->addChild(_wpnSwitchArrowSprite);

//  auto wpnSwitchBtn2 = cc::MenuItemImage::create("images/wpn_switch_btn.png",
//                                                "images/wpn_switch_btn_disabled.png",
//                                                CC_CALLBACK_1(HudLayer::weaponSwitch, this));
//  wpnSwitchBtn2->setAnchorPoint(cc::Vec2(1,0.5));
//  wpnSwitchBtn2->setPosition(cc::Vec2(0,wpnSwitchBtn->getPositionY() + wpnSwitchBtn->getBoundingBox().size.height*1.1));

//  auto wpnSwitchBtn3 = cc::MenuItemImage::create("images/wpn_switch_btn.png",
//                                                "images/wpn_switch_btn_disabled.png",
//                                                CC_CALLBACK_1(HudLayer::weaponSwitch, this));
//  wpnSwitchBtn3->setAnchorPoint(cc::Vec2(1,0.5));
//  wpnSwitchBtn3->setPosition(cc::Vec2(0,wpnSwitchBtn2->getPositionY() + wpnSwitchBtn2->getBoundingBox().size.height*1.1));

  auto menu = cc::Menu::create(_wpnSwitchButton, NULL);
  menu->setAnchorPoint(cc::Vec2(1,0));
  menu->setPosition(cc::Vec2(_origin.x + _visibleSize.width - _margin, 48));
  this->addChild(menu, 1);
}

void HudLayer::initExpBar()
{
  cc::Sprite* exp_glyph = cc::Sprite::create();
  exp_glyph->initWithFile("images/arrow_up.png");
  exp_glyph->setAnchorPoint(cc::Vec2(1,1));
  exp_glyph->setPosition(cc::Vec2(_origin.x + _visibleSize.width - _margin,
                              _origin.y + _visibleSize.height - _margin -
                              _hpBar->getBoundingBox().size.height*1.2));
  exp_glyph->setLocalZOrder(1);
  addChild(exp_glyph);

  _expBar = cc::ui::LoadingBar::create();
  _expBar->loadTexture("images/progress_bar_cyan.png");
  _expBar->setPercent(100);
  _expBar->setAnchorPoint(cc::Vec2(1,1));
  _expBar->setPosition(cc::Vec2(exp_glyph->getPositionX() - exp_glyph->getBoundingBox().size.width*1.2,
                                exp_glyph->getPositionY()));
  _expBar->setLocalZOrder(2);
  addChild(_expBar);

  cc::Sprite* barBg = cc::Sprite::create();
  barBg->initWithFile("images/progress_bar_bg.png");
  barBg->setAnchorPoint(cc::Vec2(1,1));
  barBg->setPosition(_expBar->getPosition());
  barBg->setLocalZOrder(1);
  addChild(barBg);

  _lvValue = make_label("", cc::Color4B::WHITE, 14);
  _lvValue->setPosition( barBg->getPositionX() - barBg->getBoundingBox().size.width / 2,
                      barBg->getPositionY() - barBg->getBoundingBox().size.height / 2);

  addChild(_lvValue, 3);
}

void HudLayer::initHpBar()
{
  cc::Sprite* hp_glyph = cc::Sprite::create();
  hp_glyph->initWithFile("images/heart.png");
  hp_glyph->setAnchorPoint(cc::Vec2(1,1));
  hp_glyph->setPosition(cc::Vec2(_origin.x + _visibleSize.width - _margin,
                              _origin.y + _visibleSize.height - _margin));
  hp_glyph->setLocalZOrder(1);
  addChild(hp_glyph);

  _hpBar = cc::ui::LoadingBar::create();
  _hpBar->loadTexture("images/progress_bar_red.png");
  _hpBar->setPercent(100);
  _hpBar->setAnchorPoint(cc::Vec2(1,1));
  _hpBar->setPosition(cc::Vec2(hp_glyph->getPositionX() - hp_glyph->getBoundingBox().size.width*1.2,
                                hp_glyph->getPositionY()));
  addChild(_hpBar, 2);

  cc::Sprite* barBg = cc::Sprite::create();
  barBg->initWithFile("images/progress_bar_bg.png");
  barBg->setAnchorPoint(cc::Vec2(1,1));
  barBg->setPosition(_hpBar->getPosition());
  addChild(barBg,1 );

  _hpValue = make_label("", cc::Color4B::WHITE, 14);
  _hpValue->setPosition( barBg->getPositionX() - barBg->getBoundingBox().size.width / 2,
                      barBg->getPositionY() - barBg->getBoundingBox().size.height / 2);

  addChild(_hpValue, 3);
}

void HudLayer::initDungeonLevelCounter()
{
  cc::Sprite* stairs_glyph = cc::Sprite::create();
  stairs_glyph->initWithFile("images/stairs_up_glyph.png");

  stairs_glyph->setAnchorPoint(cc::Vec2(1,1));
  stairs_glyph->setPosition(cc::Vec2(_origin.x + _visibleSize.width - _margin,
                                     _expBar->getPositionY() - _expBar->getBoundingBox().size.height*1.2));
  stairs_glyph->setLocalZOrder(1);
  addChild(stairs_glyph);

  _dungeonLevelLabel = make_label( toStr(BUTCHER.getDungeonLevel()),
                                   cc::Color4B(65,140,150,255),
                                   24,
                                   cc::Vec2(1.1,1.1) );

  _dungeonLevelLabel->setPosition(cc::Vec2(stairs_glyph->getPositionX() - stairs_glyph->getBoundingBox().size.width*1.2,
                                           stairs_glyph->getPositionY()));

  addChild(_dungeonLevelLabel, 1);
}

void HudLayer::print(const std::string &str, cc::Color4B color)
{
  _log->addMessage(str, color);

  _log->setAnchorPoint( cc::Vec2(0, 0) );
  _log->setPosition( _origin.x + 16, _origin.y + _visibleSize.height - 16 );
}

void HudLayer::updateExpBar(Player* player)
{
  if ( _expBar )
  {
    _expBar->setPercent((float)player->getExp() / (float)player->getExpForNextLevel() * 100);
    _lvValue->setString( "Level " + toStr(player->getLevel()) );
    //cc::log("%d %d", player->getExp(), player->getExpForNextLevel());
  }
}

void HudLayer::updateHpBar(Player* player)
{
  if ( _hpBar )
  {
    _hpBar->setPercent((float)player->getHp() / (float)player->getMaxHp() * 100);
    _hpValue->setString( toStr(player->getHp()) + "/" + toStr(player->getMaxHp()));
    //cc::log("hpBar%d %d", player->getHp(), player->getMaxHp());
  }
}

void HudLayer::initMinimap()
{
  if ( _minimapSprite )
    removeChild(_minimapSprite, false);

  _minimap.init();

  int margin = 10;
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  _minimapSprite = _minimap.generate();
  _minimapSprite->setPosition(origin.x + (visibleSize.width - margin) / 2,
                       origin.y + (visibleSize.height - margin) / 2);

  _minimapSprite = nullptr;
}

void HudLayer::updateQuickWeaponSwitch()
{
  std::shared_ptr<Player> player = BUTCHER.getPlayer();
  std::shared_ptr<Item> switchItem = player->getQuickSwitchWeapon();
  if ( switchItem )
  {
    _wpnSwitchButton->setEnabled(true);
    _wpnSwitchWpnSprite->initWithFile(switchItem->getSprite()->getResourceName());
  }
  else
  {
    _wpnSwitchButton->setEnabled(false);
    _wpnSwitchWpnSprite->init();
  }
}

void HudLayer::onNotify(Subject *subject, const EventData& event)
{
  Player* player = dynamic_cast<Player*>(subject);
  if ( player )
  {
    if ( event.id == EventType::LevelUP)
    {
      updateExpBar(player);
      updateHpBar(player);
      showMessage({"Congratulations!", "You advanced to level " + toStr(player->getLevel()) + "."}, cc::Color4B::WHITE, this);
    }
    else if ( event.id == EventType::Modified )
    {
      updateExpBar(player);
      updateHpBar(player);
    }
    else if ( event.id == EventType::Moved )
    {
      if ( _minimapSprite )
        updateMinimap();
    }
    else if ( event.id == EventType::ItemDestroyed )
    {
      showMessage({event.param}, cc::Color4B::RED, this);
    }
    else if ( event.id == EventType::PlayerDied )
    {      
      showMessage(explode(event.param, '|'), cc::Color4B::RED,this, [player](){
        player->setHp(player->getMaxHp() / 10);
      });
    }
    else if ( event.id == EventType::QuickSwitchWeaponChanged )
    {
      updateQuickWeaponSwitch();
    }
  }
}

void HudLayer::updateMinimap()
{
  _minimapSprite = _minimap.update();
}

void HudLayer::showMenu(Ref *)
{
  BUTCHER.showGameMenu(true);
}

void HudLayer::showInventory(cocos2d::Ref *)
{
  BUTCHER.showInventory();
}

void HudLayer::showCraftbook(cocos2d::Ref *)
{
  BUTCHER.showCraft();
}

void HudLayer::showMinimap(cocos2d::Ref *)
{
  if ( _minimapSprite )
  {
    removeChild(_minimapSprite, false);
    _minimapSprite = nullptr;
  }
  else
  {
    updateMinimap();
    addChild(_minimapSprite);
  }

//  BUTCHER.getPlayer()->performAction(
//        new PickUpAction(
//          AmountedItem( BUTCHER.actorsDatabase().createActor<Item>(ActorID::INTESTINES), 10)
//        ));

//  BUTCHER.getPlayer()->performAction(
//        new PickUpAction(
//          AmountedItem( BUTCHER.actorsDatabase().createActor<Item>(ActorID::BONE), 100)
//        ));

  //  BUTCHER.goToLevel(BUTCHER.getDungeonLevel()+1);
}

void HudLayer::weaponSwitch(cocos2d::Ref *)
{
  std::shared_ptr<Player> player = BUTCHER.getPlayer();

  ActorID itemId = player->getQuickSwitchWeaponID();
  if ( itemId != ActorID::INVALID )
  {
    AmountedItem eq = player->getInventory().equipped(ItemSlotType::WEAPON);
    ActorID currentlyEq = eq.item ? eq.item->getID() : ActorID::INVALID;

    bool performed = player->performAction( new EquipAction(itemId) );

    if ( currentlyEq != ActorID::INVALID && performed )
    {
      player->setQuickSwitchWeapon(currentlyEq);
    }
    else
    {
      _wpnSwitchButton->setEnabled(false);
      _wpnSwitchWpnSprite->init();
    }
  }
}

}
