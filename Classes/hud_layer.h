#ifndef HUDLAYER_H
#define HUDLAYER_H

#include "cocos2d.h"

namespace butcher {

class HudLayer : public cocos2d::Layer
{
public:
    HudLayer();
    ~HudLayer();

    virtual bool init();


    void showMenu(cocos2d::Ref*);
    void showMenu2(cocos2d::Ref*);

    CREATE_FUNC(HudLayer);

};

}

#endif // HUDLAYER_H
