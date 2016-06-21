#include "hud_log.h"

namespace butcher {

HudLog::HudLog()
  : _messageCnt(0)
{
  _size.width = 0;
  _size.height = 0;
}

void HudLog::addMessage(const std::string &msg, cocos2d::Color4B color)
{
  ++_messageCnt;

  cocos2d::Label* label = cocos2d::Label::createWithTTF(msg, "fonts/Marker Felt.ttf", 18);

  _size.height += label->getBoundingBox().size.height;
  _size.width = std::max(_size.width, label->getBoundingBox().size.width),

  setContentSize(_size);

  label->setAnchorPoint( cocos2d::Vec2(0, 0) );
  label->setTextColor(color);
  label->setPosition( 0, -_size.height );
  label->enableOutline( cocos2d::Color4B::BLACK, 1 );
  addChild(label, 1);

  label->runAction( cocos2d::Sequence::create(cocos2d::FadeOut::create(5),
                                              cocos2d::CallFunc::create([&]()
                                              {
                                                if ( --_messageCnt == 0)
                                                {
                                                  _size.height = 0;
                                                  _size.width = 0;
                                                }
                                              }),
                                              cocos2d::RemoveSelf::create(), nullptr) );
}

}
