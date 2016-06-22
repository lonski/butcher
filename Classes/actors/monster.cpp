#include "monster.h"
#include <butcher.h>
#include <actors/amounted_item.h>
#include <actors/actions/pickup_action.h>

namespace cc = cocos2d;

namespace butcher {

Monster::Monster(const ActorData* data)
  : Character(data)
{
  if ( data )
  {
    auto dropRules = data->drop_rules();
    for (unsigned i = 0; i < dropRules->Length(); ++i)
      _dropRules.push_back( DropRule(dropRules->Get(i)) );
  }
}

std::unique_ptr<Actor> Monster::clone(std::unique_ptr<Actor> allocated)
{
  Monster* p = dynamic_cast<Monster*>(allocated.release());
  if ( p == nullptr )
    p = new Monster(nullptr);

  p->_dropRules = _dropRules;

  return std::move( Character::clone(std::unique_ptr<Actor>{p}) );
}

void Monster::nextTurn()
{
  if ( !_ai )
    _ai.reset( new Ai(shared_from_this()) );

  _ai->update();
}

void Monster::onDestroy(std::shared_ptr<Actor> destroyer)
{
  for(DropRule& drop : _dropRules)
  {
    if ( cc::RandomHelper::random_int(0, 100) <= drop.chance )
    {
      AmountedItem item( std::shared_ptr<Item>( BUTCHER.actorsDatabase().createActor<Item>(drop.itemId) ),
                    cc::RandomHelper::random_int(drop.amountMin, drop.amountMax) );

      destroyer->performAction( PickUpAction(item) );
    }
  }
}

}
