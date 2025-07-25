// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: MarketDataRequest.proto
// Protobuf C++ Version: 5.29.1

#include "MarketDataRequest.pb.h"

#include <algorithm>
#include <type_traits>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace protobuf {
              template <typename>
PROTOBUF_CONSTEXPR MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MarketDataRequest_MarketDataOptionsEntry_DoNotUse(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MapEntry(_class_data_.base()){}
#else   // PROTOBUF_CUSTOM_VTABLE
    : MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MapEntry() {
}
#endif  // PROTOBUF_CUSTOM_VTABLE
struct MarketDataRequest_MarketDataOptionsEntry_DoNotUseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MarketDataRequest_MarketDataOptionsEntry_DoNotUseDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MarketDataRequest_MarketDataOptionsEntry_DoNotUseDefaultTypeInternal() {}
  union {
    MarketDataRequest_MarketDataOptionsEntry_DoNotUse _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MarketDataRequest_MarketDataOptionsEntry_DoNotUseDefaultTypeInternal _MarketDataRequest_MarketDataOptionsEntry_DoNotUse_default_instance_;

inline constexpr MarketDataRequest::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        marketdataoptions_{},
        genericticklist_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        contract_{nullptr},
        reqid_{0},
        snapshot_{false},
        regulatorysnapshot_{false} {}

template <typename>
PROTOBUF_CONSTEXPR MarketDataRequest::MarketDataRequest(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct MarketDataRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MarketDataRequestDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MarketDataRequestDefaultTypeInternal() {}
  union {
    MarketDataRequest _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MarketDataRequestDefaultTypeInternal _MarketDataRequest_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_MarketDataRequest_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_MarketDataRequest_2eproto = nullptr;
const ::uint32_t
    TableStruct_MarketDataRequest_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.key_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.value_),
        0,
        1,
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.reqid_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.contract_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.genericticklist_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.snapshot_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.regulatorysnapshot_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataRequest, _impl_.marketdataoptions_),
        2,
        1,
        0,
        3,
        4,
        ~0u,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 10, -1, sizeof(::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse)},
        {12, 26, -1, sizeof(::protobuf::MarketDataRequest)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_MarketDataRequest_MarketDataOptionsEntry_DoNotUse_default_instance_._instance,
    &::protobuf::_MarketDataRequest_default_instance_._instance,
};
const char descriptor_table_protodef_MarketDataRequest_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\027MarketDataRequest.proto\022\010protobuf\032\016Con"
    "tract.proto\"\200\003\n\021MarketDataRequest\022\022\n\005req"
    "Id\030\001 \001(\005H\000\210\001\001\022)\n\010contract\030\002 \001(\0132\022.protob"
    "uf.ContractH\001\210\001\001\022\034\n\017genericTickList\030\003 \001("
    "\tH\002\210\001\001\022\025\n\010snapshot\030\004 \001(\010H\003\210\001\001\022\037\n\022regulat"
    "orySnapshot\030\005 \001(\010H\004\210\001\001\022M\n\021marketDataOpti"
    "ons\030\006 \003(\01322.protobuf.MarketDataRequest.M"
    "arketDataOptionsEntry\0328\n\026MarketDataOptio"
    "nsEntry\022\013\n\003key\030\001 \001(\t\022\r\n\005value\030\002 \001(\t:\0028\001B"
    "\010\n\006_reqIdB\013\n\t_contractB\022\n\020_genericTickLi"
    "stB\013\n\t_snapshotB\025\n\023_regulatorySnapshotBA"
    "\n\026com.ib.client.protobufB\026MarketDataRequ"
    "estProto\252\002\016IBApi.protobufb\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_MarketDataRequest_2eproto_deps[1] =
    {
        &::descriptor_table_Contract_2eproto,
};
static ::absl::once_flag descriptor_table_MarketDataRequest_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_MarketDataRequest_2eproto = {
    false,
    false,
    513,
    descriptor_table_protodef_MarketDataRequest_2eproto,
    "MarketDataRequest.proto",
    &descriptor_table_MarketDataRequest_2eproto_once,
    descriptor_table_MarketDataRequest_2eproto_deps,
    1,
    2,
    schemas,
    file_default_instances,
    TableStruct_MarketDataRequest_2eproto::offsets,
    file_level_enum_descriptors_MarketDataRequest_2eproto,
    file_level_service_descriptors_MarketDataRequest_2eproto,
};
namespace protobuf {
// ===================================================================

#if defined(PROTOBUF_CUSTOM_VTABLE)
              MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MarketDataRequest_MarketDataOptionsEntry_DoNotUse() : SuperType(_class_data_.base()) {}
              MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MarketDataRequest_MarketDataOptionsEntry_DoNotUse(::google::protobuf::Arena* arena)
                  : SuperType(arena, _class_data_.base()) {}
#else   // PROTOBUF_CUSTOM_VTABLE
              MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MarketDataRequest_MarketDataOptionsEntry_DoNotUse() : SuperType() {}
              MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MarketDataRequest_MarketDataOptionsEntry_DoNotUse(::google::protobuf::Arena* arena) : SuperType(arena) {}
#endif  // PROTOBUF_CUSTOM_VTABLE
              inline void* MarketDataRequest_MarketDataOptionsEntry_DoNotUse::PlacementNew_(const void*, void* mem,
                                                      ::google::protobuf::Arena* arena) {
                return ::new (mem) MarketDataRequest_MarketDataOptionsEntry_DoNotUse(arena);
              }
              constexpr auto MarketDataRequest_MarketDataOptionsEntry_DoNotUse::InternalNewImpl_() {
                return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(MarketDataRequest_MarketDataOptionsEntry_DoNotUse),
                                                          alignof(MarketDataRequest_MarketDataOptionsEntry_DoNotUse));
              }
              PROTOBUF_CONSTINIT
              PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
              const ::google::protobuf::internal::ClassDataFull MarketDataRequest_MarketDataOptionsEntry_DoNotUse::_class_data_ = {
                  ::google::protobuf::internal::ClassData{
                      &_MarketDataRequest_MarketDataOptionsEntry_DoNotUse_default_instance_._instance,
                      &_table_.header,
                      nullptr,  // OnDemandRegisterArenaDtor
                      nullptr,  // IsInitialized
                      &MarketDataRequest_MarketDataOptionsEntry_DoNotUse::MergeImpl,
                      ::google::protobuf::Message::GetNewImpl<MarketDataRequest_MarketDataOptionsEntry_DoNotUse>(),
              #if defined(PROTOBUF_CUSTOM_VTABLE)
                      &MarketDataRequest_MarketDataOptionsEntry_DoNotUse::SharedDtor,
                      static_cast<void (::google::protobuf::MessageLite::*)()>(
                          &MarketDataRequest_MarketDataOptionsEntry_DoNotUse::ClearImpl),
                          ::google::protobuf::Message::ByteSizeLongImpl, ::google::protobuf::Message::_InternalSerializeImpl
                          ,
              #endif  // PROTOBUF_CUSTOM_VTABLE
                      PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_._cached_size_),
                      false,
                  },
                  &MarketDataRequest_MarketDataOptionsEntry_DoNotUse::kDescriptorMethods,
                  &descriptor_table_MarketDataRequest_2eproto,
                  nullptr,  // tracker
              };
              const ::google::protobuf::internal::ClassData* MarketDataRequest_MarketDataOptionsEntry_DoNotUse::GetClassData() const {
                ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
                ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
                return _class_data_.base();
              }
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<1, 2, 0, 66, 2> MarketDataRequest_MarketDataOptionsEntry_DoNotUse::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_._has_bits_),
    0, // no _extensions_
    2, 8,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967292,  // skipmap
    offsetof(decltype(_table_), field_entries),
    2,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::DiscardEverythingFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::MarketDataRequest_MarketDataOptionsEntry_DoNotUse>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string value = 2;
    {::_pbi::TcParser::FastUS1,
     {18, 63, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.value_)}},
    // string key = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.key_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string key = 1;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.key_), -1, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string value = 2;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest_MarketDataOptionsEntry_DoNotUse, _impl_.value_), -1, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\61\3\5\0\0\0\0\0"
    "protobuf.MarketDataRequest.MarketDataOptionsEntry"
    "key"
    "value"
  }},
};

