#ifndef BSPDUNGEONGENERATOR_H
#define BSPDUNGEONGENERATOR_H

#include <memory>
#include <dungeon/generators/dungeon_generator.h>
#include <utils/grid.h>
#include <dungeon/dungeon_description.h>
#include <data/levels_generated.h>

namespace butcher {

class BSPDungeonGenerator : public DungeonGenerator
{
public:
  BSPDungeonGenerator();

  bool generate(DungeonDescription& description);

private:
  struct Node
  {
    Node(std::shared_ptr<Node> parent_, int x1, int x2, int y1, int y2)
      : x1(x1)
      , x2(x2)
      , y1(y1)
      , y2(y2)
      , parent(parent_)
    {
    }

    int x1,x2,y1,y2;

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::shared_ptr<Node> parent;
    std::shared_ptr<Rect> room;

    int width() const {
      return x2 - x1;
    }

    int height() const {
      return y2 - y1;
    }
  };

  struct Tree
  {
    std::shared_ptr<Node> root;

    void leaf_walk(std::shared_ptr<Node> node,
                   std::function<void(std::shared_ptr<Node>)> fun)
    {
      if ( node )
      {
        if ( !node->left && !node->right )
          fun(node);

        leaf_walk(node->left, fun);
        leaf_walk(node->right, fun);
      }
    }

    void reverse_level_order(std::shared_ptr<Node> node,
                             std::function<void(std::shared_ptr<Node>)> fun)
    {
        int h = height(node);
        int i;
        for (i=h; i>=1; i--)
            level_operation(node, i, fun);
    }

    int height(std::shared_ptr<Node> node)
    {
      if (!node)
        return 0;

      return std::max(height(node->left) + 1, height(node->right) + 1);
    }

    void level_operation(std::shared_ptr<Node> node, int level,
                         std::function<void(std::shared_ptr<Node>)> fun)
    {
      if (!node)
        return;

      if (level == 1)
      {
        fun(node);
      }
      else if (level > 1)
      {
        level_operation(node->left, level-1, fun);
        level_operation(node->right, level-1,fun);
      }
    }

  };

  DungeonDescription* _data;
  Tree _tree;

  void partition(std::shared_ptr<Node> node, int minSize);
  void putRooms();
  void drawCorridors();
  void connectRooms(Rect& r1, Rect& r2);
  void connectSections(std::shared_ptr<Node> s1, std::shared_ptr<Node> s2);

  cocos2d::Vec2 carve(Direction::Symbol d, cocos2d::Vec2 start);
  bool tileSearch(cocos2d::Vec2 pos, cocos2d::Vec2 goal, char tileToFind);

  std::set<std::shared_ptr<Node>> _connected;
};

}

#endif // BSPDUNGEONGENERATOR_H
