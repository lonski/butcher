#include "bsp_dungeon_generator.h"
#include "cocos2d.h"
#include <dungeon/room.h>

namespace cc = cocos2d;

namespace butcher {

BSPDungeonGenerator::BSPDungeonGenerator()
  : _data(nullptr)
{

}

bool BSPDungeonGenerator::generate(DungeonDescription &description)
{
  _data = &description;
  _data->grid = Grid( _data->settings->width(), _data->settings->height());
  _data->rooms.clear();

  _tree = Tree();
  _tree.root.reset( new Node(nullptr, 0, _data->grid.width  - 1,
                                      0, _data->grid.height - 1) );

  partition(_tree.root, _data->settings->max_room_size());
  putRooms();
  drawCorridors();

  Grid g = _data->grid;
  auto l = g.tiles.find(Tiles::FLOOR);
  g.floodfill(cc::Vec2((int)l%g.width,(int)l/g.width), '+');

//  _tree.leaf_walk(_tree.root, [&](std::shared_ptr<Node> leaf){
//    for ( int x = leaf->x1; x < leaf->x2; ++x)
//    {
//      g.set(x, leaf->y1, '^');
//      g.set(x, leaf->y2, '^');
//    }

//    for ( int y = leaf->y1; y < leaf->y2; ++y)
//    {
//      g.set(leaf->x1, y, '^');
//      g.set(leaf->x2, y, '^');
//    }
//  });

//  if ( g.tiles.find('.') != std::string::npos)
//  {
//    cc::log("ERRR!");
//    cc::log("%s", g.toStr().c_str());
//  }

  return true;
}

void BSPDungeonGenerator::partition(std::shared_ptr<BSPDungeonGenerator::Node> node, int minSize)
{
  int width = node->x2 - node->x1;
  int height = node->y2 - node->y1;

  if ( width > minSize*2 || height > minSize*2 )
  {
    int d = 0;

    if (width > minSize*2 && height > minSize*2)
      d = cc::RandomHelper::random_int(0,1);
    else if (width > minSize*2)
      d = 1;

    if ( d == 1) //vertical cut
    {
      int cut_x = cc::RandomHelper::random_int(node->x1 + width / 3, node->x2 - width / 3);

      if ( cut_x - node->x1 < minSize )
        cut_x = node->x1 + minSize;

      node->left.reset( new Node(node, node->x1, cut_x, node->y1, node->y2) );
      node->right.reset( new Node(node, cut_x, node->x2, node->y1, node->y2) );
    }

    else //horizontal cut
    {
      int cut_y = cc::RandomHelper::random_int(node->y1 + height / 3, node->y2 - height / 3);

      if ( cut_y - node->y1 < minSize )
        cut_y = node->y1 + minSize;

      node->left.reset( new Node(node, node->x1, node->x2, node->y1, cut_y) );
      node->right.reset( new Node(node, node->x1, node->x2, cut_y, node->y2) );
    }

    partition(node->left, minSize);
    partition(node->right, minSize);
  }
}

void BSPDungeonGenerator::putRooms()
{
  _tree.leaf_walk(_tree.root, [&](std::shared_ptr<Node> node){

    int w = node->x2 - node->x1 + 1;
    int h = node->y2 - node->y1 + 1;

    int rw = w / 2 + cc::RandomHelper::random_int(0, w / 2);
    int rh = h / 2 + cc::RandomHelper::random_int(0, h / 2);

    int roll = cocos2d::RandomHelper::random_int(0,100);

    if ( roll <= _data->settings->polygon_room_chance() && rh >= 10 && rw >= 10 )
      node->room.reset(new PolygonRoom(rw,rh));
    else
      node->room.reset(new RectRoom(rw,rh));

    cc::Vec2 s_center(node->x1 + w / 2, node->y1 + h / 2);
    cc::Vec2 r_pos( s_center.x - rw / 2, s_center.y - rh/2);

    node->room->put_into(_data->grid, r_pos);

    _data->rooms.push_back(node->room);
  });
}

cc::Vec2 BSPDungeonGenerator::carve(Direction::Symbol d, cc::Vec2 start)
{
  _data->grid.set(start, Tiles::FLOOR);
  return Direction::getNeighbour(start, d);
}

bool BSPDungeonGenerator::tileSearch(cocos2d::Vec2 pos, cocos2d::Vec2 goal, char tileToFind)
{
  while(pos != goal)
  {
    if (_data->grid.get(pos) == tileToFind)
      return true;

    cc::Vec2 dd = goal - pos;
    cc::Vec2 dd_unary( dd.x != 0 ? ( dd.x > 0 ? 1 : -1 ) : 0,
                       dd.y != 0 ? ( dd.y > 0 ? 1 : -1 ) : 0 );
    pos += dd_unary;

  }
  return false;
}

void BSPDungeonGenerator::connectSections(std::shared_ptr<Node> s1, std::shared_ptr<Node> s2)
{
  if ( _connected.find(s1) == _connected.end() && _connected.find(s2) == _connected.end() )
  {
    _connected.insert(s1);
    _connected.insert(s2);

    bool connectedHorizontaly = s1->y1 == s2->y1;
    bool connectedVertically  = s1->x1 == s2->x1;

    if ( connectedHorizontaly )
    {
      cc::Vec2 borderPoint(s1->x2,
                           cc::RandomHelper::random_int(s1->y1 + s1->height()/4, s1->y2 - s1->height()/4) );

      cc::Vec2 goal_l(s1->x1, borderPoint.y);
      cc::Vec2 goal_r(s2->x2, borderPoint.y);

      int c(100);
      while ( (!tileSearch(borderPoint, goal_l, Tiles::FLOOR ) ||
              !tileSearch(borderPoint, goal_r, Tiles::FLOOR )) && c-- )
      {
        borderPoint= cc::Vec2(s1->x2,
                              cc::RandomHelper::random_int(s1->y1 + s1->height()/4, s1->y2 - s1->height()/4) );

        goal_l = cc::Vec2(s1->x1, borderPoint.y);
        goal_r = cc::Vec2(s2->x2, borderPoint.y);
      }

      cc::Vec2 pos = borderPoint;

      while( _data->grid.get(pos) != Tiles::FLOOR && pos.x > s1->x1 )
      {
        pos = carve(Direction::West, pos);
      }

      pos = borderPoint;
      pos.x++;

      while( _data->grid.get(pos) != Tiles::FLOOR && pos.x < s2->x2 )
      {
        pos = carve(Direction::East, pos);
      }
      //_data->grid.set(borderPoint, '$');
    }
    else if ( connectedVertically )
    {
      cc::Vec2 borderPoint(cc::RandomHelper::random_int(s1->x1 + s1->width()/4, s1->x2 - s1->width()/4),
                           s1->y2);

      cc::Vec2 goal_t(borderPoint.x, s1->y1);
      cc::Vec2 goal_b(borderPoint.x, s2->y2);

      int c(100);
      while ( (!tileSearch(borderPoint, goal_t, Tiles::FLOOR ) ||
              !tileSearch(borderPoint, goal_b, Tiles::FLOOR )) && c-- )
      {
        borderPoint = cc::Vec2(cc::RandomHelper::random_int(s1->x1 + s1->width()/4, s1->x2 - s1->width()/4),
                               s1->y2);

        goal_t = cc::Vec2(borderPoint.x, s1->y1);
        goal_b = cc::Vec2(borderPoint.x, s2->y2);
      }

      cc::Vec2 pos = borderPoint;

      while( _data->grid.get(pos) != Tiles::FLOOR && pos.y > s1->y1 )
      {
        pos = carve(Direction::North, pos);
      }

      pos = borderPoint;
      pos.y++;

      while( _data->grid.get(pos) != Tiles::FLOOR && pos.y < s2->y2 )
      {
        pos = carve(Direction::South, pos);
      }

      //_data->grid.set(borderPoint, '%');
    }
  }

  if ( s1->parent && s1->parent->parent )
    connectSections(s1->parent->parent->left, s1->parent->parent->right);
}

void BSPDungeonGenerator::drawCorridors()
{
//  _tree.leaf_walk(_tree.root, [&](std::shared_ptr<Node> leaf){
//    if ( leaf->parent )
//    {
//      std::shared_ptr<Node> right = leaf->parent->right;
//      std::shared_ptr<Node> left  = leaf->parent->left;
//      if ( left->room && right->room )
//        connectRooms(*right->room, *left->room);
//    }
//  });

  _connected.clear();


  _tree.reverse_level_order(_tree.root, [&](std::shared_ptr<Node> node){
    if ( node->parent )
    {
      connectSections(node->parent->left, node->parent->right);
    }
  });



}

void BSPDungeonGenerator::connectRooms(Room &r1, Room &r2)
{
  cc::Vec2 from = r1.getCenter();
  cc::Vec2 to   = r2.getCenter();

  while( from != to )
  {
    cc::Vec2 diff = to - from;
    cc::Vec2 dd(
            diff.x != 0 ? (diff.x < 0 ? -1 : 1) : 0,
            diff.y != 0 ? (diff.y < 0 ? -1 : 1) : 0
          );

    if ( cc::RandomHelper::random_int(0,1) )
      from.x += dd.x;
    else
      from.y += dd.y;

    _data->grid.set(from, Tiles::FLOOR);
  }
}

}
