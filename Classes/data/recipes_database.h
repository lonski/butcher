#ifndef RECIPES_DATABASE_H
#define RECIPES_DATABASE_H

#include <string>
#include <map>
#include <data/recipes_generated.h>
#include <actors/recipe.h>

namespace butcher {

class RecipeDatabase
{
public:
  RecipeDatabase();
  bool load(const std::string& fn);

  std::unique_ptr<Recipe> createRecipe(RecipeID id);
  std::vector<std::shared_ptr<Recipe> > getAllRecipes();

private:
  std::map<RecipeID, std::unique_ptr<Recipe>> _recipeTemplates;
};

}

#endif // RECIPES_DATABASE_H
