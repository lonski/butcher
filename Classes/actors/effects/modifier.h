#ifndef MODIFIER_H
#define MODIFIER_H

#include <actors/attribute_type.h>
#include <actors/effects/special_modifier_type.h>

namespace butcher {

struct Modifier
{
  Modifier(AttributeType atr = AttributeType::None,
           int v = 0,
           SpecialModifierType spc = SpecialModifierType::None )
    : attribute(atr)
    , value(v)
    , special(spc)
  {}

  SpecialModifierType special;
  AttributeType attribute;
  int value;
};

}

#endif // MODIFIER_H
