#ifndef ACTOR_ID_H
#define ACTOR_ID_H

namespace butcher {

enum class ActorID
{
  INVALID = 0,
  PLAYER  = 1,
  COW     = 2,
  STAIRS_DOWN = 3,
  STAIRS_UP   = 4,
  MEAT = 5,
  BONE = 6,
  DUNGEON_DOOR,
  COW_SQUIRE,
  COW_COOK,
  COW_CATCHER,
  COW_FIGHTER,
  COW_ROGUE,
  COW_PEASANT,
  COW_FARMER,
  COW_FISHERMAN,
  CALF,
  COW_WORKER,
  BONE_DAGGER,
  BONE_CLUB = 19,
  SCAPULA = 20,
  HOOF    = 21,
  TENDON  = 22,
  RIB     = 23,
  SKIN    = 24,
  BONE_KNIFE = 25,
  BONE_STAFF = 26,
  RIB_RIPPER = 27,
  SPEAR      = 28,
  NUNCHAKU   = 29,
  SCYTHE     = 30,
  FLAIL      = 31,
  BATTLE_AXE = 32,
  SCAPULA_AXE = 33,
  BONE_SLING  = 34,
  RIB_BOW     = 35,
  SHORT_BONE_BOW = 36
};

}

#endif // ACTOR_ID_H
