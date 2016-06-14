#ifndef levels_generated_h
#define levels_generated_h
// automatically generated, do not modify

#include "flatbuffers/flatbuffers.h"

namespace butcher {

enum {
  GeneratorType_NaturalCave = 0,
  GeneratorType_DungeonMaker = 1,
};

inline const char **EnumNamesGeneratorType() {
  static const char *names[] = { "NaturalCave", "DungeonMaker", nullptr };
  return names;
}

inline const char *EnumNameGeneratorType(int e) { return EnumNamesGeneratorType()[e]; }

struct LevelsData;
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

struct LevelData : private flatbuffers::Table {
  uint32_t id() const { return GetField<uint32_t>(4, 0); }
  int8_t generator() const { return GetField<int8_t>(6, 0); }
  const flatbuffers::String *map_template() const { return GetPointer<const flatbuffers::String *>(8); }
};

struct LevelDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) { fbb_.AddElement<uint32_t>(4, id, 0); }
  void add_generator(int8_t generator) { fbb_.AddElement<int8_t>(6, generator, 0); }
  void add_map_template(flatbuffers::Offset<flatbuffers::String> map_template) { fbb_.AddOffset(8, map_template); }
  LevelDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<LevelData> Finish() { return flatbuffers::Offset<LevelData>(fbb_.EndTable(start_, 3)); }
};

inline flatbuffers::Offset<LevelData> CreateLevelData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, int8_t generator, flatbuffers::Offset<flatbuffers::String> map_template) {
  LevelDataBuilder builder_(_fbb);
  builder_.add_map_template(map_template);
  builder_.add_id(id);
  builder_.add_generator(generator);
  return builder_.Finish();
}

inline const LevelsData *GetLevelsData(const void *buf) { return flatbuffers::GetRoot<LevelsData>(buf); }

}; // namespace butcher

#endif
