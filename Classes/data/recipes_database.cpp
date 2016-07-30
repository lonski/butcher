#include "recipes_database.h"
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

RecipeDatabase::RecipeDatabase()
{
}

bool RecipeDatabase::load(const std::string &fn)
{
  cc::Data file_data = cc::FileUtils::getInstance()->getDataFromFile(fn);

  if ( file_data.getSize() == 0 )
  {
    cc::log("%s: failed to load file.", __PRETTY_FUNCTION__);
    return false;
  }

  const RecipesData* data = GetRecipesData( file_data.getBytes() );

  if ( data == nullptr )
  {
    cc::log("%s: failed to load recipes.", __PRETTY_FUNCTION__);
    return false;
  }

  auto recipes = data->recipes();
  for (unsigned i = 0; i < recipes->Length(); ++i)
  {
    const RecipeData* d = recipes->Get(i);
    _recipeTemplates[ static_cast<RecipeID>(d->id()) ] = Recipe::create(d);
  }

  cc::log("%s: loaded %lu recipe templates.", __PRETTY_FUNCTION__, _recipeTemplates.size());

  return true;
}

std::unique_ptr<Recipe> RecipeDatabase::createRecipe(RecipeID id)
{
  auto it = _recipeTemplates.find(id);
  if ( it != _recipeTemplates.end() )
  {
    return it->second->clone();
  }
  else
  {
    cocos2d::log("%s: failed to find recipe id=%u.", __PRETTY_FUNCTION__, id);
  }

  return nullptr;
}

std::vector<std::shared_ptr<Recipe>> RecipeDatabase::getAllRecipes()
{
  std::vector<std::shared_ptr<Recipe>> recs;

  for ( auto& kv : _recipeTemplates )
    recs.push_back(kv.second->clone());

  return recs;
}

}
