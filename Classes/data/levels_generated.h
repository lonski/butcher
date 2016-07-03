// automatically generated, do not modify
#ifndef levels_generated_h
#define levels_generated_h
#include "flatbuffers/flatbuffers.h"

namespace butcher {

enum {
  GeneratorType_CelularAutomanataGenerator = 0,
  GeneratorType_MinimumCorridorGenerator = 1,
};

inline const char **EnumNamesGeneratorType() {
  static const char *names[] = { "CelularAutomanataGenerator", "MinimumCorridorGenerator", nullptr };
  return names;
}

inline const char *EnumNameGeneratorType(int e) { return EnumNamesGeneratorType()[e]; }

struct LevelsData;
struct CAGenerationParams;
struct LevelData;

struct LevelsData : private flatbuffers::Table {
  const flatbuffers::Vector<flatbuffers::Offset<LevelData>> *levels() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<LevelData>> *>(4); }
};

struct LevelsDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_levels(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<LevelData>>> levels) { fbb_.AddOffset(4, levels); }
  LevelsDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<LevelsData> Finish() { return flatbuffers::Offset<LevelsData>(fbb_.EndTable(start_, 1)); }
};

inline flatbuffers::Offset<LevelsData> CreateLevelsData(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<LevelData>>> levels) {
  LevelsDataBuilder builder_(_fbb);
  builder_.add_levels(levels);
  return builder_.Finish();
}

struct CAGenerationParams : private flatbuffers::Table {
  int32_t reps() const { return GetField<int32_t>(4, 0); }
  int32_t r1_cutoff() const { return GetField<int32_t>(6, 0); }
  int32_t r2_cutoff() const { return GetField<int32_t>(8, 0); }
};

struct CAGenerationParamsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_reps(int32_t reps) { fbb_.AddElement<int32_t>(4, reps, 0); }
  void add_r1_cutoff(int32_t r1_cutoff) { fbb_.AddElement<int32_t>(6, r1_cutoff, 0); }
  void add_r2_cutoff(int32_t r2_cutoff) { fbb_.AddElement<int32_t>(8, r2_cutoff, 0); }
  CAGenerationParamsBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<CAGenerationParams> Finish() { return flatbuffers::Offset<CAGenerationParams>(fbb_.EndTable(start_, 3)); }
};

inline flatbuffers::Offset<CAGenerationParams> CreateCAGenerationParams(flatbuffers::FlatBufferBuilder &_fbb, int32_t reps, int32_t r1_cutoff, int32_t r2_cutoff) {
  CAGenerationParamsBuilder builder_(_fbb);
  builder_.add_r2_cutoff(r2_cutoff);
  builder_.add_r1_cutoff(r1_cutoff);
  builder_.add_reps(reps);
  return builder_.Finish();
}

struct LevelData : private flatbuffers::Table {
  const flatbuffers::Vector<int32_t> *depth() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(4); }
  const flatbuffers::String *map_template() const { return GetPointer<const flatbuffers::String *>(6); }
  int8_t generator() const { return GetField<int8_t>(8, 0); }
  int32_t width() const { return GetField<int32_t>(10, 0); }
  int32_t height() const { return GetField<int32_t>(12, 0); }
  int32_t min_room_size() const { return GetField<int32_t>(14, 0); }
  int32_t max_room_size() const { return GetField<int32_t>(16, 0); }
  int32_t max_features() const { return GetField<int32_t>(18, 0); }
  int32_t min_corridor_length() const { return GetField<int32_t>(20, 0); }
  int32_t max_corridor_length() const { return GetField<int32_t>(22, 0); }
  int32_t room_chance() const { return GetField<int32_t>(24, 0); }
  int32_t wall_percent() const { return GetField<int32_t>(26, 0); }
  int32_t room_split_percent() const { return GetField<int32_t>(28, 0); }
  const flatbuffers::Vector<flatbuffers::Offset<CAGenerationParams>> *generation_params() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<CAGenerationParams>> *>(30); }
};

struct LevelDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_depth(flatbuffers::Offset<flatbuffers::Vector<int32_t>> depth) { fbb_.AddOffset(4, depth); }
  void add_map_template(flatbuffers::Offset<flatbuffers::String> map_template) { fbb_.AddOffset(6, map_template); }
  void add_generator(int8_t generator) { fbb_.AddElement<int8_t>(8, generator, 0); }
  void add_width(int32_t width) { fbb_.AddElement<int32_t>(10, width, 0); }
  void add_height(int32_t height) { fbb_.AddElement<int32_t>(12, height, 0); }
  void add_min_room_size(int32_t min_room_size) { fbb_.AddElement<int32_t>(14, min_room_size, 0); }
  void add_max_room_size(int32_t max_room_size) { fbb_.AddElement<int32_t>(16, max_room_size, 0); }
  void add_max_features(int32_t max_features) { fbb_.AddElement<int32_t>(18, max_features, 0); }
  void add_min_corridor_length(int32_t min_corridor_length) { fbb_.AddElement<int32_t>(20, min_corridor_length, 0); }
  void add_max_corridor_length(int32_t max_corridor_length) { fbb_.AddElement<int32_t>(22, max_corridor_length, 0); }
  void add_room_chance(int32_t room_chance) { fbb_.AddElement<int32_t>(24, room_chance, 0); }
  void add_wall_percent(int32_t wall_percent) { fbb_.AddElement<int32_t>(26, wall_percent, 0); }
  void add_room_split_percent(int32_t room_split_percent) { fbb_.AddElement<int32_t>(28, room_split_percent, 0); }
  void add_generation_params(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CAGenerationParams>>> generation_params) { fbb_.AddOffset(30, generation_params); }
  LevelDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<LevelData> Finish() { return flatbuffers::Offset<LevelData>(fbb_.EndTable(start_, 14)); }
};

inline flatbuffers::Offset<LevelData> CreateLevelData(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::Vector<int32_t>> depth, flatbuffers::Offset<flatbuffers::String> map_template, int8_t generator, int32_t width, int32_t height, int32_t min_room_size, int32_t max_room_size, int32_t max_features, int32_t min_corridor_length, int32_t max_corridor_length, int32_t room_chance, int32_t wall_percent, int32_t room_split_percent, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CAGenerationParams>>> generation_params) {
  LevelDataBuilder builder_(_fbb);
  builder_.add_generation_params(generation_params);
  builder_.add_room_split_percent(room_split_percent);
  builder_.add_wall_percent(wall_percent);
  builder_.add_room_chance(room_chance);
  builder_.add_max_corridor_length(max_corridor_length);
  builder_.add_min_corridor_length(min_corridor_length);
  builder_.add_max_features(max_features);
  builder_.add_max_room_size(max_room_size);
  builder_.add_min_room_size(min_room_size);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_map_template(map_template);
  builder_.add_depth(depth);
  builder_.add_generator(generator);
  return builder_.Finish();
}

inline const LevelsData *GetLevelsData(const void *buf) { return flatbuffers::GetRoot<LevelsData>(buf); }

}; // namespace butcher
#endif
