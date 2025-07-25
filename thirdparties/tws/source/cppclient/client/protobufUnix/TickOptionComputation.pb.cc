// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: TickOptionComputation.proto
// Protobuf C++ Version: 5.29.1

#include "TickOptionComputation.pb.h"

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

inline constexpr TickOptionComputation::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        reqid_{0},
        ticktype_{0},
        impliedvol_{0},
        delta_{0},
        optprice_{0},
        pvdividend_{0},
        gamma_{0},
        vega_{0},
        theta_{0},
        undprice_{0},
        tickattrib_{0} {}

template <typename>
PROTOBUF_CONSTEXPR TickOptionComputation::TickOptionComputation(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct TickOptionComputationDefaultTypeInternal {
  PROTOBUF_CONSTEXPR TickOptionComputationDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~TickOptionComputationDefaultTypeInternal() {}
  union {
    TickOptionComputation _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 TickOptionComputationDefaultTypeInternal _TickOptionComputation_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_TickOptionComputation_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_TickOptionComputation_2eproto = nullptr;
const ::uint32_t
    TableStruct_TickOptionComputation_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.reqid_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.ticktype_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.tickattrib_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.impliedvol_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.delta_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.optprice_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.pvdividend_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.gamma_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.vega_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.theta_),
        PROTOBUF_FIELD_OFFSET(::protobuf::TickOptionComputation, _impl_.undprice_),
        0,
        1,
        10,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 19, -1, sizeof(::protobuf::TickOptionComputation)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_TickOptionComputation_default_instance_._instance,
};
const char descriptor_table_protodef_TickOptionComputation_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\033TickOptionComputation.proto\022\010protobuf\""
    "\217\003\n\025TickOptionComputation\022\022\n\005reqId\030\001 \001(\005"
    "H\000\210\001\001\022\025\n\010tickType\030\002 \001(\005H\001\210\001\001\022\027\n\ntickAttr"
    "ib\030\003 \001(\005H\002\210\001\001\022\027\n\nimpliedVol\030\004 \001(\001H\003\210\001\001\022\022"
    "\n\005delta\030\005 \001(\001H\004\210\001\001\022\025\n\010optPrice\030\006 \001(\001H\005\210\001"
    "\001\022\027\n\npvDividend\030\007 \001(\001H\006\210\001\001\022\022\n\005gamma\030\010 \001("
    "\001H\007\210\001\001\022\021\n\004vega\030\t \001(\001H\010\210\001\001\022\022\n\005theta\030\n \001(\001"
    "H\t\210\001\001\022\025\n\010undPrice\030\013 \001(\001H\n\210\001\001B\010\n\006_reqIdB\013"
    "\n\t_tickTypeB\r\n\013_tickAttribB\r\n\013_impliedVo"
    "lB\010\n\006_deltaB\013\n\t_optPriceB\r\n\013_pvDividendB"
    "\010\n\006_gammaB\007\n\005_vegaB\010\n\006_thetaB\013\n\t_undPric"
    "eBE\n\026com.ib.client.protobufB\032TickOptionC"
    "omputationProto\252\002\016IBApi.protobufb\006proto3"
};
static ::absl::once_flag descriptor_table_TickOptionComputation_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_TickOptionComputation_2eproto = {
    false,
    false,
    520,
    descriptor_table_protodef_TickOptionComputation_2eproto,
    "TickOptionComputation.proto",
    &descriptor_table_TickOptionComputation_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_TickOptionComputation_2eproto::offsets,
    file_level_enum_descriptors_TickOptionComputation_2eproto,
    file_level_service_descriptors_TickOptionComputation_2eproto,
};
namespace protobuf {
// ===================================================================

class TickOptionComputation::_Internal {
 public:
  using HasBits =
      decltype(std::declval<TickOptionComputation>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_._has_bits_);
};

TickOptionComputation::TickOptionComputation(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.TickOptionComputation)
}
TickOptionComputation::TickOptionComputation(
    ::google::protobuf::Arena* arena, const TickOptionComputation& from)
    : TickOptionComputation(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE TickOptionComputation::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void TickOptionComputation::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, reqid_),
           0,
           offsetof(Impl_, tickattrib_) -
               offsetof(Impl_, reqid_) +
               sizeof(Impl_::tickattrib_));
}
TickOptionComputation::~TickOptionComputation() {
  // @@protoc_insertion_point(destructor:protobuf.TickOptionComputation)
  SharedDtor(*this);
}
inline void TickOptionComputation::SharedDtor(MessageLite& self) {
  TickOptionComputation& this_ = static_cast<TickOptionComputation&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.~Impl_();
}

