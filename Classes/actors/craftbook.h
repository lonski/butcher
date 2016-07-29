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

private:
  std::map<RecipeID, std::shared_ptr<Recipe> > _recipes;
  std::map<CraftType, int> _craftLevels;
  int calculatePoints(CraftType ct);

};

}

#endif // CRAFTBOOK_H
