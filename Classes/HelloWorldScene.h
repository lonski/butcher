#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void onEnter();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually

    void setPlayerPosition(cocos2d::Vec2 position);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event );
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    CREATE_FUNC(HelloWorld);


private:
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::TMXLayer *_tiles;
    cocos2d::TMXLayer *_tiles_meta;
    cocos2d::Sprite* _player;

    void setViewPointCenter(cocos2d::Vec2 position);
    cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
};

#endif // __HELLOWORLD_SCENE_H__
