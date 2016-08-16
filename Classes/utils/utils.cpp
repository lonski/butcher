#include "utils.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

namespace cc = cocos2d;

namespace butcher {

cc::Vec2 positionToTileCoord(cc::TMXTiledMap *map, cc::Vec2 pos)
{
    if ( !map )
        return cc::Vec2::ZERO;

    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y)
            / map->getTileSize().height;

    return cc::Vec2(x, y);
}

cc::Vec2 tileCoordToPosition(cc::TMXTiledMap *map, cc::Vec2 coord)
{
    if ( !map )
        return cc::Vec2::ZERO;

    int x = coord.x * map->getTileSize().width;
    int y = (map->getMapSize().height*map->getTileSize().height - map->getTileSize().height * coord.y) ;

    return cc::Vec2(x + map->getTileSize().height/2, y - map->getTileSize().width/2);
}

std::vector<std::string> explode(const std::string &str, char ch)
{
  std::vector<std::string> result;
  std::string line;

  for (auto s : str)
  {
    if (s == ch)
    {
      if (!line.empty())
      {
        result.push_back(line);
        line.clear();
      }
    }
    else
    {
      line += s;
    }
  }

  if (!line.empty())
  {
    result.push_back(line);
    line.clear();
  }

  return result;
}

float calculateDistance(cc::Vec2 a, cc::Vec2 b)
{
  int dx = b.x - a.x;
  int dy = b.y - a.y;

  return sqrtf(dx*dx + dy*dy);
}

cocos2d::Label* make_label(const std::string &text, cocos2d::Color4B color, int size, cocos2d::Vec2 anchor)
{
  cocos2d::Label* label = cc::Label::createWithTTF( text, "fonts/Marker Felt.ttf", size);
  label->setTextColor(color);
  label->enableOutline( cc::Color4B::BLACK, 1 );
  label->setAnchorPoint(anchor);

  return label;
}

void showMessage(const std::vector<std::string>& msg, cocos2d::Color4B color, cocos2d::Node* parent)
{
  int margin = 10;
  auto origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::ui::Layout* layout = cc::ui::Layout::create();

  layout->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  layout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  layout->setBackGroundImageScale9Enabled(true);
  layout->setBackGroundImage("images/inv_border_fill.png");
  cc::Size size;
  size.width = 0;
  size.height = 0;

  int labelWidth = 200;
  int labelsHeight = 0;

  cc::ui::Layout* textLayout = cc::ui::Layout::create();
  textLayout->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(margin, margin, margin, margin));
  textLayout->setLayoutParameter(lpT);

  cc::ui::LinearLayoutParameter* lpLabel = cc::ui::LinearLayoutParameter::create();
  lpLabel->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);

  for ( auto s : msg )
  {
    cc::ui::Text* label = cc::ui::Text::create(s,"fonts/Marker Felt.ttf",18);
    label->setTextColor(color);
    label->setLayoutParameter(lpLabel);
    labelWidth = std::max( labelWidth, (int)label->getBoundingBox().size.width);
    labelsHeight += label->getBoundingBox().size.height;
    textLayout->addChild(label);
  }
  size.width += labelWidth + 2*margin;
  size.height += labelsHeight;
  textLayout->setContentSize(size);
//  textLayout->setBackGroundColor(cc::Color3B::GREEN);
//  textLayout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::SOLID);
  layout->addChild(textLayout);

  cc::ui::Button* closeBtn = cc::ui::Button::create();
  closeBtn->loadTextures("images/button_blue.png", "images/button_blue_click.png", "");
  closeBtn->setTitleText("Close");
  closeBtn->setTitleFontSize(18);
  closeBtn->setTitleFontName("fonts/Marker Felt.ttf");
  closeBtn->setScale9Enabled(true);
  closeBtn->setContentSize(cc::Size(96,32));
  closeBtn->addTouchEventListener([=](cc::Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
    {
      parent->removeChild(layout);
    }
  });

  cc::ui::LinearLayoutParameter* lpBTN = cc::ui::LinearLayoutParameter::create();
  lpBTN->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
  closeBtn->setLayoutParameter(lpBTN);

  layout->addChild(closeBtn);
  size.height += closeBtn->getBoundingBox().size.height;

  layout->setContentSize(cc::Size(size.width + 2*margin, size.height+4*margin));
  layout->setAnchorPoint(cc::Vec2(0.5, 0.5));
  layout->setPosition(cc::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

  parent->addChild(layout);
}