inline void* TickOptionComputation::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) TickOptionComputation(arena);
}
constexpr auto TickOptionComputation::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::ZeroInit(sizeof(TickOptionComputation),
                                            alignof(TickOptionComputation));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull TickOptionComputation::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_TickOptionComputation_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &TickOptionComputation::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<TickOptionComputation>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &TickOptionComputation::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<TickOptionComputation>(), &TickOptionComputation::ByteSizeLong,
            &TickOptionComputation::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_._cached_size_),
        false,
    },
    &TickOptionComputation::kDescriptorMethods,
    &descriptor_table_TickOptionComputation_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* TickOptionComputation::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<4, 11, 0, 0, 2> TickOptionComputation::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_._has_bits_),
    0, // no _extensions_
    11, 120,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294965248,  // skipmap
    offsetof(decltype(_table_), field_entries),
    11,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::TickOptionComputation>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // optional int32 reqId = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(TickOptionComputation, _impl_.reqid_), 0>(),
     {8, 0, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.reqid_)}},
    // optional int32 tickType = 2;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(TickOptionComputation, _impl_.ticktype_), 1>(),
     {16, 1, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.ticktype_)}},
    // optional int32 tickAttrib = 3;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(TickOptionComputation, _impl_.tickattrib_), 10>(),
     {24, 10, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.tickattrib_)}},
    // optional double impliedVol = 4;
    {::_pbi::TcParser::FastF64S1,
     {33, 2, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.impliedvol_)}},
    // optional double delta = 5;
    {::_pbi::TcParser::FastF64S1,
     {41, 3, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.delta_)}},
    // optional double optPrice = 6;
    {::_pbi::TcParser::FastF64S1,
     {49, 4, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.optprice_)}},
    // optional double pvDividend = 7;
    {::_pbi::TcParser::FastF64S1,
     {57, 5, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.pvdividend_)}},
    // optional double gamma = 8;
    {::_pbi::TcParser::FastF64S1,
     {65, 6, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.gamma_)}},
    // optional double vega = 9;
    {::_pbi::TcParser::FastF64S1,
     {73, 7, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.vega_)}},
    // optional double theta = 10;
    {::_pbi::TcParser::FastF64S1,
     {81, 8, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.theta_)}},
    // optional double undPrice = 11;
    {::_pbi::TcParser::FastF64S1,
     {89, 9, 0, PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.undprice_)}},
    {::_pbi::TcParser::MiniParse, {}},
    {::_pbi::TcParser::MiniParse, {}},
    {::_pbi::TcParser::MiniParse, {}},
    {::_pbi::TcParser::MiniParse, {}},
  }}, {{
    65535, 65535
  }}, {{
    // optional int32 reqId = 1;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.reqid_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional int32 tickType = 2;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.ticktype_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional int32 tickAttrib = 3;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.tickattrib_), _Internal::kHasBitsOffset + 10, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional double impliedVol = 4;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.impliedvol_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double delta = 5;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.delta_), _Internal::kHasBitsOffset + 3, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double optPrice = 6;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.optprice_), _Internal::kHasBitsOffset + 4, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double pvDividend = 7;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.pvdividend_), _Internal::kHasBitsOffset + 5, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double gamma = 8;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.gamma_), _Internal::kHasBitsOffset + 6, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double vega = 9;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.vega_), _Internal::kHasBitsOffset + 7, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double theta = 10;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.theta_), _Internal::kHasBitsOffset + 8, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double undPrice = 11;
    {PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.undprice_), _Internal::kHasBitsOffset + 9, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
  }},
  // no aux_entries
  {{
  }},
};