// ===================================================================

class MarketDataRequest::_Internal {
 public:
  using HasBits =
      decltype(std::declval<MarketDataRequest>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_._has_bits_);
};

void MarketDataRequest::clear_contract() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.contract_ != nullptr) _impl_.contract_->Clear();
  _impl_._has_bits_[0] &= ~0x00000002u;
}
MarketDataRequest::MarketDataRequest(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.MarketDataRequest)
}
inline PROTOBUF_NDEBUG_INLINE MarketDataRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::protobuf::MarketDataRequest& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        marketdataoptions_{visibility, arena, from.marketdataoptions_},
        genericticklist_(arena, from.genericticklist_) {}

MarketDataRequest::MarketDataRequest(
    ::google::protobuf::Arena* arena,
    const MarketDataRequest& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  MarketDataRequest* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  ::uint32_t cached_has_bits = _impl_._has_bits_[0];
  _impl_.contract_ = (cached_has_bits & 0x00000002u) ? ::google::protobuf::Message::CopyConstruct<::protobuf::Contract>(
                              arena, *from._impl_.contract_)
                        : nullptr;
  ::memcpy(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, reqid_),
           reinterpret_cast<const char *>(&from._impl_) +
               offsetof(Impl_, reqid_),
           offsetof(Impl_, regulatorysnapshot_) -
               offsetof(Impl_, reqid_) +
               sizeof(Impl_::regulatorysnapshot_));

  // @@protoc_insertion_point(copy_constructor:protobuf.MarketDataRequest)
}
inline PROTOBUF_NDEBUG_INLINE MarketDataRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0},
        marketdataoptions_{visibility, arena},
        genericticklist_(arena) {}

