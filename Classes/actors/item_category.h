#ifndef ITEM_CATEGORY_H
#define ITEM_CATEGORY_H

#include <string>

namespace butcher {

enum class ItemCategory {
  None,
  Ingridient,
  Weapon,
  Armor,
  Stew,
  Device
};

static inline std::string ItemCategory2Str(ItemCategory c)
{
  switch ( c )
  {
    case ItemCategory::Ingridient:
      return "Ingridient";
    case ItemCategory::Weapon:
      return "Weapon";
    case ItemCategory::Armor:
      return "Armor";
    case ItemCategory::Stew:
      return "Stew";
    case ItemCategory::Device:
      return "Device";
    default:;
  }

  return "";
}

}

#endif // ITEM_CATEGORY_H
