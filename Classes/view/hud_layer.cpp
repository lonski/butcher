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

namespace cc = cocos2d;

namespace butcher {

HudLayer::HudLayer()
  : _log(nullptr)
  , _expBar(nullptr)
  , _hpBar(nullptr)
  , _hpValue(nullptr)
  , _lvValue(nullptr)
  , _dungeonLevelLabel(nullptr)
  , _minimapSprite(nullptr)
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

    cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
    auto visibleSize = cc::Director::getInstance()->getVisibleSize();
    int margin = 10;

    auto menuBtn = cc::MenuItemImage::create("images/btn_menu.png",
                                             "images/btn_menu_click.png",
                                             CC_CALLBACK_1(HudLayer::showMenu, this));
    menuBtn->setAnchorPoint(cc::Vec2(0,0));
    menuBtn->setPosition(cc::Vec2(origin.x + visibleSize.width - menuBtn->getBoundingBox().size.width - margin, origin.y + margin));

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

    _log = new HudLog;
    addChild(_log, 1);

    initHpBar();
    initExpBar();
    initDungeonLevelCounter();
    initMinimap();

    onNotify(BUTCHER.getPlayer().get(), EventType::Modified);
  }

  return true;
}

void HudLayer::initExpBar()
{
  int margin = 10;
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::Sprite* exp_glyph = cc::Sprite::create();
  exp_glyph->initWithFile("images/arrow_up.png");
  exp_glyph->setAnchorPoint(cc::Vec2(1,1));
  exp_glyph->setPosition(cc::Vec2(origin.x + visibleSize.width - margin,
                              origin.y + visibleSize.height - margin -
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
  int margin = 10;
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::Sprite* hp_glyph = cc::Sprite::create();
  hp_glyph->initWithFile("images/heart.png");
  hp_glyph->setAnchorPoint(cc::Vec2(1,1));
  hp_glyph->setPosition(cc::Vec2(origin.x + visibleSize.width - margin,
                              origin.y + visibleSize.height - margin));
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
  int margin = 10;
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::Sprite* stairs_glyph = cc::Sprite::create();
  stairs_glyph->initWithFile("images/stairs_up_glyph.png");

  stairs_glyph->setAnchorPoint(cc::Vec2(1,1));
  stairs_glyph->setPosition(cc::Vec2(origin.x + visibleSize.width - margin,
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
  cc::Vec2 origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  _log->addMessage(str, color);

  _log->setAnchorPoint( cc::Vec2(0, 0) );
  _log->setPosition( origin.x + 16, origin.y + visibleSize.height - 16 );
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

void HudLayer::onNotify(Subject *subject, EventType event)
{
  Player* player = dynamic_cast<Player*>(subject);
  if ( player )
  {
    if ( event == EventType::LevelUP)
    {
      updateExpBar(player);
      updateHpBar(player);
      showMessage({"Congratulations!", "You advanced to level " + toStr(player->getLevel()) + "."}, cc::Color4B::WHITE, this);
    }
    else if ( event == EventType::Modified )
    {
      updateExpBar(player);
      updateHpBar(player);
    }
    else if ( event == EventType::Moved )
    {
      if ( _minimapSprite )
        updateMinimap();
    }
    else if ( event == EventType::WeaponCracked )
    {
      showMessage({"Weapon cracked!"}, cc::Color4B::RED, this);
    }
  }
}

void HudLayer::updateMinimap()
{
  Profiler p;

  _minimapSprite = _minimap.update();

  p.log("updateMinimap");
}

void HudLayer::showMenu(Ref *)
{
  BUTCHER.showGameMenu();
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
}

}
