// automatically generated, do not modify
#ifndef cycki_h
#define cycki_h
#include "flatbuffers/flatbuffers.h"

namespace butcher {

enum {
  ActorType_Monster = 0,
  ActorType_Player = 1,
  ActorType_Item = 2,
  ActorType_Object = 3,
  ActorType_StairsDown = 4,
  ActorType_StairsUp = 5,
  ActorType_Door = 6,
};

inline const char **EnumNamesActorType() {
  static const char *names[] = { "Monster", "Player", "Item", "Object", "StairsDown", "StairsUp", "Door", nullptr };
  return names;
}

inline const char *EnumNameActorType(int e) { return EnumNamesActorType()[e]; }

struct ActorsData;
struct DropRuleData;
struct ActorData;

struct ActorsData : private flatbuffers::Table {
  const flatbuffers::Vector<flatbuffers::Offset<ActorData>> *actors() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ActorData>> *>(4); }
};

struct ActorsDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_actors(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ActorData>>> actors) { fbb_.AddOffset(4, actors); }
  ActorsDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<ActorsData> Finish() { return flatbuffers::Offset<ActorsData>(fbb_.EndTable(start_, 1)); }
};

inline flatbuffers::Offset<ActorsData> CreateActorsData(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ActorData>>> actors) {
  ActorsDataBuilder builder_(_fbb);
  builder_.add_actors(actors);
  return builder_.Finish();
}

struct DropRuleData : private flatbuffers::Table {
  uint32_t item_id() const { return GetField<uint32_t>(4, 0); }
  uint16_t chance() const { return GetField<uint16_t>(6, 0); }
  uint16_t amount_min() const { return GetField<uint16_t>(8, 0); }
  uint16_t amount_max() const { return GetField<uint16_t>(10, 0); }
};

struct DropRuleDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_item_id(uint32_t item_id) { fbb_.AddElement<uint32_t>(4, item_id, 0); }
  void add_chance(uint16_t chance) { fbb_.AddElement<uint16_t>(6, chance, 0); }
  void add_amount_min(uint16_t amount_min) { fbb_.AddElement<uint16_t>(8, amount_min, 0); }
  void add_amount_max(uint16_t amount_max) { fbb_.AddElement<uint16_t>(10, amount_max, 0); }
  DropRuleDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<DropRuleData> Finish() { return flatbuffers::Offset<DropRuleData>(fbb_.EndTable(start_, 4)); }
};

inline flatbuffers::Offset<DropRuleData> CreateDropRuleData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t item_id, uint16_t chance, uint16_t amount_min, uint16_t amount_max) {
  DropRuleDataBuilder builder_(_fbb);
  builder_.add_item_id(item_id);
  builder_.add_amount_max(amount_max);
  builder_.add_amount_min(amount_min);
  builder_.add_chance(chance);
  return builder_.Finish();
}

struct ActorData : private flatbuffers::Table {
  uint32_t id() const { return GetField<uint32_t>(4, 0); }
  int8_t type() const { return GetField<int8_t>(6, 0); }
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(8); }
  uint8_t blocks() const { return GetField<uint8_t>(10, 0); }
  uint8_t transparent() const { return GetField<uint8_t>(12, 0); }
  const flatbuffers::String *sprite_file() const { return GetPointer<const flatbuffers::String *>(14); }
  uint32_t hp() const { return GetField<uint32_t>(16, 0); }
  uint16_t attack() const { return GetField<uint16_t>(18, 0); }
  uint16_t defense() const { return GetField<uint16_t>(20, 0); }
  const flatbuffers::String *damage() const { return GetPointer<const flatbuffers::String *>(22); }
  uint16_t damage_reduction() const { return GetField<uint16_t>(24, 0); }
  int32_t exp() const { return GetField<int32_t>(26, 0); }
  int32_t level() const { return GetField<int32_t>(28, 0); }
  int32_t break_chance() const { return GetField<int32_t>(30, 0); }
  int32_t body_slot() const { return GetField<int32_t>(32, 0); }
  int32_t category() const { return GetField<int32_t>(34, 0); }
  int32_t range() const { return GetField<int32_t>(36, 0); }
  int32_t ammo_id() const { return GetField<int32_t>(38, 0); }
  const flatbuffers::Vector<flatbuffers::Offset<DropRuleData>> *drop_rules() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<DropRuleData>> *>(40); }
  uint8_t is_closed() const { return GetField<uint8_t>(42, 0); }
  const flatbuffers::String *opened_sprite_file() const { return GetPointer<const flatbuffers::String *>(44); }
};

