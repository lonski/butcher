#include "craft_view.h"
#include <actors/player.h>
#include <utils/utils.h>
#include "ui/UIText.h"
#include <butcher.h>
#include <data/actors_database.h>

namespace cc = cocos2d;

namespace butcher {

CraftView::CraftView(std::shared_ptr<Player> player)
  : _player(player)
  , _margin(5.0f)
  , _root(nullptr)
  , _craftInfoPanel(nullptr)
  , _recipesPanel(nullptr)
  , _workbenchPanel(nullptr)
  , _workbenchItemPanel(nullptr)
  , _recipeList(nullptr)
  , _selectedCraft(CraftType::NONE)
  , _smithingBtn(nullptr)
  , _tanningBtn(nullptr)
  , _cookingBtn(nullptr)
  , _engineeringBtn(nullptr)
  , _craftLevelBar(nullptr)
  , _craftLevelLabel(nullptr)
  , _craftPointsLabel(nullptr)
  , _learnBtn(nullptr)
{
  _origin = cc::Director::getInstance()->getVisibleOrigin();
  _visibleSize = cc::Director::getInstance()->getVisibleSize();
}

cocos2d::Scene *CraftView::createScene(std::shared_ptr<Player> player)
{
  cc::Scene* new_scene = cocos2d::Scene::create();
  auto cv = CraftView::create(player);
  cv->addComponents();
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

void CraftView::addComponents()
{
  createRootLayout();
  createCloseButton();
  createCraftInfoPanel();
  createWorkbenchPanel();
  createRecipesPanel();

  _root->addChild(_craftInfoPanel);
  _root->addChild(_recipesPanel);
  _root->addChild(_workbenchPanel);

  fillRecipeList();
  fillCraftInfoPanel();

  craftChooseSmithing(nullptr);
}

void CraftView::createRootLayout()
{
  _root = cc::ui::Layout::create();
  addChild(_root);
  _root->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  _root->setPosition(_origin);
  _root->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _root->setContentSize(cc::Size(_visibleSize.width, _visibleSize.height));
}

void CraftView::createCloseButton()
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

void CraftView::createCraftInfoPanel()
{
  _craftInfoPanel = cc::ui::Layout::create();
  _craftInfoPanel->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  _craftInfoPanel->setContentSize(cc::Size(_visibleSize.width - _margin*2, _visibleSize.height / 3));
  _craftInfoPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _craftInfoPanel->setBackGroundImageScale9Enabled(true);
  _craftInfoPanel->setBackGroundImage("images/inv_border.png");

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  _craftInfoPanel->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin, _margin, _margin, 0));

  int height = 0;
  height += createCraftMenuButtons() + _margin*4;
  height += createCraftLevelInfo() + _margin*4;
  height += createCraftPointsInfo() + _margin*4;

  _craftInfoPanel->setContentSize(cc::Size(_visibleSize.width - _margin*2, height));
}

void CraftView::createWorkbenchPanel()
{
  _workbenchPanel = cc::ui::Layout::create();
  _workbenchPanel->setLayoutType(cc::ui::Layout::Type::HORIZONTAL);
  _workbenchPanel->setContentSize(cc::Size(_visibleSize.width - _margin*2, _visibleSize.height / 4));
  _workbenchPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _workbenchPanel->setBackGroundImageScale9Enabled(true);
  _workbenchPanel->setBackGroundImage("images/inv_border.png");

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  _workbenchPanel->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin, _margin, _margin, 0));
}

void CraftView::createRecipesPanel()
{
  _recipesPanel = cc::ui::Layout::create();
  _recipesPanel->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  _recipesPanel->setContentSize(cc::Size(_visibleSize.width - _margin*2, _visibleSize.height
                                         - _craftInfoPanel->getBoundingBox().size.height
                                         - _workbenchPanel->getBoundingBox().size.height
                                         - 4*_margin));
  _recipesPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  _recipesPanel->setBackGroundImageScale9Enabled(true);
  _recipesPanel->setBackGroundImage("images/inv_border.png");

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  _recipesPanel->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin, _margin, _margin, 0));
}

cocos2d::ui::Button * CraftView::makeListItem(const std::string& title, const std::string& sprite_fn)
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

