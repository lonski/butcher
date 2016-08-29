#include "dungeon_layer.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <actors/actions/move_action.h>
#include <actors/actions/shot_action.h>
#include <utils/utils.h>
#include <utils/path.h>

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

    auto eventListener = cc::EventListenerKeyboard::create();

    eventListener->onKeyPressed = [this](cc::EventKeyboard::KeyCode keyCode, cc::Event* event){

        switch(keyCode){
            case cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case cc::EventKeyboard::KeyCode::KEY_A:
                move(Direction::West);
                BUTCHER.nextTurn();
                break;
            case cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case cc::EventKeyboard::KeyCode::KEY_D:
                  move(Direction::East);
                  BUTCHER.nextTurn();
                break;
            case cc::EventKeyboard::KeyCode::KEY_UP_ARROW:
            case cc::EventKeyboard::KeyCode::KEY_W:
                  move(Direction::North);
                  BUTCHER.nextTurn();
                break;
            case cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case cc::EventKeyboard::KeyCode::KEY_S:
                  move(Direction::South);
                  BUTCHER.nextTurn();
                break;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);

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

void DungeonLayer::move(Direction::Symbol direction)
{
  auto player = BUTCHER.getPlayer();

  if ( player->performAction(new MoveAction(direction)) )
  {
    this->setViewPointCenter(BUTCHER.getPlayer()->getPosition());
    BUTCHER.getPlayer()->notify(EventType::Moved);
  }
}

void DungeonLayer::onTouchEnded(cc::Touch* touch, cc::Event*)
{

  cc::Vec2 touchCoord = getTouchCoord(touch);
  Direction::Symbol direction = getTouchDirection(touchCoord);
  Target target = getTouchTarget(touchCoord);

  if ( direction != Direction::None )
  {
    auto player = BUTCHER.getPlayer();
    if ( target.actors.empty() || !player->performAction(new ShotAction(target)) )
      move(direction);

    BUTCHER.nextTurn();
  }
}

Target DungeonLayer::getTouchTarget(cc::Vec2 touchCoord)
{
  auto characterOnTile = _state->getActorsAt(touchCoord, [](std::shared_ptr<Actor> a){
    return std::dynamic_pointer_cast<Character>(a) != nullptr;
  });

  Target target(touchCoord);
  target.actors = characterOnTile;

  return target;
}

cocos2d::Vec2 DungeonLayer::getTouchCoord(cc::Touch* touch)
{
  cc::Vec2 touchLocation = touch->getLocationInView();
  touchLocation = cc::Director::getInstance()->convertToGL(touchLocation);
  touchLocation = this->convertToNodeSpace(touchLocation);
  cc::Vec2 touchCoord = positionToTileCoord(_state->map(), touchLocation);

  return touchCoord;
}

Direction::Symbol DungeonLayer::getTouchDirection(cc::Vec2 touchCoord)
{
  cc::Vec2 diff = touchCoord - BUTCHER.getPlayer()->getTileCoord();
  Direction::Symbol direction = Direction::fromPosition(diff);

  return direction;
}

void DungeonLayer::setViewPointCenter(cc::Vec2 position)
{
  cc::Vec2 coord = positionToTileCoord(_state->map(), position);
  auto tiles = _state->map()->getLayer("Background");
  cc::Vec2 pos = tiles->getPositionAt(coord);

  cc::Size winSize = cc::Director::getInstance()->getWinSize();

  cc::Vec2 centerOfView(winSize.width/2, winSize.height/2);
  _viewPoint = centerOfView - pos;

  runAction( cc::MoveTo::create(0.15,_viewPoint));
}

}
