#include "hud_log.h"
#include <utils/utils.h>

namespace cc = cocos2d;

namespace butcher {

HudLog::HudLog()
  : _messageCnt(0)
{
  _size.width = 0;
  _size.height = 0;
}

void HudLog::addMessage(const std::string &msg, cc::Color4B color)
{
  ++_messageCnt;

  cocos2d::Label* label = make_label(msg, color, 18, cc::Vec2::ZERO);

  _size.height += label->getBoundingBox().size.height;
  _size.width = std::max(_size.width, label->getBoundingBox().size.width),

  setContentSize(_size);

  label->setPosition( 0, -_size.height );
  addChild(label, 1);

  label->runAction( cc::Sequence::create(cc::FadeOut::create(5),
                                              cc::CallFunc::create([&]()
                                              {
                                                if ( --_messageCnt == 0)
                                                {
                                                  _size.height = 0;
                                                  _size.width = 0;
                                                }
                                              }),
                                              cc::RemoveSelf::create(), nullptr) );
}

}