cocos2d::ui::Button * CraftView::makeIngridientListItem(const std::string& title, const std::string& sprite_fn, cc::Color4B color)
{
  cc::ui::Button* btn = cc::ui::Button::create("","","");
  btn->setScale9Enabled(true);
  //btn->loadTextures("images/item_btn.png", "images/item_btn_click.png", "");

  cc::Sprite* sprite = cc::Sprite::create();
  sprite->setScale(0.5);
  sprite->initWithFile(sprite_fn);
  sprite->setAnchorPoint(cc::Vec2(-0.2,0.5));
  sprite->setPosition(_margin, btn->getBoundingBox().size.height / 2);
  btn->addChild(sprite);

  cc::Label* label = make_label(title, color, 18, cc::Vec2(0,0.5));
  label->setPosition(sprite->getBoundingBox().size.width * 1.4, btn->getBoundingBox().size.height / 2);
  btn->addChild(label);

  btn->setContentSize(cc::Size(sprite->getBoundingBox().size.width * 1.4 + label->getBoundingBox().size.width,
                               sprite->getBoundingBox().size.height));
  return btn;
}

void CraftView::fillRecipeList()
{
  _recipesPanel->removeChild(_recipeList);
  _recipeList = cc::ui::ListView::create();

  for ( std::shared_ptr<Recipe> rec :  _player->getCraftbook().getRecipes() )
  {
    if ( rec->getType() == _selectedCraft )
    {
      AmountedItem i = rec->getProduct();

      cc::ui::Button* btn = makeListItem(i.item->getName(), i.item->getSprite()->getResourceName());
      btn->addTouchEventListener([this,rec](Ref*, cc::ui::Widget::TouchEventType type){
        if ( type == cc::ui::Widget::TouchEventType::ENDED )
        {
          _selectedRecipe = rec;
          _selectedRecipe->setInventory(&_player->getInventory());
          fillWorkbench();
        }
      });
      _recipeList->pushBackCustomItem( btn );
    }
  }

  _recipeList->setItemsMargin(_margin);

  _recipeList->setGravity(cc::ui::ListView::Gravity::CENTER_HORIZONTAL);
  _recipeList->setContentSize(cc::Size(_recipesPanel->getContentSize().width - 4*_margin,
                                       _recipesPanel->getContentSize().height - 4*_margin));
  _recipeList->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  _recipeList->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, _margin*2, _margin*2));

  _recipesPanel->addChild(_recipeList);
}

void CraftView::fillWorkbench()
{
  _workbenchPanel->removeAllChildren();
  if ( _selectedRecipe )
  {
    fillWorkbenchItem();
    fillWorkbenchIngridients();
  }
}

void CraftView::fillWorkbenchItem()
{
  if ( _selectedRecipe )
  {
    cc::Size size;
    size.height = _workbenchPanel->getBoundingBox().size.height - 4*_margin;
    size.width = (_workbenchPanel->getBoundingBox().size.width - 4*_margin) / 2;

    _workbenchItemPanel = cc::ui::Layout::create();
    _workbenchItemPanel->setLayoutType(cc::ui::Layout::Type::VERTICAL);
    _workbenchItemPanel->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
    _workbenchItemPanel->setPosition(cc::Vec2(_margin,_margin));
    _workbenchItemPanel->setContentSize(size);

    cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
    _workbenchItemPanel->setLayoutParameter(lpT);
    lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::TOP);
    lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, _margin*2, _margin*2));

    //item info
    cc::ui::ListView* itemInfoList = cc::ui::ListView::create();
    itemInfoList->setPosition(cc::Vec2(0,size.height));
    itemInfoList->setAnchorPoint(cc::Vec2(0,1));
    itemInfoList->setContentSize(cc::Size(size.width, size.height - 48));
    itemInfoList->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
    //itemInfoList->setBackGroundColor(cc::Color3B::WHITE);
    _workbenchItemPanel->addChild(itemInfoList);

    //Add item icon
    cc::ui::Layout* itemIcon = cc::ui::Layout::create();

    cc::Sprite* sprite = cc::Sprite::create();
    sprite->initWithFile(_selectedRecipe->getProduct().item->getSprite()->getResourceName());
    sprite->setAnchorPoint(cc::Vec2(0,0));
    itemIcon->addChild(sprite);

    itemIcon->setContentSize(cc::Size(itemInfoList->getBoundingBox().size.width - 2*_margin,
                                      sprite->getBoundingBox().size.height));

    cc::Label* label = make_label(_selectedRecipe->getProduct().item->getName(),cc::Color4B::WHITE, 22,cc::Vec2(0,0.5));
    label->setPosition(sprite->getPositionX() + sprite->getBoundingBox().size.width*1.2,
                       itemIcon->getContentSize().height / 2);
    label->setAlignment(cc::TextHAlignment::LEFT);
    itemIcon->addChild(label);

    itemIcon->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
    itemInfoList->pushBackCustomItem(itemIcon);

    //add item info
    for ( auto str : _selectedRecipe->getProduct().item->getItemInfo() )
    {
      cc::ui::Layout* itemText = cc::ui::Layout::create();
      cc::Label* label = make_label(str, cc::Color4B::WHITE, 16, cc::Vec2(0,0));
      label->setPosition(cc::Vec2(sprite->getBoundingBox().size.width*1.2,0));
      itemText->addChild(label);
      itemText->setContentSize(cc::Size(itemInfoList->getBoundingBox().size.width - 2*_margin,
                                        label->getBoundingBox().size.height));
      itemText->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
      itemInfoList->pushBackCustomItem(itemText);
    }

    //add craft button
    cc::ui::Button* craftBtn = cc::ui::Button::create("images/button_green.png",
                                                      "images/button_green_click.png",
                                                      "images/button_disabled.png");
    craftBtn->setTitleText("Craft");
    craftBtn->setTitleColor(cc::Color3B::WHITE);
    craftBtn->setTitleFontSize(22);
    craftBtn->setTitleFontName("fonts/Marker Felt.ttf");
    craftBtn->setContentSize(cc::Size(itemInfoList->getBoundingBox().size.width, 48));
    craftBtn->setScale9Enabled(true);
    craftBtn->setEnabled(_selectedRecipe->checkIngridients());
    craftBtn->addTouchEventListener([this](Ref*, cc::ui::Widget::TouchEventType type){
      if ( type == cc::ui::Widget::TouchEventType::ENDED )
      {
        if ( !_selectedRecipe->produce() )
          showMessage("Failed to create item!", cc::Color4B::RED, this);
        else
          showMessage("Item created successfully!", cc::Color4B::GREEN, this);

        fillWorkbench();
      }
    });
    _workbenchItemPanel->addChild(craftBtn);

    _workbenchPanel->addChild(_workbenchItemPanel);
  }
}