void ask(const std::string &msg, cocos2d::Node *parent, std::function<void ()> yesFunction, std::function<void ()> noFunction)
{
  int margin = 10;
  auto origin = cc::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cc::Director::getInstance()->getVisibleSize();

  cc::ui::Layout* layout = cc::ui::Layout::create();
  layout->setLayoutType(cc::ui::Layout::Type::VERTICAL);
  layout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  layout->setBackGroundImageScale9Enabled(true);
  layout->setBackGroundImage("images/inv_border_fill.png");

  cc::ui::LinearLayoutParameter* lpTr = cc::ui::LinearLayoutParameter::create();
  lpTr->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  layout->setLayoutParameter(lpTr);

  cc::Size size;
  size.width = margin * 2;
  size.height = margin * 4;

  cc::ui::Text* label= cc::ui::Text::create(msg,"fonts/Marker Felt.ttf", 18);
  size.width += label->getBoundingBox().size.width;
  size.height += label->getBoundingBox().size.height;
  cc::ui::LinearLayoutParameter* lpT = cc::ui::LinearLayoutParameter::create();
  lpT->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
  lpT->setMargin(cc::ui::Margin(margin, margin, margin, margin));
  label->setLayoutParameter(lpT);

  layout->addChild(label);

  cc::ui::Button* yesBtn = cc::ui::Button::create();
  yesBtn->setTitleText("Yes");
  yesBtn->setTitleFontSize(18);
  yesBtn->setContentSize(cc::Size(128,64));
  yesBtn->setScale9Enabled(true);
  yesBtn->setTitleFontName("fonts/Marker Felt.ttf");
  yesBtn->loadTextures("images/button_orange.png", "images/button_orange_click.png", "");
  yesBtn->setAnchorPoint(cc::Vec2(0.5,0));
  yesBtn->setPosition(cc::Vec2(size.width / 2 - yesBtn->getBoundingBox().size.width * 1.2, margin));
  yesBtn->addTouchEventListener([=](cc::Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
    {
      parent->removeChild(layout);
      yesFunction();
    }
  });

  cc::ui::Button* noBtn = cc::ui::Button::create();
  noBtn->setTitleText("No");
  noBtn->setTitleFontSize(18);
  noBtn->setContentSize(cc::Size(128,64));
  noBtn->setScale9Enabled(true);
  noBtn->setTitleFontName("fonts/Marker Felt.ttf");
  noBtn->loadTextures("images/button_green.png", "images/button_green_click.png", "");
  noBtn->setAnchorPoint(cc::Vec2(0.5,0));
  noBtn->setPosition(cc::Vec2(yesBtn->getPositionX() + yesBtn->getBoundingBox().size.width * 1.2, margin));
  noBtn->addTouchEventListener([=](cc::Ref*, cc::ui::Widget::TouchEventType type){
    if ( type == cc::ui::Widget::TouchEventType::ENDED )
    {
      parent->removeChild(layout);
      noFunction();
    }
  });

  cc::ui::Layout* btnLayout = cc::ui::Layout::create();
  btnLayout->setLayoutType(cc::ui::Layout::Type::HORIZONTAL);
  btnLayout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::NONE);
  btnLayout->addChild(yesBtn);
  btnLayout->addChild(noBtn);
  cc::ui::LinearLayoutParameter* lpT2 = cc::ui::LinearLayoutParameter::create();
  lpT2->setGravity(cc::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
  lpT2->setMargin(cc::ui::Margin(margin/2, margin, margin, margin/2));
  btnLayout->setLayoutParameter(lpT2);
  btnLayout->setContentSize(cc::Size(yesBtn->getBoundingBox().size.width * 1.2 + noBtn->getBoundingBox().size.width,
                                     yesBtn->getBoundingBox().size.height));
//  btnLayout->setBackGroundColor(cc::Color3B::GREEN);
//  btnLayout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::SOLID);


  size.height += noBtn->getContentSize().height;
  size.width = std::max( size.width,  noBtn->getContentSize().width*2 + 5*margin);
  layout->addChild(btnLayout);
  layout->setContentSize(size);
  layout->setAnchorPoint(cc::Vec2(0.5, 0.5));
  layout->setPosition(cc::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

//  layout->setBackGroundColor(cc::Color3B::BLUE);
//  layout->setBackGroundColorType(cc::ui::Layout::BackGroundColorType::SOLID);

  parent->addChild(layout, 666);
}

float tangens(cocos2d::Vec2 p1, cocos2d::Vec2 p2)
{
  cc::Vec2 dp = p2 - p1;
  return dp.x != 0 ? dp.y/dp.x : 0.0f;
}

}
