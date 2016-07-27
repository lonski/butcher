#ifndef RECIPE_H
#define RECIPE_H

#include <actors/inventory.h>
#include <actors/craft_type.h>
#include <actors/recipe_id.h>
#include <data/recipes_generated.h>

namespace butcher {

class Recipe
{
public:
  typedef std::map<ActorID, int> IngridientList;

  Recipe();
  static std::unique_ptr<Recipe> create(const RecipeData* data);
  std::unique_ptr<Recipe> clone();

  bool produce();

  IngridientList getIngridientList() const;
  bool checkIngridients();

  int getLevel() const;
  CraftType getType() const;
  int getSuccessChance() const;
  int getCost() const;
  void setInventory(Inventory* inv);

private:
  Recipe(const Recipe&) = delete;
  Recipe& operator=(const Recipe&) = delete;

  IngridientList _ingridientList;
  Inventory* _inventory;
  AmountedItem _product;
  int _successChance;
  int _level;
  CraftType _type;
  int _cost;
  RecipeID _id;

};

}

#endif // RECIPE_H
