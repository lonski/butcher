#ifndef LOADINGSCENE_H
#define LOADINGSCENE_H

#include "cocos2d.h"
#include <functional>

namespace butcher {

class LoadingScreen : public cocos2d::Layer
{
public:
  LoadingScreen();

  static void run(std::function<void()> fun, const std::string& msg);

  void update(float);

private:
  std::function<void()> _function;
  bool init(const std::string& msg);

};

}

#endif // LOADINGSCENE_H
