// automatically generated, do not modify

#ifndef FLATBUFFERS_GENERATED_EFFECTS_BUTCHER_H_
#define FLATBUFFERS_GENERATED_EFFECTS_BUTCHER_H_
#include "flatbuffers/flatbuffers.h"

namespace butcher {

struct EffectsData;
struct ModifierData;
struct EffectData;

struct EffectsData : private flatbuffers::Table {
  const flatbuffers::Vector<flatbuffers::Offset<EffectData>> *effects() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<EffectData>> *>(4); }
};

struct EffectsDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_effects(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EffectData>>> effects) { fbb_.AddOffset(4, effects); }
  EffectsDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<EffectsData> Finish() { return flatbuffers::Offset<EffectsData>(fbb_.EndTable(start_, 1)); }
};

inline flatbuffers::Offset<EffectsData> CreateEffectsData(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EffectData>>> effects) {
  EffectsDataBuilder builder_(_fbb);
  builder_.add_effects(effects);
  return builder_.Finish();
}

struct ModifierData : private flatbuffers::Table {
  uint32_t attr() const { return GetField<uint32_t>(4, 0); }
  int32_t value() const { return GetField<int32_t>(6, 0); }
  uint32_t special() const { return GetField<uint32_t>(8, 0); }
};

struct ModifierDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_attr(uint32_t attr) { fbb_.AddElement<uint32_t>(4, attr, 0); }
  void add_value(int32_t value) { fbb_.AddElement<int32_t>(6, value, 0); }
  void add_special(uint32_t special) { fbb_.AddElement<uint32_t>(8, special, 0); }
  ModifierDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<ModifierData> Finish() { return flatbuffers::Offset<ModifierData>(fbb_.EndTable(start_, 3)); }
};

inline flatbuffers::Offset<ModifierData> CreateModifierData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t attr, int32_t value, uint32_t special) {
  ModifierDataBuilder builder_(_fbb);
  builder_.add_special(special);
  builder_.add_value(value);
  builder_.add_attr(attr);
  return builder_.Finish();
}

struct EffectData : private flatbuffers::Table {
  uint32_t id() const { return GetField<uint32_t>(4, 0); }
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(6); }
  int32_t turns() const { return GetField<int32_t>(8, 0); }
  const flatbuffers::String *sprite_file() const { return GetPointer<const flatbuffers::String *>(10); }
  const flatbuffers::Vector<flatbuffers::Offset<ModifierData>> *modifiers() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ModifierData>> *>(12); }
};

struct EffectDataBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) { fbb_.AddElement<uint32_t>(4, id, 0); }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(6, name); }
  void add_turns(int32_t turns) { fbb_.AddElement<int32_t>(8, turns, 0); }
  void add_sprite_file(flatbuffers::Offset<flatbuffers::String> sprite_file) { fbb_.AddOffset(10, sprite_file); }
  void add_modifiers(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ModifierData>>> modifiers) { fbb_.AddOffset(12, modifiers); }
  EffectDataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  flatbuffers::Offset<EffectData> Finish() { return flatbuffers::Offset<EffectData>(fbb_.EndTable(start_, 5)); }
};

inline flatbuffers::Offset<EffectData> CreateEffectData(flatbuffers::FlatBufferBuilder &_fbb, uint32_t id, flatbuffers::Offset<flatbuffers::String> name, int32_t turns, flatbuffers::Offset<flatbuffers::String> sprite_file, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ModifierData>>> modifiers) {
  EffectDataBuilder builder_(_fbb);
  builder_.add_modifiers(modifiers);
  builder_.add_sprite_file(sprite_file);
  builder_.add_turns(turns);
  builder_.add_name(name);
  builder_.add_id(id);
  return builder_.Finish();
}

inline const EffectsData *GetEffectsData(const void *buf) { return flatbuffers::GetRoot<EffectsData>(buf); }

}; // namespace butcher
#endif
