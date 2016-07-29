#ifndef CRAFT_VIEW_H
#define CRAFT_VIEW_H

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"
#include "ui/UILoadingBar.h"
#include <actors/craft_type.h>

namespace butcher {

class Player;
class Recipe;

class CraftView : public cocos2d::Layer
{
public:
  CraftView(std::shared_ptr<Player> player);

  static cocos2d::Scene* createScene(std::shared_ptr<Player> player);
  static CraftView* create(std::shared_ptr<Player> player);
  virtual bool init();

private:
  std::shared_ptr<Player> _player;

  cocos2d::Vec2 _origin;
  cocos2d::Size _visibleSize;
  float _margin;

  cocos2d::ui::Layout* _root;
  cocos2d::ui::Layout* _craftInfoPanel;
  cocos2d::ui::Layout* _recipesPanel;
  cocos2d::ui::Layout* _workbenchPanel;
  cocos2d::ui::Layout* _workbenchItemPanel;
  cocos2d::ui::ListView* _recipeList;

  std::shared_ptr<Recipe> _selectedRecipe;

  CraftType _selectedCraft;
  cocos2d::MenuItemImage* _smithingBtn;
  cocos2d::MenuItemImage* _tanningBtn;
  cocos2d::MenuItemImage* _cookingBtn;
  cocos2d::MenuItemImage* _engineeringBtn;
  cocos2d::ui::LoadingBar* _craftLevelBar;
  cocos2d::Label* _craftLevelLabel;
  cocos2d::Label* _craftPointsLabel;
  cocos2d::ui::Button* _learnBtn;

  void addComponents();
  void createRootLayout();
  void createCloseButton();
  void createCraftInfoPanel();
  int createCraftMenuButtons();
  int createCraftLevelInfo();
  int createCraftPointsInfo();
  void createRecipesPanel();
  void createWorkbenchPanel();

  void fillRecipeList();
  void fillWorkbench();
  void fillWorkbenchItem();
  void fillWorkbenchIngridients();
  void fillCraftInfoPanel();

  void craftChooseSmithing(cocos2d::Ref*);
  void craftChooseTanning(cocos2d::Ref*);
  void craftChooseCooking(cocos2d::Ref*);
  void craftChooseEngineering(cocos2d::Ref*);
  void chooseCraftCommon();

  cocos2d::ui::Button* makeListItem(const std::string& title, const std::string& sprite_fn);
  cocos2d::ui::Button* makeIngridientListItem(const std::string& title, const std::string& sprite_fn,
                                              cocos2d::Color4B color = cocos2d::Color4B::WHITE);
  void showMessage(const std::string &msg, cocos2d::Color4B color);
};

}


#endif // CRAFT_VIEW_H
