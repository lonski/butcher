#include "trap.h"
#include <actors/character.h>
#include <butcher.h>
#include <dungeon/dungeon_state.h>
#include <actors/effects/effect.h>

namespace cc = cocos2d;

namespace butcher {

Trap::Trap(const ActorData *data)
  : Item(data)
{
}

std::unique_ptr<Actor> Trap::clone(std::unique_ptr<Actor> allocated)
{
  Trap* o = dynamic_cast<Trap*>(allocated.release());
  if ( o == nullptr )
    o = new Trap(nullptr);

  return std::move(Item::clone(std::unique_ptr<Actor>{o}));
}

void Trap::onInterract(std::shared_ptr<Actor> actor)
{
  fadeText( "Triggered!", cc::Color4B::YELLOW, 1);

  if (getEffectID() != EffectID::None )
    actor->addEffect( BUTCHER.effectsDatabase().createEffect(getEffectID()) );

  std::shared_ptr<Character> c = std::dynamic_pointer_cast<Character>(actor);
  if ( c && getDamage().toInt() > 0 )
    c->takeDamage(getDamage(), shared_from_this());

  cc::Sprite* s = getSprite().release();

  for ( auto c : s->getChildren() )
    c->runAction( cc::Sequence::create(cc::FadeOut::create(0.5), cc::RemoveSelf::create(), nullptr) );

  s->runAction( cc::Sequence::create(cc::FadeOut::create(0.5), cc::RemoveSelf::create(), nullptr) );

  if ( !BUTCHER.getCurrentDungeon()->removeActor(shared_from_this(), false) )
    cc::log("%s Failed to remove actor!", __PRETTY_FUNCTION__);

}

}
