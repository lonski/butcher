#include "monster.h"
#include <butcher.h>
#include <actors/amounted_item.h>
#include <actors/actions/pickup_action.h>
#include <utils/profiler.h>

namespace cc = cocos2d;

namespace butcher {

Monster::Monster(const ActorData* data)
  : Character(data)
  , _hpBar(nullptr)
  , _range(0)
  , _ammoId(ActorID::BONE)
{
  if ( data )
  {
    _ammoId = (ActorID)data->ammo_id();
    _range = data->range();
    auto dropRules = data->drop_rules();
    if ( dropRules )
      for (unsigned i = 0; i < dropRules->Length(); ++i)
        _dropRules.push_back( DropRule(dropRules->Get(i)) );
    else
      cc::log("no drop rules for %s", data->name()->c_str());
  }
}

std::unique_ptr<Actor> Monster::clone(std::unique_ptr<Actor> allocated)
{
  Monster* p = dynamic_cast<Monster*>(allocated.release());
  if ( p == nullptr )
    p = new Monster(nullptr);

  p->_dropRules = _dropRules;
  p->_range = _range;
  p->_ammoId = _ammoId;

  return std::move( Character::clone(std::unique_ptr<Actor>{p}) );
}

void Monster::onNextTurn()
{
  if ( !_ai )
    _ai.reset( new Ai(shared_from_this()) );

  _ai->update();

  Character::onNextTurn();
}

void Monster::onDestroy(std::shared_ptr<Actor> destroyer)
{
  for(DropRule& drop : _dropRules)
  {
    if ( drop.randomIngridientLevel > 0 )
    {
      int amount = cc::RandomHelper::random_int(drop.amountMin, drop.amountMax);
      for ( int i = 0; i < amount; i++)
      {
        if ( cc::RandomHelper::random_int(0, 100) <= drop.chance )
        {
          std::vector<ActorID> ingridients = BUTCHER.actorsDatabase().getActorIDs([drop](const Actor& actor){
            const Item* i = dynamic_cast<const Item*>(&actor);
            return i && i->getCategory() == ItemCategory::Ingridient && i->getLevel() <= drop.randomIngridientLevel;
          });

          ActorID ingridientID = ingridients[ cc::RandomHelper::random_int(0, (int)ingridients.size() - 1) ];
          AmountedItem item( std::shared_ptr<Item>( BUTCHER.actorsDatabase().createActor<Item>(ingridientID) ), 1);

          destroyer->performAction( new PickUpAction(item) );
        }
      }
    }
    else if ( drop.itemId != ActorID::INVALID )
    {
      if ( cc::RandomHelper::random_int(0, 100) <= drop.chance )
      {
        AmountedItem item( std::shared_ptr<Item>( BUTCHER.actorsDatabase().createActor<Item>(drop.itemId) ),
                      cc::RandomHelper::random_int(drop.amountMin, drop.amountMax) );

        destroyer->performAction( new PickUpAction(item) );
      }
    }
  }

  Character::onDestroy(destroyer);
}

void Monster::setHp(int hp)
{
  Character::setHp(hp);

  if ( _hpBar )
    _hpBar->setPercent((float)getHp() / (float)getMaxHp() * 100);
}

bool Monster::canShootAt(cocos2d::Vec2 coord)
{
  if (_range > 0 )
    return calculateDistance(getTileCoord(), coord) <= _range;

  return false;
}

ActorID Monster::getAmmoID() const
{
  if ( _ammoId == ActorID::INVALID )
    return ActorID::BONE;

  return _ammoId;
}

void Monster::refresh()
{
  Actor::refresh();
  drawNameAndHp();
}

void Monster::drawNameAndHp()
{
  if ( getSprite() )
  {
    auto order = getSprite()->getGlobalZOrder();
    //Monster name
    cocos2d::Label* label = make_label(getName(), cc::Color4B::WHITE, 12, cc::Vec2(0.5,0));
    label->setPosition( 32, 64 );

    label->setGlobalZOrder(order+1);
    getSprite()->addChild(label, 1);

    //Monster HP
    cc::Sprite* barBg = cc::Sprite::create();
    barBg->initWithFile("images/progress_bar_bg_4.png");
    barBg->setAnchorPoint(cc::Vec2(0.5,0));
    barBg->setPosition( cc::Vec2(label->getPositionX(),
                                  label->getPositionY() + label->getBoundingBox().size.height) ); //Damn, why on create sprite size is 0? TODO - remove this hardcode.
    barBg->setGlobalZOrder(order+1);
    getSprite()->addChild(barBg);

    _hpBar = cc::ui::LoadingBar::create();
    _hpBar->loadTexture("images/progress_bar_red_4.png");
    _hpBar->setPercent(100);
    _hpBar->setAnchorPoint(cc::Vec2(0.5,0));
    _hpBar->setGlobalZOrder(order+2);
    _hpBar->setPosition(barBg->getPosition());
    getSprite()->addChild(_hpBar);

  }
}

void Monster::setSprite(cocos2d::Sprite *sprite)
{
  Actor::setSprite(sprite);
  drawNameAndHp();
}

}
