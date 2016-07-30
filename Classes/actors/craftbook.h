#ifndef CRAFTBOOK_H
#define CRAFTBOOK_H

#include <actors/recipe.h>
#include <memory>

namespace butcher {

class CraftBook
{
public:
  CraftBook();

  bool addRecipe(std::shared_ptr<Recipe> recipe);
  bool removeRecipe(RecipeID id);

  std::vector<std::shared_ptr<Recipe>> getRecipes() const;
  int getCraftLevel(CraftType type);
  int getSpentPoints(CraftType ct);

  int getFreePoints() const;
  void setFreePoints(int freePoints);

  bool hasRecipe(RecipeID r) const;

private:
  std::map<RecipeID, std::shared_ptr<Recipe> > _recipes;
  std::map<CraftType, int> _craftLevels;
  int _freePoints;

};

}

#endif // CRAFTBOOK_H