void CraftView::fillWorkbenchIngridients()
{
  cc::Size size;
  size.height = _workbenchPanel->getBoundingBox().size.height - 4*_margin;
  size.width = (_workbenchPanel->getBoundingBox().size.width) / 2 - 6*_margin;

  cc::ui::ListView* ingridientsList = cc::ui::ListView::create();
  //ingridientsList->setPosition(cc::Vec2(0,size.height));
  //ingridientsList->setAnchorPoint(cc::Vec2(0,1));
  ingridientsList->setContentSize(cc::Size(size.width, size.height));
  ingridientsList->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  ingridientsList->setBackGroundColor(cc::Color3B::WHITE);

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  ingridientsList->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::TOP);
  lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, _margin*2, _margin*2));

  _workbenchPanel->addChild(ingridientsList);

  //add ingridients
  for ( auto& kv : _selectedRecipe->getIngridientList() )
  {
    std::unique_ptr<Item> item = BUTCHER.actorsDatabase().createActor<Item>(kv.first);
    if ( item )
    {
      AmountedItem invItem = _player->getInventory().getItem(kv.first);
      int amountPresent = invItem.amount;
      int amountNeeded = kv.second;
      ingridientsList->pushBackCustomItem( makeIngridientListItem(item->getName() +
                                                                  " (" + toStr(amountPresent) +
                                                                  "/" + toStr(amountNeeded) + ")",
                                                                  item->getSprite()->getResourceName(),
                                                                  amountPresent >= amountNeeded ? cc::Color4B::WHITE : cc::Color4B::RED) );
    }
  }
}

