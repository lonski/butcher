#include "dungeon_layer.h"
#include <butcher.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <actors/actions/move_action.h>
#include <actors/actions/shot_action.h>
#include <actors/actions/throw_action.h>
#include <utils/utils.h>
#include <utils/path.h>
#include <utils/profiler.h>
#include <utils/path.h>

namespace cc = cocos2d;

namespace butcher {

DungeonLayer::DungeonLayer()
    : _listener(nullptr)
    , _state(nullptr)
    , _turnDone(true)
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
            default:;
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
  Layer::onExit();

  _state->onExit();

  removeAllChildren();

  auto dispatcher = cc::Director::getInstance()->getEventDispatcher();
  dispatcher->removeEventListener(_listener);
  _listener = nullptr;
}

bool DungeonLayer::onTouchBegan(cc::Touch*, cc::Event*)
{
  return _turnDone && getPosition() == _viewPoint;
}

void DungeonLayer::move(Direction::Symbol direction)
{
  auto player = BUTCHER.getPlayer();

  if ( player->performAction(new MoveAction(direction)) )
  {
    BUTCHER.getPlayer()->notify(EventType::Moved);
    this->setViewPointCenter(BUTCHER.getPlayer()->getPosition());
  }
}

void DungeonLayer::onTouchEnded(cc::Touch* touch, cc::Event*)
{
  _turnDone = false;

  cc::Vec2 touchCoord = getTouchCoord(touch);
  Direction::Symbol direction = getTouchDirection(touchCoord);
  Target target = getTouchTarget(touchCoord);

  auto player = BUTCHER.getPlayer();

  if ( player->hasScheduledAction() )
  {
    player->triggerScheduledAction(target);
  }
  else if ( direction == Direction::Middle )
  {
    //Autoheal
    if ( !_state->isAnyMobInFov() && player->getHp() < player->getMaxHp())
      if ( cc::RandomHelper::random_int(0,100) < 10 )
        player->setHp(player->getHp() + 1);

    //Interract with objects
    for ( auto a : _state->getActorsAt(player->getTileCoord()) )
      if ( a != player )
        a->onInterract(player);
  }
  else if ( direction != Direction::None )
  {
    if ( target.actors.empty() || !player->performAction(new ShotAction(target)) )
      move(direction);
  }

  BUTCHER.nextTurn();

  _turnDone = true;
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
  AStarPath path;
  path.calculate(BUTCHER.getPlayer()->getTileCoord(), touchCoord, [=](cc::Vec2 pos){
    return _state->isBlockedByWall(pos.x, pos.y);
  });
  path.walk();

  return Direction::fromPosition(path.walk() - BUTCHER.getPlayer()->getTileCoord());
}

void DungeonLayer::setViewPointCenter(cc::Vec2 position)
{
  cc::Vec2 coord = positionToTileCoord(_state->map(), position);
  auto tiles = _state->map()->getLayer("Background");
  cc::Vec2 pos = tiles->getPositionAt(coord);

  cc::Size winSize = cc::Director::getInstance()->getWinSize();

  cc::Vec2 centerOfView(winSize.width/2, winSize.height/2);
  _viewPoint = centerOfView - pos;

  runAction( cc::MoveTo::create(0.1,_viewPoint));
}

}
