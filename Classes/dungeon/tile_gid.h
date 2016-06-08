#ifndef TILE_GID_H
#define TILE_GID_H

namespace butcher {

enum class MetaTileGID {
  Collidable = 33,
  Green = 34,
  Shadow = 35,
  Unexplored = 36,
  Transparent = 37,
};

enum class CaveTileGID {
  Mid = 1,
  NONE,
  HPassage,
  VPassage,
  Top,
  Right,
  Left,
  Bottom,
  TopRight,
  TopLeft,
  BottomRight,
  BottomLeft,
  TopDeadEnd,
  BottomDeadEnd,
  RightDeadEnd,
  LeftDeadEnd
};

}

#endif // TILE_GID_H
