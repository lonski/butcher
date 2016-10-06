#include "throw_action.h"
#include <actors/actor.h>
#include <butcher.h>
#include <dungeon/dungeon_state.h>
#include <actors/character.h>

namespace cc = cocos2d;

namespace butcher {

ThrowAction::ThrowAction(std::shared_ptr<Actor> grenade)
  : _grenade(grenade)
{

}

void ThrowAction::executeEffects(std::shared_ptr<Item> grenadeItem)
{
  DungeonState* dungeon = BUTCHER.getCurrentDungeon();
  auto victims = dungeon->getActors([=](std::shared_ptr<Actor> a){
    return a->isAlive() && calculateDistance(a->getTileCoord(), _target.pos) <= grenadeItem->getRadius();
  });

  for ( std::shared_ptr<Actor> v : victims )
  {
    std::shared_ptr<Character> c = std::dynamic_pointer_cast<Character>(v);
    if ( c )
    {
      c->takeDamage(grenadeItem->getDamage(), _grenade );

      for ( Effect e : _grenade->getEffects() )
      {
        c->addEffect( e );
        c->fadeText( e.getName(), cc::Color4B::ORANGE, 1, false);
      }
    }
  }
}

ActorAction::Result ThrowAction::perform(std::shared_ptr<Actor> performer)

{
  _performer = performer;
  if ( !_performer )
  {
    cocos2d::log("%s: performer is null.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  std::shared_ptr<Item> grenadeItem = std::dynamic_pointer_cast<Item>(_grenade);
  if ( !grenadeItem )
  {
    cocos2d::log("%s: grenadeItem is null.", __PRETTY_FUNCTION__);
    return ActorAction::Result::NOK;
  }

  runThrowAnimation();
  executeEffects(grenadeItem);
  runExplosionAnimation(grenadeItem);

  return ActorAction::Result::OK;
}

void ThrowAction::runExplosionAnimation(std::shared_ptr<Item> grenadeItem)
{
  float radius = grenadeItem->getRadius();
  auto view = _performer->getSprite()->getParent();

  for( int y = _target.pos.y - radius/2 - 1; y <= _target.pos.y + radius/2 + 1; ++y )
  {
    for( int x = _target.pos.x - radius/2 - 1; x <= _target.pos.x + radius/2 + 1; ++x )
    {
      float distance = calculateDistance(_target.pos, cc::Vec2(x, y) );
      if ( distance <= radius )
      {
        auto pixelPos = tileCoordToPosition(BUTCHER.getCurrentDungeon()->map(), cc::Vec2(x,y));

        cc::Sprite* fire = cc::Sprite::create();
        fire->initWithFile("images/fire.png");
        fire->setPosition(pixelPos);
        fire->setOpacity(0);
        fire->setScale(0.4);
        view->addChild(fire);

        fire->runAction(
              cc::Sequence::create(cc::DelayTime::create(0.15), cc::FadeIn::create(0.05), cc::FadeOut::create(0.05), cc::RemoveSelf::create(), nullptr) );
      }
    }
  }
}

void ThrowAction::setTarget(Target target)
{
  _target = target;
}

void ThrowAction::runThrowAnimation()
{
  _grenade->setPosition(_performer->getPosition());
  auto view = _performer->getSprite()->getParent();
  if ( view )
  {
    auto sprite = _grenade->getSprite().release();
    sprite->setScale(0.5);
    view->addChild(sprite);
    auto pixelPos = tileCoordToPosition(BUTCHER.getCurrentDungeon()->map(), _target.pos);
    sprite->runAction(
          cc::Sequence::create(cc::MoveTo::create(0.15f, pixelPos), cc::RemoveSelf::create(), nullptr) );
  }
}

}
