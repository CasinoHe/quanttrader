// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: MarketDepthData.proto
// Protobuf C++ Version: 5.29.1

#include "MarketDepthData.pb.h"

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

inline constexpr MarketDepthData::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        size_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        marketmaker_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        position_{0},
        operation_{0},
        price_{0},
        side_{0},
        issmartdepth_{false} {}

template <typename>
PROTOBUF_CONSTEXPR MarketDepthData::MarketDepthData(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct MarketDepthDataDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MarketDepthDataDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MarketDepthDataDefaultTypeInternal() {}
  union {
    MarketDepthData _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MarketDepthDataDefaultTypeInternal _MarketDepthData_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_MarketDepthData_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_MarketDepthData_2eproto = nullptr;
const ::uint32_t
    TableStruct_MarketDepthData_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.position_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.operation_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.side_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.price_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.size_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.marketmaker_),
        PROTOBUF_FIELD_OFFSET(::protobuf::MarketDepthData, _impl_.issmartdepth_),
        2,
        3,
        5,
        4,
        0,
        1,
        6,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 15, -1, sizeof(::protobuf::MarketDepthData)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_MarketDepthData_default_instance_._instance,
};
const char descriptor_table_protodef_MarketDepthData_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\025MarketDepthData.proto\022\010protobuf\"\207\002\n\017Ma"
    "rketDepthData\022\025\n\010position\030\001 \001(\005H\000\210\001\001\022\026\n\t"
    "operation\030\002 \001(\005H\001\210\001\001\022\021\n\004side\030\003 \001(\005H\002\210\001\001\022"
    "\022\n\005price\030\004 \001(\001H\003\210\001\001\022\021\n\004size\030\005 \001(\tH\004\210\001\001\022\030"
    "\n\013marketMaker\030\006 \001(\tH\005\210\001\001\022\031\n\014isSmartDepth"
    "\030\007 \001(\010H\006\210\001\001B\013\n\t_positionB\014\n\n_operationB\007"
    "\n\005_sideB\010\n\006_priceB\007\n\005_sizeB\016\n\014_marketMak"
    "erB\017\n\r_isSmartDepthB\?\n\026com.ib.client.pro"
    "tobufB\024MarketDepthDataProto\252\002\016IBApi.prot"
    "obufb\006proto3"
};
static ::absl::once_flag descriptor_table_MarketDepthData_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_MarketDepthData_2eproto = {
    false,
    false,
    372,
    descriptor_table_protodef_MarketDepthData_2eproto,
    "MarketDepthData.proto",
    &descriptor_table_MarketDepthData_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_MarketDepthData_2eproto::offsets,
    file_level_enum_descriptors_MarketDepthData_2eproto,
    file_level_service_descriptors_MarketDepthData_2eproto,
};
namespace protobuf {
// ===================================================================

class MarketDepthData::_Internal {
 public:
  using HasBits =
      decltype(std::declval<MarketDepthData>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_._has_bits_);
};

MarketDepthData::MarketDepthData(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.MarketDepthData)
}
inline PROTOBUF_NDEBUG_INLINE MarketDepthData::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::protobuf::MarketDepthData& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        size_(arena, from.size_),
        marketmaker_(arena, from.marketmaker_) {}

MarketDepthData::MarketDepthData(
    ::google::protobuf::Arena* arena,
    const MarketDepthData& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  MarketDepthData* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  ::memcpy(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, position_),
           reinterpret_cast<const char *>(&from._impl_) +
               offsetof(Impl_, position_),
           offsetof(Impl_, issmartdepth_) -
               offsetof(Impl_, position_) +
               sizeof(Impl_::issmartdepth_));

  // @@protoc_insertion_point(copy_constructor:protobuf.MarketDepthData)
}
inline PROTOBUF_NDEBUG_INLINE MarketDepthData::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0},
        size_(arena),
        marketmaker_(arena) {}

inline void MarketDepthData::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, position_),
           0,
           offsetof(Impl_, issmartdepth_) -
               offsetof(Impl_, position_) +
               sizeof(Impl_::issmartdepth_));
}
MarketDepthData::~MarketDepthData() {
  // @@protoc_insertion_point(destructor:protobuf.MarketDepthData)
  SharedDtor(*this);
}
inline void MarketDepthData::SharedDtor(MessageLite& self) {
  MarketDepthData& this_ = static_cast<MarketDepthData&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.size_.Destroy();
  this_._impl_.marketmaker_.Destroy();
  this_._impl_.~Impl_();
}

