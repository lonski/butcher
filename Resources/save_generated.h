// automatically generated, do not modify

#include "flatbuffers/flatbuffers.h"

namespace butcher {

struct InventoryData;
struct RecipeData;
struct CraftbookData;
struct SaveData;

struct InventoryData : private flatbuffers::Table {
  int32_t item_id() const { return GetField<int32_t>(4, 0); }
  int32_t amount() const { return GetField<int32_t>(6, 0); }
  int32_t equipped() const { return GetField<int32_t>(8, 0); }
};

struct InventoryDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_item_id(int32_t item_id) { fbb_.AddElement<int32_t>(4, item_id, 0); }
  void add_amount(int32_t amount) { fbb_.AddElement<int32_t>(6, amount, 0); }
  void add_equipped(int32_t equipped) { fbb_.AddElement<int32_t>(8, equipped, 0); }
  InventoryDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<InventoryData> Finish() { return flatbuffers::Offset<InventoryData>(fbb_.EndTable(start_, 3)); }
};

inline flatbuffers::Offset<InventoryData> CreateInventoryData(flatbuffers::FlatBufferBuilder &_fbb, int32_t item_id, int32_t amount, int32_t equipped) {
  InventoryDataBuilder builder_(_fbb);
  builder_.add_equipped(equipped);
  builder_.add_amount(amount);
  builder_.add_item_id(item_id);
  return builder_.Finish();
}

struct RecipeData : private flatbuffers::Table {
  int32_t recipe_id() const { return GetField<int32_t>(4, 0); }
};

struct RecipeDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recipe_id(int32_t recipe_id) { fbb_.AddElement<int32_t>(4, recipe_id, 0); }
  RecipeDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<RecipeData> Finish() { return flatbuffers::Offset<RecipeData>(fbb_.EndTable(start_, 1)); }
};

inline flatbuffers::Offset<RecipeData> CreateRecipeData(flatbuffers::FlatBufferBuilder &_fbb, int32_t recipe_id) {
  RecipeDataBuilder builder_(_fbb);
  builder_.add_recipe_id(recipe_id);
  return builder_.Finish();
}

struct CraftbookData : private flatbuffers::Table {
  int32_t free_points() const { return GetField<int32_t>(4, 0); }
  const flatbuffers::Vector<flatbuffers::Offset<RecipeData>> *recipes() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<RecipeData>> *>(6); }
};

struct CraftbookDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_free_points(int32_t free_points) { fbb_.AddElement<int32_t>(4, free_points, 0); }
  void add_recipes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RecipeData>>> recipes) { fbb_.AddOffset(6, recipes); }
  CraftbookDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<CraftbookData> Finish() { return flatbuffers::Offset<CraftbookData>(fbb_.EndTable(start_, 2)); }
};

inline flatbuffers::Offset<CraftbookData> CreateCraftbookData(flatbuffers::FlatBufferBuilder &_fbb, int32_t free_points, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RecipeData>>> recipes) {
  CraftbookDataBuilder builder_(_fbb);
  builder_.add_recipes(recipes);
  builder_.add_free_points(free_points);
  return builder_.Finish();
}

struct SaveData : private flatbuffers::Table {
  uint32_t id() const { return GetField<uint32_t>(4, 0); }
  int32_t type() const { return GetField<int32_t>(6, 0); }
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
  const InventoryData *inventory() const { return GetPointer<const InventoryData *>(30); }
  const CraftbookData *craftbook() const { return GetPointer<const CraftbookData *>(32); }
};

struct SaveDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) { fbb_.AddElement<uint32_t>(4, id, 0); }
  void add_type(int32_t type) { fbb_.AddElement<int32_t>(6, type, 0); }
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
  void add_inventory(flatbuffers::Offset<InventoryData> inventory) { fbb_.AddOffset(30, inventory); }
  void add_craftbook(flatbuffers::Offset<CraftbookData> craftbook) { fbb_.AddOffset(32, craftbook); }
  SaveDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<SaveData> Finish() { return flatbuffers::Offset<SaveData>(fbb_.EndTable(start_, 15)); }
};

inline flatbuffers::Offset<SaveData> CreateSaveData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, int32_t type, flatbuffers::Offset<flatbuffers::String> name, uint8_t blocks, uint8_t transparent, flatbuffers::Offset<flatbuffers::String> sprite_file, uint32_t hp, uint16_t attack, uint16_t defense, flatbuffers::Offset<flatbuffers::String> damage, uint16_t damage_reduction, int32_t exp, int32_t level, flatbuffers::Offset<InventoryData> inventory, flatbuffers::Offset<CraftbookData> craftbook) {
  SaveDataBuilder builder_(_fbb);
  builder_.add_craftbook(craftbook);
  builder_.add_inventory(inventory);
  builder_.add_level(level);
  builder_.add_exp(exp);
  builder_.add_damage(damage);
  builder_.add_hp(hp);
  builder_.add_sprite_file(sprite_file);
  builder_.add_name(name);
  builder_.add_type(type);
  builder_.add_id(id);
  builder_.add_damage_reduction(damage_reduction);
  builder_.add_defense(defense);
  builder_.add_attack(attack);
  builder_.add_transparent(transparent);
  builder_.add_blocks(blocks);
  return builder_.Finish();
}

inline const SaveData *GetSaveData(const void *buf) { return flatbuffers::GetRoot<SaveData>(buf); }

}; // namespace butcher
