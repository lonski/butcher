// automatically generated, do not modify
#ifndef levels_generated_h
#define levels_generated_h
#include "flatbuffers/flatbuffers.h"

namespace butcher {

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
  const flatbuffers::String *map_template() const { return GetPointer<const flatbuffers::String *>(6); }
  int32_t width() const { return GetField<int32_t>(8, 0); }
  int32_t height() const { return GetField<int32_t>(10, 0); }
  int32_t min_room_size() const { return GetField<int32_t>(12, 0); }
  int32_t max_room_size() const { return GetField<int32_t>(14, 0); }
  int32_t max_rooms() const { return GetField<int32_t>(16, 0); }
  int32_t rect_room_chance() const { return GetField<int32_t>(18, 0); }
  int32_t polygon_room_chance() const { return GetField<int32_t>(20, 0); }
  int32_t windy_chance() const { return GetField<int32_t>(22, 0); }
};

struct LevelDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) { fbb_.AddElement<uint32_t>(4, id, 0); }
  void add_map_template(flatbuffers::Offset<flatbuffers::String> map_template) { fbb_.AddOffset(6, map_template); }
  void add_width(int32_t width) { fbb_.AddElement<int32_t>(8, width, 0); }
  void add_height(int32_t height) { fbb_.AddElement<int32_t>(10, height, 0); }
  void add_min_room_size(int32_t min_room_size) { fbb_.AddElement<int32_t>(12, min_room_size, 0); }
  void add_max_room_size(int32_t max_room_size) { fbb_.AddElement<int32_t>(14, max_room_size, 0); }
  void add_max_rooms(int32_t max_rooms) { fbb_.AddElement<int32_t>(16, max_rooms, 0); }
  void add_rect_room_chance(int32_t rect_room_chance) { fbb_.AddElement<int32_t>(18, rect_room_chance, 0); }
  void add_polygon_room_chance(int32_t polygon_room_chance) { fbb_.AddElement<int32_t>(20, polygon_room_chance, 0); }
  void add_windy_chance(int32_t windy_chance) { fbb_.AddElement<int32_t>(22, windy_chance, 0); }
  LevelDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<LevelData> Finish() { return flatbuffers::Offset<LevelData>(fbb_.EndTable(start_, 10)); }
};

inline flatbuffers::Offset<LevelData> CreateLevelData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, flatbuffers::Offset<flatbuffers::String> map_template, int32_t width, int32_t height, int32_t min_room_size, int32_t max_room_size, int32_t max_rooms, int32_t rect_room_chance, int32_t polygon_room_chance, int32_t windy_chance) {
  LevelDataBuilder builder_(_fbb);
  builder_.add_windy_chance(windy_chance);
  builder_.add_polygon_room_chance(polygon_room_chance);
  builder_.add_rect_room_chance(rect_room_chance);
  builder_.add_max_rooms(max_rooms);
  builder_.add_max_room_size(max_room_size);
  builder_.add_min_room_size(min_room_size);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_map_template(map_template);
  builder_.add_id(id);
  return builder_.Finish();
}

inline const LevelsData *GetLevelsData(const void *buf) { return flatbuffers::GetRoot<LevelsData>(buf); }

}; // namespace butcher
#endif
