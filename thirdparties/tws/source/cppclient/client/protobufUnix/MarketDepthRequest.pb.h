// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: MarketDepthRequest.proto
// Protobuf C++ Version: 5.29.1

#ifndef MarketDepthRequest_2eproto_2epb_2eh
#define MarketDepthRequest_2eproto_2epb_2eh

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
#include "google/protobuf/map.h"  // IWYU pragma: export
#include "google/protobuf/map_entry.h"
#include "google/protobuf/map_field_inl.h"
#include "google/protobuf/unknown_field_set.h"
#include "Contract.pb.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_MarketDepthRequest_2eproto

namespace google {
namespace protobuf {
namespace internal {
template <typename T>
::absl::string_view GetAnyMessageName();
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_MarketDepthRequest_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_MarketDepthRequest_2eproto;
namespace protobuf {
class MarketDepthRequest;
struct MarketDepthRequestDefaultTypeInternal;
extern MarketDepthRequestDefaultTypeInternal _MarketDepthRequest_default_instance_;
class MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse;
struct MarketDepthRequest_MarketDepthOptionsEntry_DoNotUseDefaultTypeInternal;
extern MarketDepthRequest_MarketDepthOptionsEntry_DoNotUseDefaultTypeInternal _MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse_default_instance_;
}  // namespace protobuf
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace protobuf {

// ===================================================================


// -------------------------------------------------------------------

class MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse final
    : public ::google::protobuf::internal::MapEntry<
          std::string, std::string,
          ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
          ::google::protobuf::internal::WireFormatLite::TYPE_STRING> {
 public:
  using SuperType = ::google::protobuf::internal::MapEntry<
      std::string, std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING>;
  MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse();
  template <typename = void>
  explicit PROTOBUF_CONSTEXPR MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse(
      ::google::protobuf::internal::ConstantInitialized);
  explicit MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse(::google::protobuf::Arena* arena);
  static const MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse* internal_default_instance() {
    return reinterpret_cast<const MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse*>(
        &_MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse_default_instance_);
  }


 private:
  friend class ::google::protobuf::MessageLite;
  friend struct ::TableStruct_MarketDepthRequest_2eproto;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      1, 2, 0,
      68, 2>
      _table_;

  const ::google::protobuf::internal::ClassData* GetClassData() const PROTOBUF_FINAL;
  static void* PlacementNew_(const void*, void* mem,
                             ::google::protobuf::Arena* arena);
  static constexpr auto InternalNewImpl_();
  static const ::google::protobuf::internal::ClassDataFull _class_data_;
};
// -------------------------------------------------------------------

class MarketDepthRequest final
    : public ::google::protobuf::Message
/* @@protoc_insertion_point(class_definition:protobuf.MarketDepthRequest) */ {
 public:
  inline MarketDepthRequest() : MarketDepthRequest(nullptr) {}
  ~MarketDepthRequest() PROTOBUF_FINAL;

#if defined(PROTOBUF_CUSTOM_VTABLE)
  void operator delete(MarketDepthRequest* msg, std::destroying_delete_t) {
    SharedDtor(*msg);
    ::google::protobuf::internal::SizedDelete(msg, sizeof(MarketDepthRequest));
  }
#endif

  template <typename = void>
  explicit PROTOBUF_CONSTEXPR MarketDepthRequest(
      ::google::protobuf::internal::ConstantInitialized);

  inline MarketDepthRequest(const MarketDepthRequest& from) : MarketDepthRequest(nullptr, from) {}
  inline MarketDepthRequest(MarketDepthRequest&& from) noexcept
      : MarketDepthRequest(nullptr, std::move(from)) {}
  inline MarketDepthRequest& operator=(const MarketDepthRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline MarketDepthRequest& operator=(MarketDepthRequest&& from) noexcept {
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
  static const MarketDepthRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const MarketDepthRequest* internal_default_instance() {
    return reinterpret_cast<const MarketDepthRequest*>(
        &_MarketDepthRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages = 1;
  friend void swap(MarketDepthRequest& a, MarketDepthRequest& b) { a.Swap(&b); }
  inline void Swap(MarketDepthRequest* other) {
    if (other == this) return;
    if (::google::protobuf::internal::CanUseInternalSwap(GetArena(), other->GetArena())) {
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(MarketDepthRequest* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  MarketDepthRequest* New(::google::protobuf::Arena* arena = nullptr) const {
    return ::google::protobuf::Message::DefaultConstruct<MarketDepthRequest>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const MarketDepthRequest& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom(const MarketDepthRequest& from) { MarketDepthRequest::MergeImpl(*this, from); }

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
  void InternalSwap(MarketDepthRequest* other);
 private:
  template <typename T>
  friend ::absl::string_view(
      ::google::protobuf::internal::GetAnyMessageName)();
  static ::absl::string_view FullMessageName() { return "protobuf.MarketDepthRequest"; }

 protected:
  explicit MarketDepthRequest(::google::protobuf::Arena* arena);
  MarketDepthRequest(::google::protobuf::Arena* arena, const MarketDepthRequest& from);
  MarketDepthRequest(::google::protobuf::Arena* arena, MarketDepthRequest&& from) noexcept
      : MarketDepthRequest(arena) {
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
    kMarketDepthOptionsFieldNumber = 5,
    kContractFieldNumber = 2,
    kReqIdFieldNumber = 1,
    kNumRowsFieldNumber = 3,
    kIsSmartDepthFieldNumber = 4,
  };
  // map<string, string> marketDepthOptions = 5;
  int marketdepthoptions_size() const;
  private:
  int _internal_marketdepthoptions_size() const;

  public:
  void clear_marketdepthoptions() ;
  const ::google::protobuf::Map<std::string, std::string>& marketdepthoptions() const;
  ::google::protobuf::Map<std::string, std::string>* mutable_marketdepthoptions();

  private:
  const ::google::protobuf::Map<std::string, std::string>& _internal_marketdepthoptions() const;
  ::google::protobuf::Map<std::string, std::string>* _internal_mutable_marketdepthoptions();

  public:
  // optional .protobuf.Contract contract = 2;
  bool has_contract() const;
  void clear_contract() ;
  const ::protobuf::Contract& contract() const;
  PROTOBUF_NODISCARD ::protobuf::Contract* release_contract();
  ::protobuf::Contract* mutable_contract();
  void set_allocated_contract(::protobuf::Contract* value);
  void unsafe_arena_set_allocated_contract(::protobuf::Contract* value);
  ::protobuf::Contract* unsafe_arena_release_contract();

  private:
  const ::protobuf::Contract& _internal_contract() const;
  ::protobuf::Contract* _internal_mutable_contract();

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
  // optional int32 numRows = 3;
  bool has_numrows() const;
  void clear_numrows() ;
  ::int32_t numrows() const;
  void set_numrows(::int32_t value);

  private:
  ::int32_t _internal_numrows() const;
  void _internal_set_numrows(::int32_t value);

  public:
  // optional bool isSmartDepth = 4;
  bool has_issmartdepth() const;
  void clear_issmartdepth() ;
  bool issmartdepth() const;
  void set_issmartdepth(bool value);

  private:
  bool _internal_issmartdepth() const;
  void _internal_set_issmartdepth(bool value);

  public:
  // @@protoc_insertion_point(class_scope:protobuf.MarketDepthRequest)
 private:
  class _Internal;
  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      2, 5, 2,
      54, 2>
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
                          const MarketDepthRequest& from_msg);
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    ::google::protobuf::internal::CachedSize _cached_size_;
    ::google::protobuf::internal::MapField<MarketDepthRequest_MarketDepthOptionsEntry_DoNotUse, std::string, std::string,
                      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
                      ::google::protobuf::internal::WireFormatLite::TYPE_STRING>
        marketdepthoptions_;
    ::protobuf::Contract* contract_;
    ::int32_t reqid_;
    ::int32_t numrows_;
    bool issmartdepth_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_MarketDepthRequest_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// MarketDepthRequest

// optional int32 reqId = 1;
inline bool MarketDepthRequest::has_reqid() const {
  bool value = (_impl_._has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline void MarketDepthRequest::clear_reqid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.reqid_ = 0;
  _impl_._has_bits_[0] &= ~0x00000002u;
}
inline ::int32_t MarketDepthRequest::reqid() const {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthRequest.reqId)
  return _internal_reqid();
}
inline void MarketDepthRequest::set_reqid(::int32_t value) {
  _internal_set_reqid(value);
  _impl_._has_bits_[0] |= 0x00000002u;
  // @@protoc_insertion_point(field_set:protobuf.MarketDepthRequest.reqId)
}
inline ::int32_t MarketDepthRequest::_internal_reqid() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.reqid_;
}
inline void MarketDepthRequest::_internal_set_reqid(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.reqid_ = value;
}

// optional .protobuf.Contract contract = 2;
inline bool MarketDepthRequest::has_contract() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || _impl_.contract_ != nullptr);
  return value;
}
inline const ::protobuf::Contract& MarketDepthRequest::_internal_contract() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  const ::protobuf::Contract* p = _impl_.contract_;
  return p != nullptr ? *p : reinterpret_cast<const ::protobuf::Contract&>(::protobuf::_Contract_default_instance_);
}
inline const ::protobuf::Contract& MarketDepthRequest::contract() const ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthRequest.contract)
  return _internal_contract();
}
inline void MarketDepthRequest::unsafe_arena_set_allocated_contract(::protobuf::Contract* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.contract_);
  }
  _impl_.contract_ = reinterpret_cast<::protobuf::Contract*>(value);
  if (value != nullptr) {
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:protobuf.MarketDepthRequest.contract)
}
inline ::protobuf::Contract* MarketDepthRequest::release_contract() {
  ::google::protobuf::internal::TSanWrite(&_impl_);

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::protobuf::Contract* released = _impl_.contract_;
  _impl_.contract_ = nullptr;
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
inline ::protobuf::Contract* MarketDepthRequest::unsafe_arena_release_contract() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:protobuf.MarketDepthRequest.contract)

  _impl_._has_bits_[0] &= ~0x00000001u;
  ::protobuf::Contract* temp = _impl_.contract_;
  _impl_.contract_ = nullptr;
  return temp;
}
inline ::protobuf::Contract* MarketDepthRequest::_internal_mutable_contract() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.contract_ == nullptr) {
    auto* p = ::google::protobuf::Message::DefaultConstruct<::protobuf::Contract>(GetArena());
    _impl_.contract_ = reinterpret_cast<::protobuf::Contract*>(p);
  }
  return _impl_.contract_;
}
inline ::protobuf::Contract* MarketDepthRequest::mutable_contract() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  _impl_._has_bits_[0] |= 0x00000001u;
  ::protobuf::Contract* _msg = _internal_mutable_contract();
  // @@protoc_insertion_point(field_mutable:protobuf.MarketDepthRequest.contract)
  return _msg;
}
inline void MarketDepthRequest::set_allocated_contract(::protobuf::Contract* value) {
  ::google::protobuf::Arena* message_arena = GetArena();
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (message_arena == nullptr) {
    delete reinterpret_cast<::google::protobuf::MessageLite*>(_impl_.contract_);
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

  _impl_.contract_ = reinterpret_cast<::protobuf::Contract*>(value);
  // @@protoc_insertion_point(field_set_allocated:protobuf.MarketDepthRequest.contract)
}

// optional int32 numRows = 3;
inline bool MarketDepthRequest::has_numrows() const {
  bool value = (_impl_._has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline void MarketDepthRequest::clear_numrows() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.numrows_ = 0;
  _impl_._has_bits_[0] &= ~0x00000004u;
}
inline ::int32_t MarketDepthRequest::numrows() const {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthRequest.numRows)
  return _internal_numrows();
}
inline void MarketDepthRequest::set_numrows(::int32_t value) {
  _internal_set_numrows(value);
  _impl_._has_bits_[0] |= 0x00000004u;
  // @@protoc_insertion_point(field_set:protobuf.MarketDepthRequest.numRows)
}
inline ::int32_t MarketDepthRequest::_internal_numrows() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.numrows_;
}
inline void MarketDepthRequest::_internal_set_numrows(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.numrows_ = value;
}