struct ActorDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) { fbb_.AddElement<uint32_t>(4, id, 0); }
  void add_type(int8_t type) { fbb_.AddElement<int8_t>(6, type, 0); }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(8, name); }
  void add_blocks(uint8_t blocks) { fbb_.AddElement<uint8_t>(10, blocks, 0); }
  void add_transparent(uint8_t transparent) { fbb_.AddElement<uint8_t>(12, transparent, 0); }
  void add_sprite_file(flatbuffers::Offset<flatbuffers::String> sprite_file) { fbb_.AddOffset(14, sprite_file); }
  void add_hp(uint32_t hp) { fbb_.AddElement<uint32_t>(16, hp, 0); }
  void add_attack(uint16_t attack) { fbb_.AddElement<uint16_t>(18, attack, 0); }
  void add_defense(uint16_t defense) { fbb_.AddElement<uint16_t>(20, defense, 0); }
  void add_damage(flatbuffers::Offset<flatbuffers::String> damage) { fbb_.AddOffset(22, damage); }
  void add_damage_reduction(uint16_t damage_reduction) { fbb_.AddElement<uint16_t>(24, damage_reduction, 0); }
  void add_exp(int32_t exp) { fbb_.AddElement<int32_t>(26, exp, 0); }
  void add_level(int32_t level) { fbb_.AddElement<int32_t>(28, level, 0); }
  void add_break_chance(int32_t break_chance) { fbb_.AddElement<int32_t>(30, break_chance, 0); }
  void add_body_slot(int32_t body_slot) { fbb_.AddElement<int32_t>(32, body_slot, 0); }
  void add_category(int32_t category) { fbb_.AddElement<int32_t>(34, category, 0); }
  void add_range(int32_t range) { fbb_.AddElement<int32_t>(36, range, 0); }
  void add_ammo_id(int32_t ammo_id) { fbb_.AddElement<int32_t>(38, ammo_id, 0); }
  void add_drop_rules(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<DropRuleData>>> drop_rules) { fbb_.AddOffset(40, drop_rules); }
  void add_is_closed(uint8_t is_closed) { fbb_.AddElement<uint8_t>(42, is_closed, 0); }
  void add_opened_sprite_file(flatbuffers::Offset<flatbuffers::String> opened_sprite_file) { fbb_.AddOffset(44, opened_sprite_file); }
  ActorDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<ActorData> Finish() { return flatbuffers::Offset<ActorData>(fbb_.EndTable(start_, 21)); }
};

inline flatbuffers::Offset<ActorData> CreateActorData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, int8_t type, flatbuffers::Offset<flatbuffers::String> name, uint8_t blocks, uint8_t transparent, flatbuffers::Offset<flatbuffers::String> sprite_file, uint32_t hp, uint16_t attack, uint16_t defense, flatbuffers::Offset<flatbuffers::String> damage, uint16_t damage_reduction, int32_t exp, int32_t level, int32_t break_chance, int32_t body_slot, int32_t category, int32_t range, int32_t ammo_id, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<DropRuleData>>> drop_rules, uint8_t is_closed, flatbuffers::Offset<flatbuffers::String> opened_sprite_file) {
  ActorDataBuilder builder_(_fbb);
  builder_.add_opened_sprite_file(opened_sprite_file);
  builder_.add_drop_rules(drop_rules);
  builder_.add_ammo_id(ammo_id);
  builder_.add_range(range);
  builder_.add_category(category);
  builder_.add_body_slot(body_slot);
  builder_.add_break_chance(break_chance);
  builder_.add_level(level);
  builder_.add_exp(exp);
  builder_.add_damage(damage);
  builder_.add_hp(hp);
  builder_.add_sprite_file(sprite_file);
  builder_.add_name(name);
  builder_.add_id(id);
  builder_.add_damage_reduction(damage_reduction);
  builder_.add_defense(defense);
  builder_.add_attack(attack);
  builder_.add_is_closed(is_closed);
  builder_.add_transparent(transparent);
  builder_.add_blocks(blocks);
  builder_.add_type(type);
  return builder_.Finish();
}

inline const ActorsData *GetActorsData(const void *buf) { return flatbuffers::GetRoot<ActorsData>(buf); }

}; // namespace butcher
#endif