inline void MarketDataRequest::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, contract_),
           0,
           offsetof(Impl_, regulatorysnapshot_) -
               offsetof(Impl_, contract_) +
               sizeof(Impl_::regulatorysnapshot_));
}
MarketDataRequest::~MarketDataRequest() {
  // @@protoc_insertion_point(destructor:protobuf.MarketDataRequest)
  SharedDtor(*this);
}
inline void MarketDataRequest::SharedDtor(MessageLite& self) {
  MarketDataRequest& this_ = static_cast<MarketDataRequest&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.genericticklist_.Destroy();
  delete this_._impl_.contract_;
  this_._impl_.~Impl_();
}

inline void* MarketDataRequest::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) MarketDataRequest(arena);
}
constexpr auto MarketDataRequest::InternalNewImpl_() {
  constexpr auto arena_bits = ::google::protobuf::internal::EncodePlacementArenaOffsets({
      PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.marketdataoptions_) +
          decltype(MarketDataRequest::_impl_.marketdataoptions_)::
              InternalGetArenaOffset(
                  ::google::protobuf::Message::internal_visibility()),
      PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.marketdataoptions_) +
          decltype(MarketDataRequest::_impl_.marketdataoptions_)::
              InternalGetArenaOffsetAlt(
                  ::google::protobuf::Message::internal_visibility()),
  });
  if (arena_bits.has_value()) {
    return ::google::protobuf::internal::MessageCreator::CopyInit(
        sizeof(MarketDataRequest), alignof(MarketDataRequest), *arena_bits);
  } else {
    return ::google::protobuf::internal::MessageCreator(&MarketDataRequest::PlacementNew_,
                                 sizeof(MarketDataRequest),
                                 alignof(MarketDataRequest));
  }
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull MarketDataRequest::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_MarketDataRequest_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &MarketDataRequest::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<MarketDataRequest>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &MarketDataRequest::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<MarketDataRequest>(), &MarketDataRequest::ByteSizeLong,
            &MarketDataRequest::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_._cached_size_),
        false,
    },
    &MarketDataRequest::kDescriptorMethods,
    &descriptor_table_MarketDataRequest_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* MarketDataRequest::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<3, 6, 2, 67, 2> MarketDataRequest::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_._has_bits_),
    0, // no _extensions_
    6, 56,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967232,  // skipmap
    offsetof(decltype(_table_), field_entries),
    6,  // num_field_entries
    2,  // num_aux_entries
    offsetof(decltype(_table_), aux_entries),
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::MarketDataRequest>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // optional int32 reqId = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDataRequest, _impl_.reqid_), 2>(),
     {8, 2, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.reqid_)}},
    // optional .protobuf.Contract contract = 2;
    {::_pbi::TcParser::FastMtS1,
     {18, 1, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.contract_)}},
    // optional string genericTickList = 3;
    {::_pbi::TcParser::FastUS1,
     {26, 0, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.genericticklist_)}},
    // optional bool snapshot = 4;
    {::_pbi::TcParser::SingularVarintNoZag1<bool, offsetof(MarketDataRequest, _impl_.snapshot_), 3>(),
     {32, 3, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.snapshot_)}},
    // optional bool regulatorySnapshot = 5;
    {::_pbi::TcParser::SingularVarintNoZag1<bool, offsetof(MarketDataRequest, _impl_.regulatorysnapshot_), 4>(),
     {40, 4, 0, PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.regulatorysnapshot_)}},
    {::_pbi::TcParser::MiniParse, {}},
    {::_pbi::TcParser::MiniParse, {}},
  }}, {{
    65535, 65535
  }}, {{
    // optional int32 reqId = 1;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.reqid_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional .protobuf.Contract contract = 2;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.contract_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kMessage | ::_fl::kTvTable)},
    // optional string genericTickList = 3;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.genericticklist_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // optional bool snapshot = 4;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.snapshot_), _Internal::kHasBitsOffset + 3, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBool)},
    // optional bool regulatorySnapshot = 5;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.regulatorysnapshot_), _Internal::kHasBitsOffset + 4, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBool)},
    // map<string, string> marketDataOptions = 6;
    {PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.marketdataoptions_), -1, 1,
    (0 | ::_fl::kFcRepeated | ::_fl::kMap)},
  }}, {{
    {::_pbi::TcParser::GetTable<::protobuf::Contract>()},
    {::_pbi::TcParser::GetMapAuxInfo<
        decltype(MarketDataRequest()._impl_.marketdataoptions_)>(
        1, 0, 0, 9,
        9)},
  }}, {{
    "\32\0\0\17\0\0\21\0"
    "protobuf.MarketDataRequest"
    "genericTickList"
    "marketDataOptions"
  }},
};

