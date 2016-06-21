#ifndef HUD_LOG_H
#define HUD_LOG_H

#include "cocos2d.h"

namespace butcher {

class HudLog : public cocos2d::Sprite
{
public:
  HudLog();

  void addMessage(const std::string& msg,
              cocos2d::Color4B color = cocos2d::Color4B(177, 0, 177, 255));

private:
  int _messageCnt;
  cocos2d::Size _size;

};

}

#endif // HUD_LOG_H
