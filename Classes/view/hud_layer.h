#ifndef HUDLAYER_H
#define HUDLAYER_H

#include "cocos2d.h"
#include <view/hud_log.h>

namespace butcher {

class HudLayer : public cocos2d::Layer
{
public:
  HudLayer();
  ~HudLayer();

  virtual bool init();
  void log(const std::string& str);


  void showMenu(cocos2d::Ref*);
  void showMenu2(cocos2d::Ref*);

  CREATE_FUNC(HudLayer);

private:
  int _logCount;
  HudLog _log;

};

}

#endif // HUDLAYER_H
