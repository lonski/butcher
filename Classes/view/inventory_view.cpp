#include "inventory_view.h"
#include <actors/player.h>
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

InventoryView::InventoryView(std::shared_ptr<Player> player)
  : _player(player)
  , _margin(5.0f)
  , _root(nullptr)
  , _topPanel(nullptr)
  , _topLeftPanel(nullptr)
  , _topRightPanel(nullptr)
  , _bottomPanel(nullptr)
  , _itemList(nullptr)
  , _wpnSlot(nullptr)
  , _armorSlot(nullptr)
  , _glovesSlot(nullptr)
  , _bootsSlot(nullptr)
{
  _origin = cc::Director::getInstance()->getVisibleOrigin();
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

cocos2d::Scene *InventoryView::createScene(std::shared_ptr<Player> player)
{
  cc::Scene* new_scene = cocos2d::Scene::create();
  auto iv = InventoryView::create(player);
  iv->addComponents();
  new_scene->addChild(iv);
  return new_scene;
}

InventoryView *InventoryView::create(std::shared_ptr<Player> player)
{
  InventoryView *pRet = new(std::nothrow) InventoryView(player);
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

bool InventoryView::init()
{
  if ( !cc::Layer::init() )
    return false;

  return true;
}

void InventoryView::createRootLayout()
{
  _root = cc::ui::Layout::create();
  addChild(_root);
  _root->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  _root->setPosition(_origin);
  _root->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _root->setBackGroundImageScale9Enabled(true);
  _root->setContentSize(cc::Size(_visibleSize.width, _visibleSize.height));
}

void InventoryView::createTopPanel()
{
  _topPanel = cc::ui::Layout::create();
  _root->addChild(_topPanel);
  _topPanel->setLayoutType(cc::ui::Layout::Type::HORIZONTAL);
  _topPanel->setContentSize(cc::Size(_visibleSize.width, _visibleSize.height / 3));
  _topPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  _topPanel->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin, _margin, _margin, 0));
}

void InventoryView::createTopLeftPanel()
{
  _topLeftPanel = cc::ui::Layout::create();
  _topPanel->addChild(_topLeftPanel);

  cc::ui::LinearLayoutParameter* lpTL = cc::ui::LinearLayoutParameter::create();
  _topLeftPanel->setLayoutParameter(lpTL);
  lpTL->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::LEFT);
  lpTL->setMargin(cc::ui::Margin(0, 0, _margin, 0));

  _topLeftPanel->setContentSize( cc::Size(_topPanel->getContentSize().width / 2 - _margin,
                                         _topPanel->getContentSize().height) );

  _topLeftPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _topLeftPanel->setBackGroundImageScale9Enabled(true);
  _topLeftPanel->setBackGroundImage("images/inv_border.png");
}

void InventoryView::createTopRightPanel()
{
  _topRightPanel = cc::ui::Layout::create();
  _topPanel->addChild(_topRightPanel);

  cc::ui::LinearLayoutParameter* lpTR = cc::ui::LinearLayoutParameter::create();
  _topRightPanel->setLayoutParameter(lpTR);
  lpTR->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::RIGHT);
  lpTR->setMargin(cc::ui::Margin(0, 0, 0, 0));

  _topRightPanel->setContentSize( cc::Size(_topPanel->getContentSize().width / 2 - 2*_margin,
                                         _topPanel->getContentSize().height) );

  _topRightPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _topRightPanel->setBackGroundImageScale9Enabled(true);
  _topRightPanel->setBackGroundImage("images/inv_border.png");
}

void InventoryView::createBottomPanel()
{
  _bottomPanel = cc::ui::Layout::create();
  _root->addChild(_bottomPanel);

  cc::ui::LinearLayoutParameter* lp1 = cc::ui::LinearLayoutParameter::create();
  _bottomPanel->setLayoutParameter(lp1);
  lp1->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lp1->setMargin(cc::ui::Margin(_margin, _margin, _margin, _margin));

  _bottomPanel->setContentSize(cc::Size(_visibleSize.width - 2*_margin, _visibleSize.height / 3 * 2 - 2*_margin));
  _bottomPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _bottomPanel->setBackGroundImageScale9Enabled(true);
  _bottomPanel->setBackGroundImage("images/inv_border.png");
}

