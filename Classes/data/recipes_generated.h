// automatically generated, do not modify
#ifndef recipes_gen_h
#define recipes_gen_h
#include "flatbuffers/flatbuffers.h"

namespace butcher {

struct RecipesData;
struct IngridientData;
struct RecipeData;

struct RecipesData : private flatbuffers::Table {
  const flatbuffers::Vector<flatbuffers::Offset<RecipeData>> *recipes() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<RecipeData>> *>(4); }
};

struct RecipesDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recipes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RecipeData>>> recipes) { fbb_.AddOffset(4, recipes); }
  RecipesDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<RecipesData> Finish() { return flatbuffers::Offset<RecipesData>(fbb_.EndTable(start_, 1)); }
};

inline flatbuffers::Offset<RecipesData> CreateRecipesData(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<RecipeData>>> recipes) {
  RecipesDataBuilder builder_(_fbb);
  builder_.add_recipes(recipes);
  return builder_.Finish();
}

struct IngridientData : private flatbuffers::Table {
  uint32_t item_id() const { return GetField<uint32_t>(4, 0); }
  int32_t amount() const { return GetField<int32_t>(6, 0); }
};

struct IngridientDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_item_id(uint32_t item_id) { fbb_.AddElement<uint32_t>(4, item_id, 0); }
  void add_amount(int32_t amount) { fbb_.AddElement<int32_t>(6, amount, 0); }
  IngridientDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<IngridientData> Finish() { return flatbuffers::Offset<IngridientData>(fbb_.EndTable(start_, 2)); }
};

inline flatbuffers::Offset<IngridientData> CreateIngridientData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t item_id, int32_t amount) {
  IngridientDataBuilder builder_(_fbb);
  builder_.add_amount(amount);
  builder_.add_item_id(item_id);
  return builder_.Finish();
}

struct RecipeData : private flatbuffers::Table {
  int32_t id() const { return GetField<int32_t>(4, 0); }
  int32_t type() const { return GetField<int32_t>(6, 0); }
  int32_t level() const { return GetField<int32_t>(8, 0); }
  int32_t cost() const { return GetField<int32_t>(10, 0); }
  int32_t success_chance() const { return GetField<int32_t>(12, 0); }
  int32_t product_id() const { return GetField<int32_t>(14, 0); }
  int32_t product_amount() const { return GetField<int32_t>(16, 0); }
  const flatbuffers::Vector<flatbuffers::Offset<IngridientData>> *ingridients() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<IngridientData>> *>(18); }
};

struct RecipeDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(4, id, 0); }
  void add_type(int32_t type) { fbb_.AddElement<int32_t>(6, type, 0); }
  void add_level(int32_t level) { fbb_.AddElement<int32_t>(8, level, 0); }
  void add_cost(int32_t cost) { fbb_.AddElement<int32_t>(10, cost, 0); }
  void add_success_chance(int32_t success_chance) { fbb_.AddElement<int32_t>(12, success_chance, 0); }
  void add_product_id(int32_t product_id) { fbb_.AddElement<int32_t>(14, product_id, 0); }
  void add_product_amount(int32_t product_amount) { fbb_.AddElement<int32_t>(16, product_amount, 0); }
  void add_ingridients(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IngridientData>>> ingridients) { fbb_.AddOffset(18, ingridients); }
  RecipeDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<RecipeData> Finish() { return flatbuffers::Offset<RecipeData>(fbb_.EndTable(start_, 8)); }
};

inline flatbuffers::Offset<RecipeData> CreateRecipeData(flatbuffers::FlatBufferBuilder &_fbb, int32_t id, int32_t type, int32_t level, int32_t cost, int32_t success_chance, int32_t product_id, int32_t product_amount, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IngridientData>>> ingridients) {
  RecipeDataBuilder builder_(_fbb);
  builder_.add_ingridients(ingridients);
  builder_.add_product_amount(product_amount);
  builder_.add_product_id(product_id);
  builder_.add_success_chance(success_chance);
  builder_.add_cost(cost);
  builder_.add_level(level);
  builder_.add_type(type);
  builder_.add_id(id);
  return builder_.Finish();
}

inline const RecipesData *GetRecipesData(const void *buf) { return flatbuffers::GetRoot<RecipesData>(buf); }

}; // namespace butcher
#endif
