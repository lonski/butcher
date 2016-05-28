#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();

    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

//    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
//    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));

//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 1);

    _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("cave1.tmx");

    _tiles = _tileMap->getLayer("Background");

    _tiles_meta = _tileMap->getLayer("Meta");
    _tiles_meta->setVisible(false);

    this->addChild(_tileMap);

    TMXObjectGroup *objectGroup = _tileMap->getObjectGroup("Objects");

    if(objectGroup == NULL){
        log("tile map has no objects object layer");
        return false;
    }

    ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");

    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();

    _player = new Sprite();
    _player->initWithFile("player.png");
    _player->setPosition(x,y);

    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());

    //this->setTouchEnabled(true);

    return true;
}

Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y)
            / _tileMap->getTileSize().height/ Director::getInstance()->getContentScaleFactor();
    return ccp(x, y);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::setViewPointCenter(Vec2 position)
{
    Size winSize = Director::sharedDirector()->getWinSize();

    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width ) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    Vec2 actualPosition = ccp(x, y);

    Vec2 centerOfView = ccp(winSize.width/2, winSize.height/2);
    Vec2 viewPoint = ccpSub(centerOfView, actualPosition);

    this->setPosition(viewPoint);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void HelloWorld::setPlayerPosition(Vec2 position) {
    Vec2 tileCoord = this->tileCoordForPosition(position);
    int tileGid = _tiles_meta->tileGIDAt(tileCoord);
    if (tileGid) {

        auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty()) {
            auto collision = properties["Collidable"].asString();
            if ("True" == collision) {
                return;
            }
        }

    }
    _player->setPosition(position);
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Vec2 touchLocation = touch->getLocationInView();
    touchLocation = Director::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);

    Vec2 playerPos = _player->getPosition();
    Vec2 diff = ccpSub(touchLocation, playerPos);

    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }

    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        this->setPlayerPosition(playerPos);
    }

    this->setViewPointCenter(_player->getPosition());
}
