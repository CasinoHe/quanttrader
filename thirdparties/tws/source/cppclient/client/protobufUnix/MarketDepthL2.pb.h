// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: MarketDepthL2.proto
// Protobuf C++ Version: 5.29.1

#ifndef MarketDepthL2_2eproto_2epb_2eh
#define MarketDepthL2_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/runtime_version.h"
#if PROTOBUF_VERSION != 5029001
#error "Protobuf C++ gencode is built with an incompatible version of"
#error "Protobuf C++ headers/runtime. See"
#error "https://protobuf.dev/support/cross-version-runtime-guarantee/#cpp"
#endif
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/unknown_field_set.h"
#include "MarketDepthData.pb.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_MarketDepthL2_2eproto

namespace google {
namespace protobuf {
namespace internal {
template <typename T>
::absl::string_view GetAnyMessageName();
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_MarketDepthL2_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_MarketDepthL2_2eproto;
namespace protobuf {
class MarketDepthL2;
struct MarketDepthL2DefaultTypeInternal;
extern MarketDepthL2DefaultTypeInternal _MarketDepthL2_default_instance_;
}  // namespace protobuf
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace protobuf {

// ===================================================================


// -------------------------------------------------------------------

class MarketDepthL2 final
    : public ::google::protobuf::Message
/* @@protoc_insertion_point(class_definition:protobuf.MarketDepthL2) */ {
 public:
  inline MarketDepthL2() : MarketDepthL2(nullptr) {}
  ~MarketDepthL2() PROTOBUF_FINAL;

#if defined(PROTOBUF_CUSTOM_VTABLE)
  void operator delete(MarketDepthL2* msg, std::destroying_delete_t) {
    SharedDtor(*msg);
    ::google::protobuf::internal::SizedDelete(msg, sizeof(MarketDepthL2));
  }
#endif

  template <typename = void>
  explicit PROTOBUF_CONSTEXPR MarketDepthL2(
      ::google::protobuf::internal::ConstantInitialized);

  inline MarketDepthL2(const MarketDepthL2& from) : MarketDepthL2(nullptr, from) {}
  inline MarketDepthL2(MarketDepthL2&& from) noexcept
      : MarketDepthL2(nullptr, std::move(from)) {}
  inline MarketDepthL2& operator=(const MarketDepthL2& from) {
    CopyFrom(from);
    return *this;
  }
  inline MarketDepthL2& operator=(MarketDepthL2&& from) noexcept {
    if (this == &from) return *this;
    if (::google::protobuf::internal::CanMoveWithInternalSwap(GetArena(), from.GetArena())) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const MarketDepthL2& default_instance() {
    return *internal_default_instance();
  }
  static inline const MarketDepthL2* internal_default_instance() {
    return reinterpret_cast<const MarketDepthL2*>(
        &_MarketDepthL2_default_instance_);
  }
  static constexpr int kIndexInFileMessages = 0;
  friend void swap(MarketDepthL2& a, MarketDepthL2& b) { a.Swap(&b); }
  inline void Swap(MarketDepthL2* other) {
    if (other == this) return;
    if (::google::protobuf::internal::CanUseInternalSwap(GetArena(), other->GetArena())) {
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(MarketDepthL2* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  MarketDepthL2* New(::google::protobuf::Arena* arena = nullptr) const {
    return ::google::protobuf::Message::DefaultConstruct<MarketDepthL2>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const MarketDepthL2& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom(const MarketDepthL2& from) { MarketDepthL2::MergeImpl(*this, from); }

  private:
  static void MergeImpl(
      ::google::protobuf::MessageLite& to_msg,
      const ::google::protobuf::MessageLite& from_msg);

  public:
  bool IsInitialized() const {
    return true;
  }
  ABSL_ATTRIBUTE_REINITIALIZES void Clear() PROTOBUF_FINAL;
  #if defined(PROTOBUF_CUSTOM_VTABLE)
  private:
  static ::size_t ByteSizeLong(const ::google::protobuf::MessageLite& msg);
  static ::uint8_t* _InternalSerialize(
      const MessageLite& msg, ::uint8_t* target,
      ::google::protobuf::io::EpsCopyOutputStream* stream);

  public:
  ::size_t ByteSizeLong() const { return ByteSizeLong(*this); }
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target,
      ::google::protobuf::io::EpsCopyOutputStream* stream) const {
    return _InternalSerialize(*this, target, stream);
  }
  #else   // PROTOBUF_CUSTOM_VTABLE
  ::size_t ByteSizeLong() const final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target,
      ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  #endif  // PROTOBUF_CUSTOM_VTABLE
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::google::protobuf::Arena* arena);
  static void SharedDtor(MessageLite& self);
  void InternalSwap(MarketDepthL2* other);
 private:
  template <typename T>
  friend ::absl::string_view(
      ::google::protobuf::internal::GetAnyMessageName)();
  static ::absl::string_view FullMessageName() { return "protobuf.MarketDepthL2"; }

 protected:
  explicit MarketDepthL2(::google::protobuf::Arena* arena);
  MarketDepthL2(::google::protobuf::Arena* arena, const MarketDepthL2& from);
  MarketDepthL2(::google::protobuf::Arena* arena, MarketDepthL2&& from) noexcept
      : MarketDepthL2(arena) {
    *this = ::std::move(from);
  }
  const ::google::protobuf::internal::ClassData* GetClassData() const PROTOBUF_FINAL;
  static void* PlacementNew_(const void*, void* mem,
                             ::google::protobuf::Arena* arena);
  static constexpr auto InternalNewImpl_();
  static const ::google::protobuf::internal::ClassDataFull _class_data_;

 public:
  ::google::protobuf::Metadata GetMetadata() const;
  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------
  enum : int {
    kMarketDepthDataFieldNumber = 2,
    kReqIdFieldNumber = 1,
  };
  // optional .protobuf.MarketDepthData marketDepthData = 2;
  bool has_marketdepthdata() const;
  void clear_marketdepthdata() ;
  const ::protobuf::MarketDepthData& marketdepthdata() const;
  PROTOBUF_NODISCARD ::protobuf::MarketDepthData* release_marketdepthdata();
  ::protobuf::MarketDepthData* mutable_marketdepthdata();
  void set_allocated_marketdepthdata(::protobuf::MarketDepthData* value);
  void unsafe_arena_set_allocated_marketdepthdata(::protobuf::MarketDepthData* value);
  ::protobuf::MarketDepthData* unsafe_arena_release_marketdepthdata();

  private:
  const ::protobuf::MarketDepthData& _internal_marketdepthdata() const;
  ::protobuf::MarketDepthData* _internal_mutable_marketdepthdata();

  public:
  // optional int32 reqId = 1;
  bool has_reqid() const;
  void clear_reqid() ;
  ::int32_t reqid() const;
  void set_reqid(::int32_t value);

  private:
  ::int32_t _internal_reqid() const;
  void _internal_set_reqid(::int32_t value);

  public:
  // @@protoc_insertion_point(class_scope:protobuf.MarketDepthL2)
 private:
  class _Internal;
  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      1, 2, 1,
      0, 2>
      _table_;

  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {
    inline explicit constexpr Impl_(
        ::google::protobuf::internal::ConstantInitialized) noexcept;
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena);
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena, const Impl_& from,
                          const MarketDepthL2& from_msg);
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    ::google::protobuf::internal::CachedSize _cached_size_;
    ::protobuf::MarketDepthData* marketdepthdata_;
    ::int32_t reqid_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_MarketDepthL2_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// MarketDepthL2

// optional int32 reqId = 1;
inline bool MarketDepthL2::has_reqid() const {
  bool value = (_impl_._has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline void MarketDepthL2::clear_reqid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.reqid_ = 0;
  _impl_._has_bits_[0] &= ~0x00000002u;
}
inline ::int32_t MarketDepthL2::reqid() const {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthL2.reqId)
  return _internal_reqid();
}
inline void MarketDepthL2::set_reqid(::int32_t value) {
  _internal_set_reqid(value);
  _impl_._has_bits_[0] |= 0x00000002u;
  // @@protoc_insertion_point(field_set:protobuf.MarketDepthL2.reqId)
}
inline ::int32_t MarketDepthL2::_internal_reqid() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.reqid_;
}
inline void MarketDepthL2::_internal_set_reqid(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.reqid_ = value;
}

// optional .protobuf.MarketDepthData marketDepthData = 2;
inline bool MarketDepthL2::has_marketdepthdata() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || _impl_.marketdepthdata_ != nullptr);
  return value;
}
inline const ::protobuf::MarketDepthData& MarketDepthL2::_internal_marketdepthdata() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  const ::protobuf::MarketDepthData* p = _impl_.marketdepthdata_;
  return p != nullptr ? *p : reinterpret_cast<const ::protobuf::MarketDepthData&>(::protobuf::_MarketDepthData_default_instance_);
}
inline const ::protobuf::MarketDepthData& MarketDepthL2::marketdepthdata() const ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthL2.marketDepthData)
  return _internal_marketdepthdata();
}
inline void MarketDepthL2::unsafe_arena_set_allocated_marketdepthdata(::protobuf::MarketDepthData* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.marketdepthdata_);
  }
  _impl_.marketdepthdata_ = reinterpret_cast<::protobuf::MarketDepthData*>(value);
  if (value != nullptr) {
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:protobuf.MarketDepthL2.marketDepthData)
}
inline ::protobuf::MarketDepthData* MarketDepthL2::release_marketdepthdata() {
  ::google::protobuf::internal::TSanWrite(&_impl_);

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::protobuf::MarketDepthData* released = _impl_.marketdepthdata_;
  _impl_.marketdepthdata_ = nullptr;
  if (::google::protobuf::internal::DebugHardenForceCopyInRelease()) {
    auto* old = reinterpret_cast<::google::protobuf::MessageLite*>(released);
    released = ::google::protobuf::internal::DuplicateIfNonNull(released);
    if (GetArena() == nullptr) {
      delete old;
    }
  } else {
    if (GetArena() != nullptr) {
      released = ::google::protobuf::internal::DuplicateIfNonNull(released);
    }
  }
  return released;
}
inline ::protobuf::MarketDepthData* MarketDepthL2::unsafe_arena_release_marketdepthdata() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:protobuf.MarketDepthL2.marketDepthData)

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::protobuf::MarketDepthData* temp = _impl_.marketdepthdata_;
  _impl_.marketdepthdata_ = nullptr;
  return temp;
}
inline ::protobuf::MarketDepthData* MarketDepthL2::_internal_mutable_marketdepthdata() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.marketdepthdata_ == nullptr) {
    auto* p = ::google::protobuf::Message::DefaultConstruct<::protobuf::MarketDepthData>(GetArena());
    _impl_.marketdepthdata_ = reinterpret_cast<::protobuf::MarketDepthData*>(p);
  }
  return _impl_.marketdepthdata_;
}
inline ::protobuf::MarketDepthData* MarketDepthL2::mutable_marketdepthdata() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  _impl_._has_bits_[0] |= 0x00000001u;
  ::protobuf::MarketDepthData* _msg = _internal_mutable_marketdepthdata();
  // @@protoc_insertion_point(field_mutable:protobuf.MarketDepthL2.marketDepthData)
  return _msg;
}
inline void MarketDepthL2::set_allocated_marketdepthdata(::protobuf::MarketDepthData* value) {
  ::google::protobuf::Arena* message_arena = GetArena();
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (message_arena == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.marketdepthdata_);
  }

  if (value != nullptr) {
    ::google::protobuf::Arena* submessage_arena = reinterpret_cast<::google::protobuf::MessageLite*>(value)->GetArena();
    if (message_arena != submessage_arena) {
      value = ::google::protobuf::internal::GetOwnedMessage(message_arena, value, submessage_arena);
    }
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }

  _impl_.marketdepthdata_ = reinterpret_cast<::protobuf::MarketDepthData*>(value);
  // @@protoc_insertion_point(field_set_allocated:protobuf.MarketDepthL2.marketDepthData)
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // MarketDepthL2_2eproto_2epb_2eh
