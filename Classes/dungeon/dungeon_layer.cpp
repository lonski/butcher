#include "dungeon_layer.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <utils/directions.h>
#include <actors/actions/move_action.h>

using namespace cocos2d;

namespace butcher {

DungeonLayer::DungeonLayer()
    : _listener(nullptr)
    , _state(nullptr)
{
}

DungeonLayer::~DungeonLayer()
{
}

bool DungeonLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

void DungeonLayer::setState(DungeonState* state)
{
  _state = state;
}

void DungeonLayer::onEnter()
{
    Layer::onEnter();

    if ( _state == nullptr )
    {
      log("DungeonLayer::onEnter: DungeonState not set!");
      return;
    }

    if ( _listener == nullptr )
    {
      _listener = EventListenerTouchOneByOne::create();
      _listener->onTouchBegan = CC_CALLBACK_2(DungeonLayer::onTouchBegan, this);
      _listener->onTouchEnded = CC_CALLBACK_2(DungeonLayer::onTouchEnded, this);
    }

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

    removeAllChildren();

    addChild( _state->map() );
    _state->spawnActors(this);

    setViewPointCenter(BUTCHER.getPlayer()->getPosition());
}

void DungeonLayer::onExit()
{
  removeAllChildren();

  auto dispatcher = Director::getInstance()->getEventDispatcher();
  dispatcher->removeEventListener(_listener);
  _listener = nullptr;
}

bool DungeonLayer::onTouchBegan(Touch*, Event*)
{
    return true;
}

void DungeonLayer::onTouchEnded(Touch* touch, Event*)
{
  Vec2 touchLocation = touch->getLocationInView();
  touchLocation = Director::getInstance()->convertToGL(touchLocation);
  touchLocation = this->convertToNodeSpace(touchLocation);

  Vec2 playerPos = BUTCHER.getPlayer()->getPosition();
  Vec2 diff = touchLocation - playerPos;

  Direction direction = Direction::None;

  if ( abs(diff.x) > abs(diff.y) )
      direction = diff.x > 0 ? Direction::East : Direction::West;
  else
    direction = diff.y > 0 ? Direction::South : Direction::North;

  if ( direction != Direction::None )
  {
    if ( BUTCHER.getPlayer()->performAction( new MoveAction(direction) ) )
      this->setViewPointCenter(BUTCHER.getPlayer()->getPosition());

    BUTCHER.nextTurn();
  }
}

void DungeonLayer::setViewPointCenter(Vec2 position)
{
  Size winSize = Director::getInstance()->getWinSize();

  int x = std::max(position.x, winSize.width/2);
  int y = std::max(position.y, winSize.height/2);
  x = std::min(x, (int)((_state->map()->getMapSize().width * this->_state->map()->getTileSize().width ) - winSize.width / 2));
  y = std::min(y, (int)((_state->map()->getMapSize().height * _state->map()->getTileSize().height) - winSize.height/2));
  Vec2 actualPosition(x, y);

  Vec2 centerOfView(winSize.width/2, winSize.height/2);
  Vec2 viewPoint = centerOfView - actualPosition;

  runAction( MoveTo::create(0.1,viewPoint));
}

}
