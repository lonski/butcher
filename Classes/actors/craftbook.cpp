#include "craftbook.h"
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

CraftBook::CraftBook()
  : _freePoints(0)
{
}

bool CraftBook::addRecipe(std::shared_ptr<Recipe> recipe)
{
  if ( !recipe )
  {
    cc::log("%s: recipe is null", __PRETTY_FUNCTION__);
    return false;
  }

  auto it = _recipes.find(recipe->getId());

  if ( it == _recipes.end() )
  {
    _recipes[ recipe->getId() ] = recipe;

    //check if increase craft level
    int points = getSpentPoints(recipe->getType());
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
  auto it = _craftLevels.find(type);
  if ( it == _craftLevels.end() )
    _craftLevels[ type ] = 1;

  return _craftLevels[ type ];
}

int CraftBook::getSpentPoints(CraftType ct)
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

int CraftBook::getFreePoints() const
{
  return _freePoints;
}

void CraftBook::setFreePoints(int freePoints)
{
  _freePoints = freePoints;
}

bool CraftBook::hasRecipe(RecipeID r) const
{
  return _recipes.find(r) != _recipes.end();
}

}
