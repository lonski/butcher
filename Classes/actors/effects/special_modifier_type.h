#ifndef SPECIAL_MODIFIER_TYPE_H
#define SPECIAL_MODIFIER_TYPE_H

namespace butcher {

enum class SpecialModifierType {
  None,
  Paralyzed
};

static inline std::string SpecialModifierType2Str(SpecialModifierType t)
{
  switch ( t )
  {
    case SpecialModifierType::Paralyzed:
      return "Paralyzed";
    default:;
  }

  return "";
}

}

#endif // SPECIAL_MODIFIER_TYPE_H
