// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: MarketDataType.proto
// Protobuf C++ Version: 5.29.1

#include "MarketDataType.pb.h"

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

inline constexpr MarketDataType::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        reqid_{0},
        marketdatatype_{0} {}

template <typename>
PROTOBUF_CONSTEXPR MarketDataType::MarketDataType(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct MarketDataTypeDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MarketDataTypeDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MarketDataTypeDefaultTypeInternal() {}
  union {
    MarketDataType _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MarketDataTypeDefaultTypeInternal _MarketDataType_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_MarketDataType_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_MarketDataType_2eproto = nullptr;
const ::uint32_t
    TableStruct_MarketDataType_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataType, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataType, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataType, _impl_.reqid_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDataType, _impl_.marketdatatype_),
        0,
        1,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 10, -1, sizeof(::protobuf::MarketDataType)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_MarketDataType_default_instance_._instance,
};
const char descriptor_table_protodef_MarketDataType_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\024MarketDataType.proto\022\010protobuf\"^\n\016Mark"
    "etDataType\022\022\n\005reqId\030\001 \001(\005H\000\210\001\001\022\033\n\016market"
    "DataType\030\002 \001(\005H\001\210\001\001B\010\n\006_reqIdB\021\n\017_market"
    "DataTypeB>\n\026com.ib.client.protobufB\023Mark"
    "etDataTypeProto\252\002\016IBApi.protobufb\006proto3"
};
static ::absl::once_flag descriptor_table_MarketDataType_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_MarketDataType_2eproto = {
    false,
    false,
    200,
    descriptor_table_protodef_MarketDataType_2eproto,
    "MarketDataType.proto",
    &descriptor_table_MarketDataType_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_MarketDataType_2eproto::offsets,
    file_level_enum_descriptors_MarketDataType_2eproto,
    file_level_service_descriptors_MarketDataType_2eproto,
};
namespace protobuf {
// ===================================================================

class MarketDataType::_Internal {
 public:
  using HasBits =
      decltype(std::declval<MarketDataType>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_._has_bits_);
};

MarketDataType::MarketDataType(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.MarketDataType)
}
MarketDataType::MarketDataType(
    ::google::protobuf::Arena* arena, const MarketDataType& from)
    : MarketDataType(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE MarketDataType::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void MarketDataType::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, reqid_),
           0,
           offsetof(Impl_, marketdatatype_) -
               offsetof(Impl_, reqid_) +
               sizeof(Impl_::marketdatatype_));
}
MarketDataType::~MarketDataType() {
  // @@protoc_insertion_point(destructor:protobuf.MarketDataType)
  SharedDtor(*this);
}
inline void MarketDataType::SharedDtor(MessageLite& self) {
  MarketDataType& this_ = static_cast<MarketDataType&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.~Impl_();
}

inline void* MarketDataType::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) MarketDataType(arena);
}
constexpr auto MarketDataType::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::ZeroInit(sizeof(MarketDataType),
                                            alignof(MarketDataType));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull MarketDataType::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_MarketDataType_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &MarketDataType::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<MarketDataType>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &MarketDataType::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<MarketDataType>(), &MarketDataType::ByteSizeLong,
            &MarketDataType::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_._cached_size_),
        false,
    },
    &MarketDataType::kDescriptorMethods,
    &descriptor_table_MarketDataType_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* MarketDataType::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<1, 2, 0, 0, 2> MarketDataType::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_._has_bits_),
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
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::MarketDataType>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // optional int32 marketDataType = 2;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDataType, _impl_.marketdatatype_), 1>(),
     {16, 1, 0, PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.marketdatatype_)}},
    // optional int32 reqId = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDataType, _impl_.reqid_), 0>(),
     {8, 0, 0, PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.reqid_)}},
  }}, {{
    65535, 65535
  }}, {{
    // optional int32 reqId = 1;
    {PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.reqid_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional int32 marketDataType = 2;
    {PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.marketdatatype_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
  }},
  // no aux_entries
  {{
  }},
};

PROTOBUF_NOINLINE void MarketDataType::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.MarketDataType)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&_impl_.reqid_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.marketdatatype_) -
        reinterpret_cast<char*>(&_impl_.reqid_)) + sizeof(_impl_.marketdatatype_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* MarketDataType::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const MarketDataType& this_ = static_cast<const MarketDataType&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* MarketDataType::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const MarketDataType& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.MarketDataType)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 reqId = 1;
          if (cached_has_bits & 0x00000001u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_reqid(), target);
          }

          // optional int32 marketDataType = 2;
          if (cached_has_bits & 0x00000002u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<2>(
                    stream, this_._internal_marketdatatype(), target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.MarketDataType)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t MarketDataType::ByteSizeLong(const MessageLite& base) {
          const MarketDataType& this_ = static_cast<const MarketDataType&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t MarketDataType::ByteSizeLong() const {
          const MarketDataType& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.MarketDataType)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x00000003u) {
            // optional int32 reqId = 1;
            if (cached_has_bits & 0x00000001u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_reqid());
            }
            // optional int32 marketDataType = 2;
            if (cached_has_bits & 0x00000002u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_marketdatatype());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void MarketDataType::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<MarketDataType*>(&to_msg);
  auto& from = static_cast<const MarketDataType&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.MarketDataType)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _this->_impl_.reqid_ = from._impl_.reqid_;
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_impl_.marketdatatype_ = from._impl_.marketdatatype_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void MarketDataType::CopyFrom(const MarketDataType& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.MarketDataType)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void MarketDataType::InternalSwap(MarketDataType* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.marketdatatype_)
      + sizeof(MarketDataType::_impl_.marketdatatype_)
      - PROTOBUF_FIELD_OFFSET(MarketDataType, _impl_.reqid_)>(
          reinterpret_cast<char*>(&_impl_.reqid_),
          reinterpret_cast<char*>(&other->_impl_.reqid_));
}

::google::protobuf::Metadata MarketDataType::GetMetadata() const {
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
        (::_pbi::AddDescriptors(&descriptor_table_MarketDataType_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