inline void* MarketDepthData::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) MarketDepthData(arena);
}
constexpr auto MarketDepthData::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(MarketDepthData),
                                            alignof(MarketDepthData));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull MarketDepthData::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_MarketDepthData_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &MarketDepthData::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<MarketDepthData>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &MarketDepthData::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<MarketDepthData>(), &MarketDepthData::ByteSizeLong,
            &MarketDepthData::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_._cached_size_),
        false,
    },
    &MarketDepthData::kDescriptorMethods,
    &descriptor_table_MarketDepthData_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* MarketDepthData::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<3, 7, 0, 48, 2> MarketDepthData::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_._has_bits_),
    0, // no _extensions_
    7, 56,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967168,  // skipmap
    offsetof(decltype(_table_), field_entries),
    7,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::MarketDepthData>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // optional int32 position = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDepthData, _impl_.position_), 2>(),
     {8, 2, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.position_)}},
    // optional int32 operation = 2;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDepthData, _impl_.operation_), 3>(),
     {16, 3, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.operation_)}},
    // optional int32 side = 3;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MarketDepthData, _impl_.side_), 5>(),
     {24, 5, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.side_)}},
    // optional double price = 4;
    {::_pbi::TcParser::FastF64S1,
     {33, 4, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.price_)}},
    // optional string size = 5;
    {::_pbi::TcParser::FastUS1,
     {42, 0, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.size_)}},
    // optional string marketMaker = 6;
    {::_pbi::TcParser::FastUS1,
     {50, 1, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.marketmaker_)}},
    // optional bool isSmartDepth = 7;
    {::_pbi::TcParser::SingularVarintNoZag1<bool, offsetof(MarketDepthData, _impl_.issmartdepth_), 6>(),
     {56, 6, 0, PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.issmartdepth_)}},
  }}, {{
    65535, 65535
  }}, {{
    // optional int32 position = 1;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.position_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional int32 operation = 2;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.operation_), _Internal::kHasBitsOffset + 3, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional int32 side = 3;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.side_), _Internal::kHasBitsOffset + 5, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional double price = 4;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.price_), _Internal::kHasBitsOffset + 4, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional string size = 5;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.size_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // optional string marketMaker = 6;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.marketmaker_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // optional bool isSmartDepth = 7;
    {PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.issmartdepth_), _Internal::kHasBitsOffset + 6, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBool)},
  }},
  // no aux_entries
  {{
    "\30\0\0\0\0\4\13\0"
    "protobuf.MarketDepthData"
    "size"
    "marketMaker"
  }},
};

PROTOBUF_NOINLINE void MarketDepthData::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.MarketDepthData)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _impl_.size_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      _impl_.marketmaker_.ClearNonDefaultToEmpty();
    }
  }
  if (cached_has_bits & 0x0000007cu) {
    ::memset(&_impl_.position_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.issmartdepth_) -
        reinterpret_cast<char*>(&_impl_.position_)) + sizeof(_impl_.issmartdepth_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* MarketDepthData::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const MarketDepthData& this_ = static_cast<const MarketDepthData&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* MarketDepthData::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const MarketDepthData& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.MarketDepthData)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 position = 1;
          if (cached_has_bits & 0x00000004u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_position(), target);
          }

          // optional int32 operation = 2;
          if (cached_has_bits & 0x00000008u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<2>(
                    stream, this_._internal_operation(), target);
          }

          // optional int32 side = 3;
          if (cached_has_bits & 0x00000020u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<3>(
                    stream, this_._internal_side(), target);
          }

          // optional double price = 4;
          if (cached_has_bits & 0x00000010u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                4, this_._internal_price(), target);
          }

          // optional string size = 5;
          if (cached_has_bits & 0x00000001u) {
            const std::string& _s = this_._internal_size();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDepthData.size");
            target = stream->WriteStringMaybeAliased(5, _s, target);
          }

          // optional string marketMaker = 6;
          if (cached_has_bits & 0x00000002u) {
            const std::string& _s = this_._internal_marketmaker();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "protobuf.MarketDepthData.marketMaker");
            target = stream->WriteStringMaybeAliased(6, _s, target);
          }

          // optional bool isSmartDepth = 7;
          if (cached_has_bits & 0x00000040u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteBoolToArray(
                7, this_._internal_issmartdepth(), target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.MarketDepthData)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t MarketDepthData::ByteSizeLong(const MessageLite& base) {
          const MarketDepthData& this_ = static_cast<const MarketDepthData&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t MarketDepthData::ByteSizeLong() const {
          const MarketDepthData& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.MarketDepthData)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x0000007fu) {
            // optional string size = 5;
            if (cached_has_bits & 0x00000001u) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_size());
            }
            // optional string marketMaker = 6;
            if (cached_has_bits & 0x00000002u) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_marketmaker());
            }
            // optional int32 position = 1;
            if (cached_has_bits & 0x00000004u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_position());
            }
            // optional int32 operation = 2;
            if (cached_has_bits & 0x00000008u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_operation());
            }
            // optional double price = 4;
            if (cached_has_bits & 0x00000010u) {
              total_size += 9;
            }
            // optional int32 side = 3;
            if (cached_has_bits & 0x00000020u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_side());
            }
            // optional bool isSmartDepth = 7;
            if (cached_has_bits & 0x00000040u) {
              total_size += 2;
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void MarketDepthData::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<MarketDepthData*>(&to_msg);
  auto& from = static_cast<const MarketDepthData&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.MarketDepthData)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x0000007fu) {
    if (cached_has_bits & 0x00000001u) {
      _this->_internal_set_size(from._internal_size());
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_internal_set_marketmaker(from._internal_marketmaker());
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.position_ = from._impl_.position_;
    }
    if (cached_has_bits & 0x00000008u) {
      _this->_impl_.operation_ = from._impl_.operation_;
    }
    if (cached_has_bits & 0x00000010u) {
      _this->_impl_.price_ = from._impl_.price_;
    }
    if (cached_has_bits & 0x00000020u) {
      _this->_impl_.side_ = from._impl_.side_;
    }
    if (cached_has_bits & 0x00000040u) {
      _this->_impl_.issmartdepth_ = from._impl_.issmartdepth_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void MarketDepthData::CopyFrom(const MarketDepthData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.MarketDepthData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void MarketDepthData::InternalSwap(MarketDepthData* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.size_, &other->_impl_.size_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.marketmaker_, &other->_impl_.marketmaker_, arena);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.issmartdepth_)
      + sizeof(MarketDepthData::_impl_.issmartdepth_)
      - PROTOBUF_FIELD_OFFSET(MarketDepthData, _impl_.position_)>(
          reinterpret_cast<char*>(&_impl_.position_),
          reinterpret_cast<char*>(&other->_impl_.position_));
}

::google::protobuf::Metadata MarketDepthData::GetMetadata() const {
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
        (::_pbi::AddDescriptors(&descriptor_table_MarketDepthData_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
