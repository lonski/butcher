#include "dungeon_layer.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <utils/directions.h>
#include <actors/actions/move_action.h>
#include <utils/utils.h>

namespace cc = cocos2d;

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
      cc::log("DungeonLayer::onEnter: DungeonState not set!");
      return;
    }

    if ( _listener == nullptr )
    {
      _listener = cc::EventListenerTouchOneByOne::create();
      _listener->onTouchBegan = CC_CALLBACK_2(DungeonLayer::onTouchBegan, this);
      _listener->onTouchEnded = CC_CALLBACK_2(DungeonLayer::onTouchEnded, this);
    }

    auto dispatcher = cc::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

    removeAllChildren();

    addChild( _state->map() );
    _state->onEnter(this);

    setViewPointCenter(BUTCHER.getPlayer()->getPosition());
}

void DungeonLayer::onExit()
{
  _state->onExit();

  removeAllChildren();

  auto dispatcher = cc::Director::getInstance()->getEventDispatcher();
  dispatcher->removeEventListener(_listener);
  _listener = nullptr;
}

bool DungeonLayer::onTouchBegan(cc::Touch*, cc::Event*)
{
  return getPosition() == _viewPoint;
}

void DungeonLayer::onTouchEnded(cc::Touch* touch, cc::Event*)
{
  cc::Vec2 touchLocation = touch->getLocationInView();
  touchLocation = cc::Director::getInstance()->convertToGL(touchLocation);
  touchLocation = this->convertToNodeSpace(touchLocation);

  cc::Vec2 diff = positionToTileCoord(_state->map(), touchLocation) - BUTCHER.getPlayer()->getTileCoord();
  Direction direction = Direction::None;


  if ( diff.x > 0 && diff.y == 0 )
  {
    direction = Direction::East;
  }
  else if ( diff.x < 0 && diff.y == 0 )
  {
    direction = Direction::West;
  }
  else if ( diff.x == 0 && diff.y > 0 )
  {
    direction = Direction::North;
  }
  else if ( diff.x == 0 && diff.y < 0 )
  {
    direction = Direction::South;
  }
  else if ( diff.x < 0 && diff.y < 0 )
  {
    direction = Direction::SouthWest;
  }
  else if ( diff.x > 0 && diff.y < 0 )
  {
    direction = Direction::SouthEast;
  }
  else if ( diff.x < 0 && diff.y > 0 )
  {
    direction = Direction::NorthWest;
  }
  else if ( diff.x > 0 && diff.y > 0 )
  {
    direction = Direction::NorthEast;
  }

  if ( direction != Direction::None )
  {
    if ( BUTCHER.getPlayer()->performAction( new MoveAction(direction) ) )
      this->setViewPointCenter(BUTCHER.getPlayer()->getPosition());

    BUTCHER.nextTurn();
  }
}

void DungeonLayer::setViewPointCenter(cc::Vec2 position)
{
  cc::Vec2 coord = positionToTileCoord(_state->map(), position);
  auto tiles = _state->map()->getLayer("Background");
  cc::Vec2 pos = tiles->getPositionAt(coord);

  cc::Size winSize = cc::Director::getInstance()->getWinSize();

  cc::Vec2 centerOfView(winSize.width/2, winSize.height/2);
  _viewPoint = centerOfView - pos;

  runAction( cc::MoveTo::create(0.08,_viewPoint));
}

}
