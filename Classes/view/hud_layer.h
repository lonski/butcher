#ifndef HUDLAYER_H
#define HUDLAYER_H

#include "cocos2d.h"

namespace butcher {

class HudLog;

class HudLayer : public cocos2d::Layer
{
public:
  HudLayer();
  ~HudLayer();

  virtual bool init();

  void print(const std::string& str,
           cocos2d::Color4B color = cocos2d::Color4B(177, 0, 177, 255));

  CREATE_FUNC(HudLayer);

private:
  int _logCount;
  HudLog* _log;

  void showMenu(cocos2d::Ref*);
};

}

#endif // HUDLAYER_H
