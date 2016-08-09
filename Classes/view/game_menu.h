#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "cocos2d.h"

namespace butcher {

class GameMenu : public cocos2d::Layer
{
public:
  GameMenu();

  void newGame(Ref *);
  void quitGame(Ref *);
  void backToGame(Ref *);
  void saveGame(Ref *);
  void loadGame(Ref *);

  static cocos2d::Scene* createScene(bool gameRunning = false);
  CREATE_FUNC(GameMenu);
  virtual bool init();

private:
  void addComponents(bool gameRunning);

};

}

#endif // GAMEMENU_H
