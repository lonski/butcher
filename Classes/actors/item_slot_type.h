#ifndef ITEM_SLOT_TYPE_H
#define ITEM_SLOT_TYPE_H

namespace butcher {

enum class ItemSlotType{
  NONE,
  WEAPON,
  ARMOR,
  GLOVES,
  BOOTS,

  END
};

inline ItemSlotType operator++(ItemSlotType& x) { return x = (ItemSlotType)(std::underlying_type<ItemSlotType>::type(x) + 1); }
inline ItemSlotType operator*(ItemSlotType c) {return c;}
inline ItemSlotType begin(ItemSlotType) {return ItemSlotType::NONE;}
inline ItemSlotType end(ItemSlotType)   {return ItemSlotType::END;}

}

#endif // ITEM_SLOT_TYPE_H
