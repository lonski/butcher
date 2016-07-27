#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include <actors/inventory.h>
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"

namespace butcher {

class Player;

class InventoryView : public cocos2d::Layer
{
public:
  InventoryView(std::shared_ptr<Player> player);

  static cocos2d::Scene* createScene(std::shared_ptr<Player> player);
  static InventoryView* create(std::shared_ptr<Player> player);
  virtual bool init();

private:
  enum class ItemAction {
    NONE,
    USE,
    DESTROY,
    EQUIP
  };

  std::shared_ptr<Player> _player;

  cocos2d::Vec2 _origin;
  cocos2d::Size _visibleSize;
  float _margin;

  cocos2d::ui::Layout* _root;
  cocos2d::ui::Layout* _topPanel;
  cocos2d::ui::Layout* _topLeftPanel;
  cocos2d::ui::Layout* _topRightPanel;
  cocos2d::ui::Layout* _bottomPanel;
  cocos2d::ui::ListView* _itemList;

  cocos2d::ui::Button* _wpnSlot;
  cocos2d::ui::Button* _armorSlot;
  cocos2d::ui::Button* _glovesSlot;
  cocos2d::ui::Button* _bootsSlot;

  void addComponents();

  void createRootLayout();
  void createTopPanel();
  void createTopLeftPanel();
  void createTopRightPanel();
  void createBottomPanel();

  void fillCharacterInfo();
  cocos2d::ui::Button * makeListItem(const std::string &title, const std::string &sprite_fn);
  void fillInventoryItems();
  void createBodySlots();
  void createCloseButton();

  void chooseItemAction(const AmountedItem& item);
};

}

#endif // INVENTORYVIEW_H
