#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

#include <string>

namespace butcher {

enum class AttributeType
{
  None,
  Attack,
  Defense,
  DamageReduction,

  END
};

static inline std::string AttributeType2Str(AttributeType t)
{
  switch(t)
  {
    case AttributeType::Attack:
      return "Attack";
    case AttributeType::Defense:
      return "Defense";
    case AttributeType::DamageReduction:
      return "DR";
    default:;
  }
  return "";
}

inline AttributeType operator++(AttributeType& x) { return x = (AttributeType)(std::underlying_type<AttributeType>::type(x) + 1); }
inline AttributeType operator*(AttributeType c) {return c;}
inline AttributeType begin(AttributeType) {return AttributeType::None;}
inline AttributeType end(AttributeType)   {return AttributeType::END;}

}

#endif // ATTRIBUTE_TYPE_H