int CraftView::createCraftMenuButtons()
{
  _smithingBtn = cc::MenuItemImage::create("images/btn_smithing.png",
                                           "images/btn_smithing_click.png",
                                           "images/btn_smithing_click.png",
                                               CC_CALLBACK_1(CraftView::craftChooseSmithing, this));
  _smithingBtn->setAnchorPoint(cc::Vec2(0,0));
  _smithingBtn->setPosition(cc::Vec2(0, 0));

  _tanningBtn = cc::MenuItemImage::create("images/btn_tanning.png",
                                              "images/btn_tanning_click.png",
                                              "images/btn_tanning_click.png",
                                              CC_CALLBACK_1(CraftView::craftChooseTanning, this));
  _tanningBtn->setAnchorPoint(cc::Vec2(0,0));
  _tanningBtn->setPosition(cc::Vec2(_smithingBtn->getPositionX() + _smithingBtn->getBoundingBox().size.width*1.1,_smithingBtn->getPositionY()));

  _cookingBtn = cc::MenuItemImage::create("images/btn_cooking.png",
                                              "images/btn_cooking_click.png",
                                              "images/btn_cooking_click.png",
                                              CC_CALLBACK_1(CraftView::craftChooseCooking, this));
  _cookingBtn->setAnchorPoint(cc::Vec2(0,0));
  _cookingBtn->setPosition(cc::Vec2(_tanningBtn->getPositionX() + _tanningBtn->getBoundingBox().size.width*1.1,_tanningBtn->getPositionY()));

  _engineeringBtn = cc::MenuItemImage::create("images/btn_engineering.png",
                                                  "images/btn_engineering_click.png",
                                                  "images/btn_engineering_click.png",
                                                  CC_CALLBACK_1(CraftView::craftChooseEngineering, this));
  _engineeringBtn->setAnchorPoint(cc::Vec2(0,0));
  _engineeringBtn->setPosition(cc::Vec2(_cookingBtn->getPositionX() + _cookingBtn->getBoundingBox().size.width*1.1,_cookingBtn->getPositionY()));

  //Create menu
  auto menu = cc::Menu::create( _smithingBtn, _tanningBtn, _cookingBtn, _engineeringBtn, NULL);
  menu->setAnchorPoint(cc::Vec2(0.5,0));
  menu->setPosition(cc::Vec2::ZERO);
  int menuWidth = _smithingBtn->getBoundingBox().size.width*1.1*4;
  int menuHeight = _smithingBtn->getBoundingBox().size.height;

  //Create layout for menu
  cc::ui::Layout* layout = cc::ui::Layout::create();
  layout->setContentSize( cc::Size(menuWidth, menuHeight) );
  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  layout->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
  lpT->setMargin(cc::ui::Margin( _margin*2, _margin*2, 0, _margin));

  layout->addChild(menu);
  _craftInfoPanel->addChild(layout);

  return layout->getContentSize().height;
}

int CraftView::createCraftLevelInfo()
{
  int totalWidth = 0;
  //create level label
  _craftLevelLabel = make_label("Level: 0");
  _craftLevelLabel->setAnchorPoint(cc::Vec2(0,0.5));
  _craftLevelLabel->setPosition(0, _craftLevelLabel->getBoundingBox().size.height / 2);
  totalWidth += _craftLevelLabel->getBoundingBox().size.width * 1.2;

  //create progress bar
  _craftLevelBar = cc::ui::LoadingBar::create();
  _craftLevelBar->loadTexture("images/progress_bar_cyan.png");
  _craftLevelBar->setPercent( 0 );
  _craftLevelBar->setAnchorPoint(cc::Vec2(0,0.5));
  _craftLevelBar->setPosition(cc::Vec2(_craftLevelLabel->getPositionX() + _craftLevelLabel->getBoundingBox().size.width*1.2,
                                _craftLevelLabel->getPositionY()));
  _craftLevelBar->setScaleY(0.6);
  _craftLevelBar->setScaleX(1.2);
  _craftLevelBar->setLocalZOrder(2);

  cc::Sprite* barBg = cc::Sprite::create();
  barBg->initWithFile("images/progress_bar_bg.png");
  barBg->setAnchorPoint(cc::Vec2(0,0.5));
  barBg->setPosition(_craftLevelBar->getPosition());
  barBg->setScaleY(0.6);
  barBg->setScaleX(1.2);
  barBg->setLocalZOrder(1);
  totalWidth += barBg->getBoundingBox().size.width;

  //Create layout
  cc::ui::Layout* layout = cc::ui::Layout::create();
  layout->setContentSize( cc::Size(totalWidth, _craftLevelLabel->getBoundingBox().size.height) );

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  layout->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
  lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, 0, _margin));

  layout->addChild(_craftLevelLabel);
  layout->addChild(_craftLevelBar);
  layout->addChild(barBg);
  _craftInfoPanel->addChild(layout);

  return layout->getContentSize().height;
}

