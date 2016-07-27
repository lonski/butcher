#ifndef ITEM_SLOT_TYPE_H
#define ITEM_SLOT_TYPE_H

#include <string>

namespace butcher {

enum class ItemSlotType{
  NONE,
  WEAPON,
  ARMOR,
  GLOVES,
  BOOTS,

  END
};

static inline std::string ItemSlotType2Str(ItemSlotType t)
{
  switch(t)
  {
    case ItemSlotType::WEAPON:
      return "Weapon";
    case ItemSlotType::ARMOR:
      return "Armor";
    case ItemSlotType::GLOVES:
      return "Gloves";
    case ItemSlotType::BOOTS:
      return "Boots";
    default:;
  }
  return "";
}

inline ItemSlotType operator++(ItemSlotType& x) { return x = (ItemSlotType)(std::underlying_type<ItemSlotType>::type(x) + 1); }
inline ItemSlotType operator*(ItemSlotType c) {return c;}
inline ItemSlotType begin(ItemSlotType) {return ItemSlotType::NONE;}
inline ItemSlotType end(ItemSlotType)   {return ItemSlotType::END;}

}

#endif // ITEM_SLOT_TYPE_H
