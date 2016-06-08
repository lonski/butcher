#ifndef DUNGEONSCENE_H
#define DUNGEONSCENE_H

#include "cocos2d.h"
#include <memory>

namespace butcher {

class Actor;
class DungeonState;

class DungeonLayer : public cocos2d::Layer
{
public:
    DungeonLayer();
    ~DungeonLayer();

    virtual bool init();
    virtual void setState(DungeonState* state);
    virtual void onEnter();
    virtual void onExit();

    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event*);

    CREATE_FUNC(DungeonLayer);

private:
    cocos2d::EventListenerTouchOneByOne* _listener;
    DungeonState* _state;
    cocos2d::Vec2 _viewPoint;

    void setViewPointCenter(cocos2d::Vec2 position);

};

}

#endif // DUNGEONSCENE_H