// optional bool isSmartDepth = 4;
inline bool MarketDepthRequest::has_issmartdepth() const {
  bool value = (_impl_._has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline void MarketDepthRequest::clear_issmartdepth() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.issmartdepth_ = false;
  _impl_._has_bits_[0] &= ~0x00000008u;
}
inline bool MarketDepthRequest::issmartdepth() const {
  // @@protoc_insertion_point(field_get:protobuf.MarketDepthRequest.isSmartDepth)
  return _internal_issmartdepth();
}
inline void MarketDepthRequest::set_issmartdepth(bool value) {
  _internal_set_issmartdepth(value);
  _impl_._has_bits_[0] |= 0x00000008u;
  // @@protoc_insertion_point(field_set:protobuf.MarketDepthRequest.isSmartDepth)
}
inline bool MarketDepthRequest::_internal_issmartdepth() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.issmartdepth_;
}
inline void MarketDepthRequest::_internal_set_issmartdepth(bool value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.issmartdepth_ = value;
}

// map<string, string> marketDepthOptions = 5;
inline int MarketDepthRequest::_internal_marketdepthoptions_size() const {
  return _internal_marketdepthoptions().size();
}
inline int MarketDepthRequest::marketdepthoptions_size() const {
  return _internal_marketdepthoptions_size();
}
inline void MarketDepthRequest::clear_marketdepthoptions() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.marketdepthoptions_.Clear();
}
inline const ::google::protobuf::Map<std::string, std::string>& MarketDepthRequest::_internal_marketdepthoptions() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.marketdepthoptions_.GetMap();
}
inline const ::google::protobuf::Map<std::string, std::string>& MarketDepthRequest::marketdepthoptions() const ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_map:protobuf.MarketDepthRequest.marketDepthOptions)
  return _internal_marketdepthoptions();
}
inline ::google::protobuf::Map<std::string, std::string>* MarketDepthRequest::_internal_mutable_marketdepthoptions() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  return _impl_.marketdepthoptions_.MutableMap();
}
inline ::google::protobuf::Map<std::string, std::string>* MarketDepthRequest::mutable_marketdepthoptions() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_mutable_map:protobuf.MarketDepthRequest.marketDepthOptions)
  return _internal_mutable_marketdepthoptions();
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // MarketDepthRequest_2eproto_2epb_2eh