PROTOBUF_NOINLINE void MarketDataRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.MarketDataRequest)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.marketdataoptions_.Clear();
  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _impl_.genericticklist_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      ABSL_DCHECK(_impl_.contract_ != nullptr);
      _impl_.contract_->Clear();
    }
  }
  if (cached_has_bits & 0x0000001cu) {
    ::memset(&_impl_.reqid_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.regulatorysnapshot_) -
        reinterpret_cast<char*>(&_impl_.reqid_)) + sizeof(_impl_.regulatorysnapshot_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* MarketDataRequest::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const MarketDataRequest& this_ = static_cast<const MarketDataRequest&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* MarketDataRequest::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const MarketDataRequest& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.MarketDataRequest)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 reqId = 1;
          if (cached_has_bits & 0x00000004u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_reqid(), target);
          }

          // optional .protobuf.Contract contract = 2;
          if (cached_has_bits & 0x00000002u) {
            target = ::google::protobuf::internal::WireFormatLite::InternalWriteMessage(
                2, *this_._impl_.contract_, this_._impl_.contract_->GetCachedSize(), target,
                stream);
          }

          // optional string genericTickList = 3;
          if (cached_has_bits & 0x00000001u) {
            const std::string& _s = this_._internal_genericticklist();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDataRequest.genericTickList");
            target = stream->WriteStringMaybeAliased(3, _s, target);
          }

          // optional bool snapshot = 4;
          if (cached_has_bits & 0x00000008u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteBoolToArray(
                4, this_._internal_snapshot(), target);
          }

          // optional bool regulatorySnapshot = 5;
          if (cached_has_bits & 0x00000010u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteBoolToArray(
                5, this_._internal_regulatorysnapshot(), target);
          }

          // map<string, string> marketDataOptions = 6;
          if (!this_._internal_marketdataoptions().empty()) {
            using MapType = ::google::protobuf::Map<std::string, std::string>;
            using WireHelper = _pbi::MapEntryFuncs<std::string, std::string,
                                           _pbi::WireFormatLite::TYPE_STRING,
                                           _pbi::WireFormatLite::TYPE_STRING>;
            const auto& field = this_._internal_marketdataoptions();

            if (stream->IsSerializationDeterministic() && field.size() > 1) {
              for (const auto& entry : ::google::protobuf::internal::MapSorterPtr<MapType>(field)) {
                target = WireHelper::InternalSerialize(
                    6, entry.first, entry.second, target, stream);
                ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                    entry.first.data(), static_cast<int>(entry.first.length()),
 ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDataRequest.marketDataOptions");
                ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                    entry.second.data(), static_cast<int>(entry.second.length()),
 ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDataRequest.marketDataOptions");
              }
            } else {
              for (const auto& entry : field) {
                target = WireHelper::InternalSerialize(
                    6, entry.first, entry.second, target, stream);
                ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                    entry.first.data(), static_cast<int>(entry.first.length()),
 ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDataRequest.marketDataOptions");
                ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                    entry.second.data(), static_cast<int>(entry.second.length()),
 ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDataRequest.marketDataOptions");
              }
            }
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.MarketDataRequest)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t MarketDataRequest::ByteSizeLong(const MessageLite& base) {
          const MarketDataRequest& this_ = static_cast<const MarketDataRequest&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t MarketDataRequest::ByteSizeLong() const {
          const MarketDataRequest& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.MarketDataRequest)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
           {
            // map<string, string> marketDataOptions = 6;
            {
              total_size +=
                  1 * ::google::protobuf::internal::FromIntSize(this_._internal_marketdataoptions_size());
              for (const auto& entry : this_._internal_marketdataoptions()) {
                total_size += _pbi::MapEntryFuncs<std::string, std::string,
                                               _pbi::WireFormatLite::TYPE_STRING,
                                               _pbi::WireFormatLite::TYPE_STRING>::ByteSizeLong(entry.first, entry.second);
              }
            }
          }
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x0000001fu) {
            // optional string genericTickList = 3;
            if (cached_has_bits & 0x00000001u) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_genericticklist());
            }
            // optional .protobuf.Contract contract = 2;
            if (cached_has_bits & 0x00000002u) {
              total_size += 1 +
                            ::google::protobuf::internal::WireFormatLite::MessageSize(*this_._impl_.contract_);
            }
            // optional int32 reqId = 1;
            if (cached_has_bits & 0x00000004u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_reqid());
            }
            // optional bool snapshot = 4;
            if (cached_has_bits & 0x00000008u) {
              total_size += 2;
            }
            // optional bool regulatorySnapshot = 5;
            if (cached_has_bits & 0x00000010u) {
              total_size += 2;
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void MarketDataRequest::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<MarketDataRequest*>(&to_msg);
  auto& from = static_cast<const MarketDataRequest&>(from_msg);
  ::google::protobuf::Arena* arena = _this->GetArena();
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.MarketDataRequest)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.marketdataoptions_.MergeFrom(from._impl_.marketdataoptions_);
  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      _this->_internal_set_genericticklist(from._internal_genericticklist());
    }
    if (cached_has_bits & 0x00000002u) {
      ABSL_DCHECK(from._impl_.contract_ != nullptr);
      if (_this->_impl_.contract_ == nullptr) {
        _this->_impl_.contract_ =
            ::google::protobuf::Message::CopyConstruct<::protobuf::Contract>(arena, *from._impl_.contract_);
      } else {
        _this->_impl_.contract_->MergeFrom(*from._impl_.contract_);
      }
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.reqid_ = from._impl_.reqid_;
    }
    if (cached_has_bits & 0x00000008u) {
      _this->_impl_.snapshot_ = from._impl_.snapshot_;
    }
    if (cached_has_bits & 0x00000010u) {
      _this->_impl_.regulatorysnapshot_ = from._impl_.regulatorysnapshot_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void MarketDataRequest::CopyFrom(const MarketDataRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.MarketDataRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void MarketDataRequest::InternalSwap(MarketDataRequest* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  _impl_.marketdataoptions_.InternalSwap(&other->_impl_.marketdataoptions_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.genericticklist_, &other->_impl_.genericticklist_, arena);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.regulatorysnapshot_)
      + sizeof(MarketDataRequest::_impl_.regulatorysnapshot_)
      - PROTOBUF_FIELD_OFFSET(MarketDataRequest, _impl_.contract_)>(
          reinterpret_cast<char*>(&_impl_.contract_),
          reinterpret_cast<char*>(&other->_impl_.contract_));
}

::google::protobuf::Metadata MarketDataRequest::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::std::false_type
    _static_init2_ PROTOBUF_UNUSED =
        (::_pbi::AddDescriptors(&descriptor_table_MarketDataRequest_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
