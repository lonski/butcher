#ifndef WAYPOINT_VIEW_H
#define WAYPOINT_VIEW_H

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"

namespace butcher {

class WaypointView : public cocos2d::Layer
{
public:
  WaypointView();

  static cocos2d::Scene* createScene();
  static WaypointView* create();
  virtual bool init();

private:
  cocos2d::Vec2 _origin;
  cocos2d::Size _visibleSize;
  float _margin;

  void addComponents();
  cocos2d::ui::Button* makeListItem(int level);
};

}

#endif // WAYPOINT_VIEW_H
