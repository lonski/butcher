// automatically generated, do not modify
#ifndef save_gen_h
#define save_gen_h
#include "flatbuffers/flatbuffers.h"

namespace butcher {

struct InventoryData;
struct CraftbookData;
struct SaveData;

struct InventoryData : private flatbuffers::Table {
  int32_t item_id() const { return GetField<int32_t>(4, 0); }
  int32_t amount() const { return GetField<int32_t>(6, 0); }
  uint8_t equipped() const { return GetField<uint8_t>(8, 0); }
};

struct InventoryDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_item_id(int32_t item_id) { fbb_.AddElement<int32_t>(4, item_id, 0); }
  void add_amount(int32_t amount) { fbb_.AddElement<int32_t>(6, amount, 0); }
  void add_equipped(uint8_t equipped) { fbb_.AddElement<uint8_t>(8, equipped, 0); }
  InventoryDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<InventoryData> Finish() { return flatbuffers::Offset<InventoryData>(fbb_.EndTable(start_, 3)); }
};

inline flatbuffers::Offset<InventoryData> CreateInventoryData(flatbuffers::FlatBufferBuilder &_fbb, int32_t item_id, int32_t amount, uint8_t equipped) {
  InventoryDataBuilder builder_(_fbb);
  builder_.add_amount(amount);
  builder_.add_item_id(item_id);
  builder_.add_equipped(equipped);
  return builder_.Finish();
}

struct CraftbookData : private flatbuffers::Table {
  int32_t free_points() const { return GetField<int32_t>(4, 0); }
  const flatbuffers::Vector<int32_t> *recipes() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(6); }
};

struct CraftbookDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_free_points(int32_t free_points) { fbb_.AddElement<int32_t>(4, free_points, 0); }
  void add_recipes(flatbuffers::Offset<flatbuffers::Vector<int32_t>> recipes) { fbb_.AddOffset(6, recipes); }
  CraftbookDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<CraftbookData> Finish() { return flatbuffers::Offset<CraftbookData>(fbb_.EndTable(start_, 2)); }
};

inline flatbuffers::Offset<CraftbookData> CreateCraftbookData(flatbuffers::FlatBufferBuilder &_fbb, int32_t free_points, flatbuffers::Offset<flatbuffers::Vector<int32_t>> recipes) {
  CraftbookDataBuilder builder_(_fbb);
  builder_.add_recipes(recipes);
  builder_.add_free_points(free_points);
  return builder_.Finish();
}

struct SaveData : private flatbuffers::Table {
  int32_t exp() const { return GetField<int32_t>(4, 0); }
  int32_t level() const { return GetField<int32_t>(6, 0); }
  int32_t dungeon_level() const { return GetField<int32_t>(8, 0); }
  int32_t quick_switch() const { return GetField<int32_t>(10, 0); }
  const flatbuffers::Vector<int32_t> *waypoints() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(12); }
  const flatbuffers::Vector<flatbuffers::Offset<InventoryData>> *inventory() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<InventoryData>> *>(14); }
  const CraftbookData *craftbook() const { return GetPointer<const CraftbookData *>(16); }
};

struct SaveDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_exp(int32_t exp) { fbb_.AddElement<int32_t>(4, exp, 0); }
  void add_level(int32_t level) { fbb_.AddElement<int32_t>(6, level, 0); }
  void add_dungeon_level(int32_t dungeon_level) { fbb_.AddElement<int32_t>(8, dungeon_level, 0); }
  void add_quick_switch(int32_t quick_switch) { fbb_.AddElement<int32_t>(10, quick_switch, 0); }
  void add_waypoints(flatbuffers::Offset<flatbuffers::Vector<int32_t>> waypoints) { fbb_.AddOffset(12, waypoints); }
  void add_inventory(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<InventoryData>>> inventory) { fbb_.AddOffset(14, inventory); }
  void add_craftbook(flatbuffers::Offset<CraftbookData> craftbook) { fbb_.AddOffset(16, craftbook); }
  SaveDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<SaveData> Finish() { return flatbuffers::Offset<SaveData>(fbb_.EndTable(start_, 7)); }
};

inline flatbuffers::Offset<SaveData> CreateSaveData(flatbuffers::FlatBufferBuilder &_fbb, int32_t exp, int32_t level, int32_t dungeon_level, int32_t quick_switch, flatbuffers::Offset<flatbuffers::Vector<int32_t>> waypoints, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<InventoryData>>> inventory, flatbuffers::Offset<CraftbookData> craftbook) {
  SaveDataBuilder builder_(_fbb);
  builder_.add_craftbook(craftbook);
  builder_.add_inventory(inventory);
  builder_.add_waypoints(waypoints);
  builder_.add_quick_switch(quick_switch);
  builder_.add_dungeon_level(dungeon_level);
  builder_.add_level(level);
  builder_.add_exp(exp);
  return builder_.Finish();
}

inline const SaveData *GetSaveData(const void *buf) { return flatbuffers::GetRoot<SaveData>(buf); }

}; // namespace butcher
#endif
