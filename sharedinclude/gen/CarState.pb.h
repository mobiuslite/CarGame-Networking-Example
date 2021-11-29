// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CarState.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_CarState_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_CarState_2eproto

#define PROTOBUF_USE_DLLS

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3018000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3018001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_CarState_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_CarState_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_CarState_2eproto;
namespace bufferProtos {
class CarState;
struct CarStateDefaultTypeInternal;
extern CarStateDefaultTypeInternal _CarState_default_instance_;
class CarState_Vector2;
struct CarState_Vector2DefaultTypeInternal;
extern CarState_Vector2DefaultTypeInternal _CarState_Vector2_default_instance_;
}  // namespace bufferProtos
PROTOBUF_NAMESPACE_OPEN
template<> ::bufferProtos::CarState* Arena::CreateMaybeMessage<::bufferProtos::CarState>(Arena*);
template<> ::bufferProtos::CarState_Vector2* Arena::CreateMaybeMessage<::bufferProtos::CarState_Vector2>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace bufferProtos {

// ===================================================================

class CarState_Vector2 final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bufferProtos.CarState.Vector2) */ {
 public:
  inline CarState_Vector2() : CarState_Vector2(nullptr) {}
  ~CarState_Vector2() override;
  explicit constexpr CarState_Vector2(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  CarState_Vector2(const CarState_Vector2& from);
  CarState_Vector2(CarState_Vector2&& from) noexcept
    : CarState_Vector2() {
    *this = ::std::move(from);
  }

  inline CarState_Vector2& operator=(const CarState_Vector2& from) {
    CopyFrom(from);
    return *this;
  }
  inline CarState_Vector2& operator=(CarState_Vector2&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const CarState_Vector2& default_instance() {
    return *internal_default_instance();
  }
  static inline const CarState_Vector2* internal_default_instance() {
    return reinterpret_cast<const CarState_Vector2*>(
               &_CarState_Vector2_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(CarState_Vector2& a, CarState_Vector2& b) {
    a.Swap(&b);
  }
  inline void Swap(CarState_Vector2* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(CarState_Vector2* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline CarState_Vector2* New() const final {
    return new CarState_Vector2();
  }

  CarState_Vector2* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<CarState_Vector2>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const CarState_Vector2& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const CarState_Vector2& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(CarState_Vector2* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bufferProtos.CarState.Vector2";
  }
  protected:
  explicit CarState_Vector2(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kXFieldNumber = 1,
    kZFieldNumber = 2,
  };
  // required float x = 1;
  bool has_x() const;
  private:
  bool _internal_has_x() const;
  public:
  void clear_x();
  float x() const;
  void set_x(float value);
  private:
  float _internal_x() const;
  void _internal_set_x(float value);
  public:

  // required float z = 2;
  bool has_z() const;
  private:
  bool _internal_has_z() const;
  public:
  void clear_z();
  float z() const;
  void set_z(float value);
  private:
  float _internal_z() const;
  void _internal_set_z(float value);
  public:

  // @@protoc_insertion_point(class_scope:bufferProtos.CarState.Vector2)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  float x_;
  float z_;
  friend struct ::TableStruct_CarState_2eproto;
};
// -------------------------------------------------------------------

class CarState final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bufferProtos.CarState) */ {
 public:
  inline CarState() : CarState(nullptr) {}
  ~CarState() override;
  explicit constexpr CarState(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  CarState(const CarState& from);
  CarState(CarState&& from) noexcept
    : CarState() {
    *this = ::std::move(from);
  }

  inline CarState& operator=(const CarState& from) {
    CopyFrom(from);
    return *this;
  }
  inline CarState& operator=(CarState&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const CarState& default_instance() {
    return *internal_default_instance();
  }
  static inline const CarState* internal_default_instance() {
    return reinterpret_cast<const CarState*>(
               &_CarState_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(CarState& a, CarState& b) {
    a.Swap(&b);
  }
  inline void Swap(CarState* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(CarState* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline CarState* New() const final {
    return new CarState();
  }

  CarState* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<CarState>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const CarState& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const CarState& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(CarState* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bufferProtos.CarState";
  }
  protected:
  explicit CarState(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef CarState_Vector2 Vector2;

  // accessors -------------------------------------------------------

  enum : int {
    kUsernameFieldNumber = 1,
    kPositionFieldNumber = 2,
    kVelocityFieldNumber = 3,
    kYRadiansRotationFieldNumber = 4,
  };
  // required string username = 1;
  bool has_username() const;
  private:
  bool _internal_has_username() const;
  public:
  void clear_username();
  const std::string& username() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_username(ArgT0&& arg0, ArgT... args);
  std::string* mutable_username();
  PROTOBUF_MUST_USE_RESULT std::string* release_username();
  void set_allocated_username(std::string* username);
  private:
  const std::string& _internal_username() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_username(const std::string& value);
  std::string* _internal_mutable_username();
  public:

  // required .bufferProtos.CarState.Vector2 position = 2;
  bool has_position() const;
  private:
  bool _internal_has_position() const;
  public:
  void clear_position();
  const ::bufferProtos::CarState_Vector2& position() const;
  PROTOBUF_MUST_USE_RESULT ::bufferProtos::CarState_Vector2* release_position();
  ::bufferProtos::CarState_Vector2* mutable_position();
  void set_allocated_position(::bufferProtos::CarState_Vector2* position);
  private:
  const ::bufferProtos::CarState_Vector2& _internal_position() const;
  ::bufferProtos::CarState_Vector2* _internal_mutable_position();
  public:
  void unsafe_arena_set_allocated_position(
      ::bufferProtos::CarState_Vector2* position);
  ::bufferProtos::CarState_Vector2* unsafe_arena_release_position();

  // required .bufferProtos.CarState.Vector2 velocity = 3;
  bool has_velocity() const;
  private:
  bool _internal_has_velocity() const;
  public:
  void clear_velocity();
  const ::bufferProtos::CarState_Vector2& velocity() const;
  PROTOBUF_MUST_USE_RESULT ::bufferProtos::CarState_Vector2* release_velocity();
  ::bufferProtos::CarState_Vector2* mutable_velocity();
  void set_allocated_velocity(::bufferProtos::CarState_Vector2* velocity);
  private:
  const ::bufferProtos::CarState_Vector2& _internal_velocity() const;
  ::bufferProtos::CarState_Vector2* _internal_mutable_velocity();
  public:
  void unsafe_arena_set_allocated_velocity(
      ::bufferProtos::CarState_Vector2* velocity);
  ::bufferProtos::CarState_Vector2* unsafe_arena_release_velocity();

  // required float yRadiansRotation = 4;
  bool has_yradiansrotation() const;
  private:
  bool _internal_has_yradiansrotation() const;
  public:
  void clear_yradiansrotation();
  float yradiansrotation() const;
  void set_yradiansrotation(float value);
  private:
  float _internal_yradiansrotation() const;
  void _internal_set_yradiansrotation(float value);
  public:

  // @@protoc_insertion_point(class_scope:bufferProtos.CarState)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr username_;
  ::bufferProtos::CarState_Vector2* position_;
  ::bufferProtos::CarState_Vector2* velocity_;
  float yradiansrotation_;
  friend struct ::TableStruct_CarState_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// CarState_Vector2

// required float x = 1;
inline bool CarState_Vector2::_internal_has_x() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool CarState_Vector2::has_x() const {
  return _internal_has_x();
}
inline void CarState_Vector2::clear_x() {
  x_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline float CarState_Vector2::_internal_x() const {
  return x_;
}
inline float CarState_Vector2::x() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.Vector2.x)
  return _internal_x();
}
inline void CarState_Vector2::_internal_set_x(float value) {
  _has_bits_[0] |= 0x00000001u;
  x_ = value;
}
inline void CarState_Vector2::set_x(float value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:bufferProtos.CarState.Vector2.x)
}

// required float z = 2;
inline bool CarState_Vector2::_internal_has_z() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool CarState_Vector2::has_z() const {
  return _internal_has_z();
}
inline void CarState_Vector2::clear_z() {
  z_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline float CarState_Vector2::_internal_z() const {
  return z_;
}
inline float CarState_Vector2::z() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.Vector2.z)
  return _internal_z();
}
inline void CarState_Vector2::_internal_set_z(float value) {
  _has_bits_[0] |= 0x00000002u;
  z_ = value;
}
inline void CarState_Vector2::set_z(float value) {
  _internal_set_z(value);
  // @@protoc_insertion_point(field_set:bufferProtos.CarState.Vector2.z)
}

// -------------------------------------------------------------------

// CarState

// required string username = 1;
inline bool CarState::_internal_has_username() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool CarState::has_username() const {
  return _internal_has_username();
}
inline void CarState::clear_username() {
  username_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& CarState::username() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.username)
  return _internal_username();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void CarState::set_username(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000001u;
 username_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:bufferProtos.CarState.username)
}
inline std::string* CarState::mutable_username() {
  std::string* _s = _internal_mutable_username();
  // @@protoc_insertion_point(field_mutable:bufferProtos.CarState.username)
  return _s;
}
inline const std::string& CarState::_internal_username() const {
  return username_.Get();
}
inline void CarState::_internal_set_username(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  username_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* CarState::_internal_mutable_username() {
  _has_bits_[0] |= 0x00000001u;
  return username_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* CarState::release_username() {
  // @@protoc_insertion_point(field_release:bufferProtos.CarState.username)
  if (!_internal_has_username()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return username_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void CarState::set_allocated_username(std::string* username) {
  if (username != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  username_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), username,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:bufferProtos.CarState.username)
}

// required .bufferProtos.CarState.Vector2 position = 2;
inline bool CarState::_internal_has_position() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  PROTOBUF_ASSUME(!value || position_ != nullptr);
  return value;
}
inline bool CarState::has_position() const {
  return _internal_has_position();
}
inline void CarState::clear_position() {
  if (position_ != nullptr) position_->Clear();
  _has_bits_[0] &= ~0x00000002u;
}
inline const ::bufferProtos::CarState_Vector2& CarState::_internal_position() const {
  const ::bufferProtos::CarState_Vector2* p = position_;
  return p != nullptr ? *p : reinterpret_cast<const ::bufferProtos::CarState_Vector2&>(
      ::bufferProtos::_CarState_Vector2_default_instance_);
}
inline const ::bufferProtos::CarState_Vector2& CarState::position() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.position)
  return _internal_position();
}
inline void CarState::unsafe_arena_set_allocated_position(
    ::bufferProtos::CarState_Vector2* position) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(position_);
  }
  position_ = position;
  if (position) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:bufferProtos.CarState.position)
}
inline ::bufferProtos::CarState_Vector2* CarState::release_position() {
  _has_bits_[0] &= ~0x00000002u;
  ::bufferProtos::CarState_Vector2* temp = position_;
  position_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::bufferProtos::CarState_Vector2* CarState::unsafe_arena_release_position() {
  // @@protoc_insertion_point(field_release:bufferProtos.CarState.position)
  _has_bits_[0] &= ~0x00000002u;
  ::bufferProtos::CarState_Vector2* temp = position_;
  position_ = nullptr;
  return temp;
}
inline ::bufferProtos::CarState_Vector2* CarState::_internal_mutable_position() {
  _has_bits_[0] |= 0x00000002u;
  if (position_ == nullptr) {
    auto* p = CreateMaybeMessage<::bufferProtos::CarState_Vector2>(GetArenaForAllocation());
    position_ = p;
  }
  return position_;
}
inline ::bufferProtos::CarState_Vector2* CarState::mutable_position() {
  ::bufferProtos::CarState_Vector2* _msg = _internal_mutable_position();
  // @@protoc_insertion_point(field_mutable:bufferProtos.CarState.position)
  return _msg;
}
inline void CarState::set_allocated_position(::bufferProtos::CarState_Vector2* position) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete position_;
  }
  if (position) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper<::bufferProtos::CarState_Vector2>::GetOwningArena(position);
    if (message_arena != submessage_arena) {
      position = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, position, submessage_arena);
    }
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  position_ = position;
  // @@protoc_insertion_point(field_set_allocated:bufferProtos.CarState.position)
}

// required .bufferProtos.CarState.Vector2 velocity = 3;
inline bool CarState::_internal_has_velocity() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  PROTOBUF_ASSUME(!value || velocity_ != nullptr);
  return value;
}
inline bool CarState::has_velocity() const {
  return _internal_has_velocity();
}
inline void CarState::clear_velocity() {
  if (velocity_ != nullptr) velocity_->Clear();
  _has_bits_[0] &= ~0x00000004u;
}
inline const ::bufferProtos::CarState_Vector2& CarState::_internal_velocity() const {
  const ::bufferProtos::CarState_Vector2* p = velocity_;
  return p != nullptr ? *p : reinterpret_cast<const ::bufferProtos::CarState_Vector2&>(
      ::bufferProtos::_CarState_Vector2_default_instance_);
}
inline const ::bufferProtos::CarState_Vector2& CarState::velocity() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.velocity)
  return _internal_velocity();
}
inline void CarState::unsafe_arena_set_allocated_velocity(
    ::bufferProtos::CarState_Vector2* velocity) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(velocity_);
  }
  velocity_ = velocity;
  if (velocity) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:bufferProtos.CarState.velocity)
}
inline ::bufferProtos::CarState_Vector2* CarState::release_velocity() {
  _has_bits_[0] &= ~0x00000004u;
  ::bufferProtos::CarState_Vector2* temp = velocity_;
  velocity_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::bufferProtos::CarState_Vector2* CarState::unsafe_arena_release_velocity() {
  // @@protoc_insertion_point(field_release:bufferProtos.CarState.velocity)
  _has_bits_[0] &= ~0x00000004u;
  ::bufferProtos::CarState_Vector2* temp = velocity_;
  velocity_ = nullptr;
  return temp;
}
inline ::bufferProtos::CarState_Vector2* CarState::_internal_mutable_velocity() {
  _has_bits_[0] |= 0x00000004u;
  if (velocity_ == nullptr) {
    auto* p = CreateMaybeMessage<::bufferProtos::CarState_Vector2>(GetArenaForAllocation());
    velocity_ = p;
  }
  return velocity_;
}
inline ::bufferProtos::CarState_Vector2* CarState::mutable_velocity() {
  ::bufferProtos::CarState_Vector2* _msg = _internal_mutable_velocity();
  // @@protoc_insertion_point(field_mutable:bufferProtos.CarState.velocity)
  return _msg;
}
inline void CarState::set_allocated_velocity(::bufferProtos::CarState_Vector2* velocity) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete velocity_;
  }
  if (velocity) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper<::bufferProtos::CarState_Vector2>::GetOwningArena(velocity);
    if (message_arena != submessage_arena) {
      velocity = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, velocity, submessage_arena);
    }
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  velocity_ = velocity;
  // @@protoc_insertion_point(field_set_allocated:bufferProtos.CarState.velocity)
}

// required float yRadiansRotation = 4;
inline bool CarState::_internal_has_yradiansrotation() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool CarState::has_yradiansrotation() const {
  return _internal_has_yradiansrotation();
}
inline void CarState::clear_yradiansrotation() {
  yradiansrotation_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline float CarState::_internal_yradiansrotation() const {
  return yradiansrotation_;
}
inline float CarState::yradiansrotation() const {
  // @@protoc_insertion_point(field_get:bufferProtos.CarState.yRadiansRotation)
  return _internal_yradiansrotation();
}
inline void CarState::_internal_set_yradiansrotation(float value) {
  _has_bits_[0] |= 0x00000008u;
  yradiansrotation_ = value;
}
inline void CarState::set_yradiansrotation(float value) {
  _internal_set_yradiansrotation(value);
  // @@protoc_insertion_point(field_set:bufferProtos.CarState.yRadiansRotation)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace bufferProtos

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_CarState_2eproto