void InventoryView::fillCharacterInfo()
{
  cc::ui::Layout* l = cc::ui::Layout::create();

  cc::Size lSize;
  lSize.height = 0;
  lSize.width = 0;

  _topRightPanel->removeAllChildren();

  cc::ui::LinearLayoutParameter* lp1 = cc::ui::LinearLayoutParameter::create();
  lp1->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::LEFT);
  lp1->setMargin(cc::ui::Margin(_margin, _margin, _margin, _margin));

  cc::Label* levelLabel = make_label("Level: " + toStr(_player->getLevel()), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  lSize.height = levelLabel->getBoundingBox().size.height * 1.2 * 6;
  lSize.width = levelLabel->getBoundingBox().size.width * 3;
  levelLabel->setPosition(0, lSize.height - levelLabel->getBoundingBox().size.height*(1.2/2));
  l->addChild(levelLabel);

  cc::Label* hpLabel = make_label("HP: " + toStr(_player->getHp()) + "/" + toStr(_player->getMaxHp()), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  hpLabel->setPosition(0, levelLabel->getPositionY() - levelLabel->getBoundingBox().size.height * 1.2);
  l->addChild(hpLabel);

  cc::Label* attackLabel = make_label("Attack: " + toStr(_player->getAttribute(AttributeType::Attack)), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  attackLabel->setPosition(0, hpLabel->getPositionY() - hpLabel->getBoundingBox().size.height * 1.2);
  l->addChild(attackLabel);

  cc::Label* defLabel = make_label("Defense: " + toStr(_player->getAttribute(AttributeType::Defense)), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  defLabel->setPosition(0, attackLabel->getPositionY() - attackLabel->getBoundingBox().size.height * 1.2);
  l->addChild(defLabel);

  cc::Label* drLabel = make_label("DR: " + toStr(_player->getAttribute(AttributeType::DamageReduction)), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  drLabel->setPosition(0, defLabel->getPositionY() - defLabel->getBoundingBox().size.height * 1.2);
  l->addChild(drLabel);

  cc::Label* dmgLabel = make_label("Damage: " + _player->getDamage().toString(), cc::Color4B::WHITE, 18, cc::Vec2(0,0.5));
  dmgLabel->setPosition(0, drLabel->getPositionY() - drLabel->getBoundingBox().size.height * 1.2);
  l->addChild(dmgLabel);

  l->setContentSize(lSize);
  l->setAnchorPoint(cc::Vec2(0, 0.5));
  l->setPosition(cc::Vec2(_margin * 2, _topRightPanel->getContentSize().height / 2));
  _topRightPanel->addChild(l);
}

cocos2d::ui::Button * InventoryView::makeListItem(const std::string& title, const std::string& sprite_fn)
{
  cc::ui::Button* btn = cc::ui::Button::create();
  btn->loadTextures("images/item_btn.png", "images/item_btn_click.png", "");

  cc::Sprite* sprite = cc::Sprite::create();
  sprite->initWithFile(sprite_fn);
  sprite->setAnchorPoint(cc::Vec2(-0.2,0.5));
  sprite->setPosition(_margin, btn->getBoundingBox().size.height / 2);
  btn->addChild(sprite);

  cc::Label* label = make_label(title, cc::Color4B::WHITE, 22, cc::Vec2(0,0.5));
  label->setPosition(sprite->getBoundingBox().size.width * 1.4, btn->getBoundingBox().size.height / 2);
  btn->addChild(label);

  return btn;
}

void InventoryView::fillInventoryItems()
{
  _bottomPanel->removeChild(_itemList);
  _itemList = cc::ui::ListView::create();

  for ( auto& kv :  _player->getInventory().getItems() )
  {
    AmountedItem i = kv.second;

    std::string title = i.item->getName();
    if ( i.amount > 1 )
      title += " (" + toStr(i.amount) + ")";

    cc::ui::Button* btn = makeListItem(title, i.item->getSprite()->getResourceName());
    btn->addTouchEventListener([this,i](Ref*, cc::ui::Widget::TouchEventType type){
      if ( type == cc::ui::Widget::TouchEventType::ENDED )
        chooseItemAction(i);
    });
    _itemList->pushBackCustomItem( btn );
  }

  _itemList->setItemsMargin(_margin);

  _itemList->setGravity(cc::ui::ListView::Gravity::CENTER_HORIZONTAL);
  _itemList->setSize(cc::Size(_bottomPanel->getContentSize().width - 4*_margin,
                             _bottomPanel->getContentSize().height - 6*_margin));
  _itemList->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _itemList->setPosition(cc::Vec2(_margin*2, _margin*3));

  _bottomPanel->addChild(_itemList);
}

void InventoryView::createBodySlots()
{
  cc::ui::Layout* l = cc::ui::Layout::create();

  cc::Size lSize;
  lSize.height = 0;
  lSize.width = 0;

  _wpnSlot = cc::ui::Button::create();
  _wpnSlot->loadTextures("images/inv_body_slot.png", "images/inv_body_slot_click.png", "images/inv_body_slot_wpn_disabled.png");
  _wpnSlot->setAnchorPoint(cc::Vec2(0,-1));
  //wpnSlot->setPosition(cc::Vec2(_margin*2, _topLeftPanel->getContentSize().height - _margin*2));
  _wpnSlot->setPosition(cc::Vec2(0, _margin));
  _wpnSlot->setEnabled(false);
  //_topLeftPanel->addChild(wpnSlot);
  lSize.height += _wpnSlot->getBoundingBox().size.height;
  lSize.width += _wpnSlot->getBoundingBox().size.width;
  l->addChild(_wpnSlot);

  _armorSlot = cc::ui::Button::create();
  _armorSlot->loadTextures("images/inv_body_slot.png", "images/inv_body_slot_click.png", "images/inv_body_slot_armor_disabled.png");
  _armorSlot->setAnchorPoint(cc::Vec2(0,-1));
  _armorSlot->setPosition(cc::Vec2(_wpnSlot->getPositionX() + _wpnSlot->getBoundingBox().size.width + _margin,
                                  _wpnSlot->getPositionY()));
  _armorSlot->setEnabled(false);
  lSize.width += _armorSlot->getBoundingBox().size.width + _margin;
  l->addChild(_armorSlot);

  _glovesSlot = cc::ui::Button::create();
  _glovesSlot->loadTextures("images/inv_body_slot.png", "images/inv_body_slot_click.png", "images/inv_body_slot_glv_disabled.png");
  _glovesSlot->setAnchorPoint(cc::Vec2(0,-1));
  _glovesSlot->setPosition(cc::Vec2(_wpnSlot->getPositionX(),
                                   _wpnSlot->getPositionY() - _wpnSlot->getBoundingBox().size.height - _margin));
  _glovesSlot->setEnabled(false);
  lSize.height += _glovesSlot->getBoundingBox().size.height + _margin;
  l->addChild(_glovesSlot);

  _bootsSlot = cc::ui::Button::create();
  _bootsSlot->loadTextures("images/inv_body_slot.png", "images/inv_body_slot_click.png", "images/inv_body_slot_boots_disabled.png");
  _bootsSlot->setAnchorPoint(cc::Vec2(0,-1));
  _bootsSlot->setPosition(cc::Vec2(_glovesSlot->getPositionX() + _glovesSlot->getBoundingBox().size.width + _margin,
                                  _glovesSlot->getPositionY()));
  _bootsSlot->setEnabled(false);
  l->addChild(_bootsSlot);

  l->setContentSize(lSize);
  l->setAnchorPoint(cc::Vec2(0.5, 0.5));
  l->setPosition(cc::Vec2(_topLeftPanel->getContentSize().width / 2, _topLeftPanel->getContentSize().height / 2));
//  l->setBackGroundColor(cc::Color3B::WHITE);
//  l->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::SOLID);
  _topLeftPanel->addChild(l);
}

void InventoryView::chooseItemAction(const AmountedItem &item)
{
  cc::ui::Layout* layout = cc::ui::Layout::create();
  layout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  layout->setBackGroundImageScale9Enabled(true);
  layout->setBackGroundImage("images/inv_border_fill.png");
  cc::Size size;
  size.width = _margin * 4;
  size.height = _margin * 4;

  //DESTROY
  cc::ui::Button* destroyBtn = cc::ui::Button::create();
  destroyBtn->loadTextures("images/button_red.png", "images/button_red_click.png", "");
  destroyBtn->setTitleFontName("fonts/Marker Felt.ttf");
  destroyBtn->setTitleText("Destroy");
  destroyBtn->setTitleFontSize(22);
  destroyBtn->setAnchorPoint(cc::Vec2(0,0));
  destroyBtn->setPosition(cc::Vec2(_margin*2, _margin*2));
  destroyBtn->addTouchEventListener([=](Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
    {
      _player->getInventory().removeItem(item);
      _bottomPanel->removeChild(layout);
      fillInventoryItems();
    }
  });
  size.width += destroyBtn->getBoundingBox().size.width;
  size.height += destroyBtn->getBoundingBox().size.height;
  layout->addChild(destroyBtn);

  float posY = destroyBtn->getPositionY() + destroyBtn->getBoundingBox().size.height + _margin;

  if ( item.item->isUsable() )
  {
    //USE
    cc::ui::Button* useBtn = cc::ui::Button::create();
    useBtn->loadTextures("images/button_orange.png", "images/button_orange_click.png", "");
    useBtn->setTitleFontName("fonts/Marker Felt.ttf");
    useBtn->setTitleText("Use");
    useBtn->setTitleFontSize(22);
    useBtn->setAnchorPoint(cc::Vec2(0,0));
    useBtn->setPosition(cc::Vec2(_margin*2, posY));
    size.height += useBtn->getBoundingBox().size.height + _margin;
    layout->addChild(useBtn);

    posY += useBtn->getBoundingBox().size.height + _margin;
  }

  if ( item.item->getItemSlotType() != ItemSlotType::NONE )
  {
    //EQUIP
    cc::ui::Button* eqBtn = cc::ui::Button::create();
    eqBtn->loadTextures("images/button_orange.png", "images/button_orange_click.png", "");
    eqBtn->setTitleFontName("fonts/Marker Felt.ttf");
    eqBtn->setTitleText("Equip");
    eqBtn->setTitleFontSize(22);
    eqBtn->setAnchorPoint(cc::Vec2(0,0));
    eqBtn->setPosition(cc::Vec2(_margin*2, posY));
    size.height += eqBtn->getBoundingBox().size.height + _margin;
    layout->addChild(eqBtn);
  }

  layout->setContentSize(size);
  layout->setAnchorPoint(cc::Vec2(0.5, 0.5));
  layout->setPosition(cc::Vec2(_bottomPanel->getContentSize().width / 2, _bottomPanel->getContentSize().height / 2));

  cc::ui::Button* closeBtn = cc::ui::Button::create();
  closeBtn->loadTextures("images/x_btn.png", "images/x_btn_click.png", "");
  closeBtn->setAnchorPoint(cc::Vec2(0.5,0.5));
  closeBtn->setPosition(cc::Vec2(size.width, size.height));
  closeBtn->addTouchEventListener([=](Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
    {
      _bottomPanel->removeChild(layout);
      _itemList->setEnabled(true);
    }
  });
  layout->addChild(closeBtn);

  _itemList->setEnabled(false);
  _bottomPanel->addChild(layout);
}

void InventoryView::createCloseButton()
{
  cc::ui::Button* closeBtn = cc::ui::Button::create();
  closeBtn->loadTextures("images/x_btn.png", "images/x_btn_click.png", "");
  closeBtn->setAnchorPoint(cc::Vec2(1,1));
  closeBtn->setPosition(cc::Vec2(_origin.x + _visibleSize.width, _origin.y + _visibleSize.height));
  closeBtn->addTouchEventListener([=](Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
      cc::Director::getInstance()->popScene();
  });
  addChild(closeBtn);
}

void InventoryView::addComponents()
{
  createRootLayout();
  createTopPanel();
  createTopLeftPanel();
  createTopRightPanel();
  createBottomPanel();

  fillCharacterInfo();
  fillInventoryItems();
  createBodySlots();

  createCloseButton();

}

}
