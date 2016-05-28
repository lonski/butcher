// automatically generated, do not modify

#include "flatbuffers/flatbuffers.h"

namespace butcher {

enum {
  ActorType_Monster = 0,
  ActorType_Player = 1,
  ActorType_Item = 2,
};

inline const char **EnumNamesActorType() {
  static const char *names[] = { "Monster", "Player", "Item", nullptr };
  return names;
}

inline const char *EnumNameActorType(int e) { return EnumNamesActorType()[e]; }

struct ActorsData;
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

struct ActorData : private flatbuffers::Table {
  uint32_t id() const { return GetField<uint32_t>(4, 0); }
  int8_t type() const { return GetField<int8_t>(6, 0); }
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(8); }
  uint8_t blocks() const { return GetField<uint8_t>(10, 0); }
  uint8_t transparent() const { return GetField<uint8_t>(12, 0); }
  const flatbuffers::String *sprite_file() const { return GetPointer<const flatbuffers::String *>(14); }
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
  ActorDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<ActorData> Finish() { return flatbuffers::Offset<ActorData>(fbb_.EndTable(start_, 6)); }
};

inline flatbuffers::Offset<ActorData> CreateActorData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, int8_t type, flatbuffers::Offset<flatbuffers::String> name, uint8_t blocks, uint8_t transparent, flatbuffers::Offset<flatbuffers::String> sprite_file) {
  ActorDataBuilder builder_(_fbb);
  builder_.add_sprite_file(sprite_file);
  builder_.add_name(name);
  builder_.add_id(id);
  builder_.add_transparent(transparent);
  builder_.add_blocks(blocks);
  builder_.add_type(type);
  return builder_.Finish();
}

inline const ActorsData *GetActorsData(const void *buf) { return flatbuffers::GetRoot<ActorsData>(buf); }

}; // namespace butcher