PROTOBUF_NOINLINE void TickOptionComputation::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.TickOptionComputation)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    ::memset(&_impl_.reqid_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.vega_) -
        reinterpret_cast<char*>(&_impl_.reqid_)) + sizeof(_impl_.vega_));
  }
  if (cached_has_bits & 0x00000700u) {
    ::memset(&_impl_.theta_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.tickattrib_) -
        reinterpret_cast<char*>(&_impl_.theta_)) + sizeof(_impl_.tickattrib_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* TickOptionComputation::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const TickOptionComputation& this_ = static_cast<const TickOptionComputation&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* TickOptionComputation::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const TickOptionComputation& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.TickOptionComputation)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 reqId = 1;
          if (cached_has_bits & 0x00000001u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_reqid(), target);
          }

          // optional int32 tickType = 2;
          if (cached_has_bits & 0x00000002u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<2>(
                    stream, this_._internal_ticktype(), target);
          }

          // optional int32 tickAttrib = 3;
          if (cached_has_bits & 0x00000400u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<3>(
                    stream, this_._internal_tickattrib(), target);
          }

          // optional double impliedVol = 4;
          if (cached_has_bits & 0x00000004u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                4, this_._internal_impliedvol(), target);
          }

          // optional double delta = 5;
          if (cached_has_bits & 0x00000008u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                5, this_._internal_delta(), target);
          }

          // optional double optPrice = 6;
          if (cached_has_bits & 0x00000010u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                6, this_._internal_optprice(), target);
          }

          // optional double pvDividend = 7;
          if (cached_has_bits & 0x00000020u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                7, this_._internal_pvdividend(), target);
          }

          // optional double gamma = 8;
          if (cached_has_bits & 0x00000040u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                8, this_._internal_gamma(), target);
          }

          // optional double vega = 9;
          if (cached_has_bits & 0x00000080u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                9, this_._internal_vega(), target);
          }

          // optional double theta = 10;
          if (cached_has_bits & 0x00000100u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                10, this_._internal_theta(), target);
          }

          // optional double undPrice = 11;
          if (cached_has_bits & 0x00000200u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                11, this_._internal_undprice(), target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.TickOptionComputation)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t TickOptionComputation::ByteSizeLong(const MessageLite& base) {
          const TickOptionComputation& this_ = static_cast<const TickOptionComputation&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t TickOptionComputation::ByteSizeLong() const {
          const TickOptionComputation& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.TickOptionComputation)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x000000ffu) {
            // optional int32 reqId = 1;
            if (cached_has_bits & 0x00000001u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_reqid());
            }
            // optional int32 tickType = 2;
            if (cached_has_bits & 0x00000002u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_ticktype());
            }
            // optional double impliedVol = 4;
            if (cached_has_bits & 0x00000004u) {
              total_size += 9;
            }
            // optional double delta = 5;
            if (cached_has_bits & 0x00000008u) {
              total_size += 9;
            }
            // optional double optPrice = 6;
            if (cached_has_bits & 0x00000010u) {
              total_size += 9;
            }
            // optional double pvDividend = 7;
            if (cached_has_bits & 0x00000020u) {
              total_size += 9;
            }
            // optional double gamma = 8;
            if (cached_has_bits & 0x00000040u) {
              total_size += 9;
            }
            // optional double vega = 9;
            if (cached_has_bits & 0x00000080u) {
              total_size += 9;
            }
          }
          if (cached_has_bits & 0x00000700u) {
            // optional double theta = 10;
            if (cached_has_bits & 0x00000100u) {
              total_size += 9;
            }
            // optional double undPrice = 11;
            if (cached_has_bits & 0x00000200u) {
              total_size += 9;
            }
            // optional int32 tickAttrib = 3;
            if (cached_has_bits & 0x00000400u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_tickattrib());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void TickOptionComputation::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<TickOptionComputation*>(&to_msg);
  auto& from = static_cast<const TickOptionComputation&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.TickOptionComputation)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    if (cached_has_bits & 0x00000001u) {
      _this->_impl_.reqid_ = from._impl_.reqid_;
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_impl_.ticktype_ = from._impl_.ticktype_;
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.impliedvol_ = from._impl_.impliedvol_;
    }
    if (cached_has_bits & 0x00000008u) {
      _this->_impl_.delta_ = from._impl_.delta_;
    }
    if (cached_has_bits & 0x00000010u) {
      _this->_impl_.optprice_ = from._impl_.optprice_;
    }
    if (cached_has_bits & 0x00000020u) {
      _this->_impl_.pvdividend_ = from._impl_.pvdividend_;
    }
    if (cached_has_bits & 0x00000040u) {
      _this->_impl_.gamma_ = from._impl_.gamma_;
    }
    if (cached_has_bits & 0x00000080u) {
      _this->_impl_.vega_ = from._impl_.vega_;
    }
  }
  if (cached_has_bits & 0x00000700u) {
    if (cached_has_bits & 0x00000100u) {
      _this->_impl_.theta_ = from._impl_.theta_;
    }
    if (cached_has_bits & 0x00000200u) {
      _this->_impl_.undprice_ = from._impl_.undprice_;
    }
    if (cached_has_bits & 0x00000400u) {
      _this->_impl_.tickattrib_ = from._impl_.tickattrib_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void TickOptionComputation::CopyFrom(const TickOptionComputation& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.TickOptionComputation)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void TickOptionComputation::InternalSwap(TickOptionComputation* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.tickattrib_)
      + sizeof(TickOptionComputation::_impl_.tickattrib_)
      - PROTOBUF_FIELD_OFFSET(TickOptionComputation, _impl_.reqid_)>(
          reinterpret_cast<char*>(&_impl_.reqid_),
          reinterpret_cast<char*>(&other->_impl_.reqid_));
}

::google::protobuf::Metadata TickOptionComputation::GetMetadata() const {
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
        (::_pbi::AddDescriptors(&descriptor_table_TickOptionComputation_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