int CraftView::createCraftPointsInfo()
{
  int totalWidth = 0;
  //create level label
  _craftPointsLabel = make_label("Craft points: 0");
  _craftPointsLabel->setAnchorPoint(cc::Vec2(0,0.5));
  _craftPointsLabel->setPosition(0, _craftPointsLabel->getBoundingBox().size.height / 2);
  totalWidth += _craftPointsLabel->getBoundingBox().size.width * 1.2;

  //create earn button

  //add craft button
  _learnBtn = cc::ui::Button::create("images/button_orange.png",
                                                    "images/button_orange_click.png",
                                                    "images/button_disabled.png");
  _learnBtn->setTitleText("Learn");
  _learnBtn->setTitleColor(cc::Color3B::WHITE);
  _learnBtn->setTitleFontSize(18);
  _learnBtn->setTitleFontName("fonts/Marker Felt.ttf");
  _learnBtn->setContentSize(cc::Size(120, _craftPointsLabel->getBoundingBox().size.height));
  _learnBtn->setScale9Enabled(true);
  _learnBtn->setAnchorPoint(cc::Vec2(0, 0.5));
  _learnBtn->setPositionX(_craftPointsLabel->getPositionX() + _craftPointsLabel->getBoundingBox().size.width*1.2);
  _learnBtn->setPositionY(_craftPointsLabel->getBoundingBox().size.height / 2);
  totalWidth += _learnBtn->getBoundingBox().size.width;
  //learnBtn->setEnabled(_selectedRecipe->checkIngridients());
//  learnBtn->addTouchEventListener([this](Ref*, cc::ui::Widget::TouchEventType type){
//    if ( type == cc::ui::Widget::TouchEventType::ENDED )
//    {
//      if ( !_selectedRecipe->produce() )
//        showMessage("Failed to create item!", cc::Color4B::RED);
//      else
//        showMessage("Item created successfully!", cc::Color4B::GREEN);

//      fillWorkbench();
//    }
//  });

  //Create layout
  cc::ui::Layout* layout = cc::ui::Layout::create();
  layout->setContentSize( cc::Size(totalWidth, _craftPointsLabel->getBoundingBox().size.height) );

  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  layout->setLayoutParameter(lpT);
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
  lpT->setMargin(cc::ui::Margin(_margin*2, _margin*2, 0, _margin));

  layout->addChild(_craftPointsLabel);
  layout->addChild(_learnBtn);
  _craftInfoPanel->addChild(layout);

  return layout->getContentSize().height;
}


void CraftView::fillCraftInfoPanel()
{
  //Craft level
  int craftLevel = _player->getCraftbook().getCraftLevel(_selectedCraft);
  _craftLevelLabel->setString("Level: " + toStr(craftLevel));

  //craft level bar
  int previousLevelPtsNeeded = getCraftLevelPointsNeeded(_selectedCraft, craftLevel);
  int pts = _player->getCraftbook().getSpentPoints(_selectedCraft) - previousLevelPtsNeeded;
  int needed = getCraftLevelPointsNeeded(_selectedCraft, craftLevel + 1) - previousLevelPtsNeeded;
  int percent = (float)((float)pts / (float)needed) * 100.f;
  _craftLevelBar->setPercent(percent);

  //craft points
  int freePoints = _player->getCraftbook().getFreePoints();
  _craftPointsLabel->setString( "Craft points: " + toStr(freePoints));

  //learn button
  _learnBtn->setEnabled( freePoints > 0 );

}

void CraftView::chooseCraftCommon()
{
  fillCraftInfoPanel();
  fillRecipeList();
  _selectedRecipe = nullptr;
  fillWorkbench();
}

void CraftView::craftChooseSmithing(cocos2d::Ref *)
{
  _selectedCraft = CraftType::SMITHING;

  _smithingBtn->setEnabled(false);
  _tanningBtn->setEnabled(true);
  _cookingBtn->setEnabled(true);
  _engineeringBtn->setEnabled(true);

  chooseCraftCommon();
}

void CraftView::craftChooseTanning(cocos2d::Ref *)
{
  _selectedCraft = CraftType::TANNING;

  _smithingBtn->setEnabled(true);
  _tanningBtn->setEnabled(false);
  _cookingBtn->setEnabled(true);
  _engineeringBtn->setEnabled(true);

  chooseCraftCommon();
}

void CraftView::craftChooseCooking(cocos2d::Ref *)
{
  _selectedCraft = CraftType::COOKING;

  _smithingBtn->setEnabled(true);
  _tanningBtn->setEnabled(true);
  _cookingBtn->setEnabled(false);
  _engineeringBtn->setEnabled(true);

  chooseCraftCommon();
}

void CraftView::craftChooseEngineering(cocos2d::Ref *)
{
  _selectedCraft = CraftType::ENGINEERING;

  _smithingBtn->setEnabled(true);
  _tanningBtn->setEnabled(true);
  _cookingBtn->setEnabled(true);
  _engineeringBtn->setEnabled(false);

  chooseCraftCommon();
}

}
