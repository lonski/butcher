#include "craftbook.h"

namespace butcher {

CraftBook::CraftBook()
{
}

bool CraftBook::addRecipe(std::shared_ptr<Recipe> recipe)
{
  auto it = _recipes.find(recipe->getId());

  if ( it == _recipes.end() )
  {
    _recipes[ recipe->getId() ] = recipe;

    //check if increase craft level
    int points = calculatePoints(recipe->getType());
    int currentLevel = getCraftLevel(recipe->getType());
    if ( getCraftLevelPointsNeeded(recipe->getType(), currentLevel + 1) <= points )
      _craftLevels[ recipe->getType() ] = currentLevel + 1;

    return true;
  }

  return false;
}

bool CraftBook::removeRecipe(RecipeID id)
{
  auto it = _recipes.find(id);

  if ( it != _recipes.end() )
  {
    _recipes.erase(it);
    return true;
  }

  return false;
}

std::vector<std::shared_ptr<Recipe> > CraftBook::getRecipes() const
{
  std::vector<std::shared_ptr<Recipe>> recs;

  for ( auto& kv : _recipes )
    recs.push_back(kv.second);

  return recs;
}

int CraftBook::getCraftLevel(CraftType type)
{
  return _craftLevels[ type ];
}

int CraftBook::calculatePoints(CraftType ct)
{
  int sum = 0;
  for ( auto& kv : _recipes )
  {
    std::shared_ptr<Recipe> r = kv.second;
    if ( r->getType() == ct )
      sum += r->getCost();
  }
  return sum;
}

}
