// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: DeltaNeutralContract.proto
// Protobuf C++ Version: 5.29.1

#include "DeltaNeutralContract.pb.h"

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

inline constexpr DeltaNeutralContract::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        delta_{0},
        price_{0},
        conid_{0} {}

template <typename>
PROTOBUF_CONSTEXPR DeltaNeutralContract::DeltaNeutralContract(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct DeltaNeutralContractDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DeltaNeutralContractDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~DeltaNeutralContractDefaultTypeInternal() {}
  union {
    DeltaNeutralContract _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DeltaNeutralContractDefaultTypeInternal _DeltaNeutralContract_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_DeltaNeutralContract_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_DeltaNeutralContract_2eproto = nullptr;
const ::uint32_t
    TableStruct_DeltaNeutralContract_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::DeltaNeutralContract, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::DeltaNeutralContract, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::DeltaNeutralContract, _impl_.conid_),
        PROTOBUF_FIELD_OFFSET(::protobuf::DeltaNeutralContract, _impl_.delta_),
        PROTOBUF_FIELD_OFFSET(::protobuf::DeltaNeutralContract, _impl_.price_),
        2,
        0,
        1,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 11, -1, sizeof(::protobuf::DeltaNeutralContract)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_DeltaNeutralContract_default_instance_._instance,
};
const char descriptor_table_protodef_DeltaNeutralContract_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\032DeltaNeutralContract.proto\022\010protobuf\"p"
    "\n\024DeltaNeutralContract\022\022\n\005conId\030\001 \001(\005H\000\210"
    "\001\001\022\022\n\005delta\030\002 \001(\001H\001\210\001\001\022\022\n\005price\030\003 \001(\001H\002\210"
    "\001\001B\010\n\006_conIdB\010\n\006_deltaB\010\n\006_priceBD\n\026com."
    "ib.client.protobufB\031DeltaNeutralContract"
    "Proto\252\002\016IBApi.protobufb\006proto3"
};
static ::absl::once_flag descriptor_table_DeltaNeutralContract_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_DeltaNeutralContract_2eproto = {
    false,
    false,
    230,
    descriptor_table_protodef_DeltaNeutralContract_2eproto,
    "DeltaNeutralContract.proto",
    &descriptor_table_DeltaNeutralContract_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_DeltaNeutralContract_2eproto::offsets,
    file_level_enum_descriptors_DeltaNeutralContract_2eproto,
    file_level_service_descriptors_DeltaNeutralContract_2eproto,
};
namespace protobuf {
// ===================================================================

class DeltaNeutralContract::_Internal {
 public:
  using HasBits =
      decltype(std::declval<DeltaNeutralContract>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_._has_bits_);
};

DeltaNeutralContract::DeltaNeutralContract(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.DeltaNeutralContract)
}
DeltaNeutralContract::DeltaNeutralContract(
    ::google::protobuf::Arena* arena, const DeltaNeutralContract& from)
    : DeltaNeutralContract(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE DeltaNeutralContract::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void DeltaNeutralContract::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, delta_),
           0,
           offsetof(Impl_, conid_) -
               offsetof(Impl_, delta_) +
               sizeof(Impl_::conid_));
}
DeltaNeutralContract::~DeltaNeutralContract() {
  // @@protoc_insertion_point(destructor:protobuf.DeltaNeutralContract)
  SharedDtor(*this);
}
inline void DeltaNeutralContract::SharedDtor(MessageLite& self) {
  DeltaNeutralContract& this_ = static_cast<DeltaNeutralContract&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.~Impl_();
}

inline void* DeltaNeutralContract::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) DeltaNeutralContract(arena);
}
constexpr auto DeltaNeutralContract::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::ZeroInit(sizeof(DeltaNeutralContract),
                                            alignof(DeltaNeutralContract));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull DeltaNeutralContract::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_DeltaNeutralContract_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &DeltaNeutralContract::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<DeltaNeutralContract>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &DeltaNeutralContract::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<DeltaNeutralContract>(), &DeltaNeutralContract::ByteSizeLong,
            &DeltaNeutralContract::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_._cached_size_),
        false,
    },
    &DeltaNeutralContract::kDescriptorMethods,
    &descriptor_table_DeltaNeutralContract_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* DeltaNeutralContract::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 3, 0, 0, 2> DeltaNeutralContract::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_._has_bits_),
    0, // no _extensions_
    3, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967288,  // skipmap
    offsetof(decltype(_table_), field_entries),
    3,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::DeltaNeutralContract>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // optional int32 conId = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(DeltaNeutralContract, _impl_.conid_), 2>(),
     {8, 2, 0, PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.conid_)}},
    // optional double delta = 2;
    {::_pbi::TcParser::FastF64S1,
     {17, 0, 0, PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.delta_)}},
    // optional double price = 3;
    {::_pbi::TcParser::FastF64S1,
     {25, 1, 0, PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.price_)}},
  }}, {{
    65535, 65535
  }}, {{
    // optional int32 conId = 1;
    {PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.conid_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional double delta = 2;
    {PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.delta_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
    // optional double price = 3;
    {PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.price_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kDouble)},
  }},
  // no aux_entries
  {{
  }},
};

PROTOBUF_NOINLINE void DeltaNeutralContract::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.DeltaNeutralContract)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    ::memset(&_impl_.delta_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.conid_) -
        reinterpret_cast<char*>(&_impl_.delta_)) + sizeof(_impl_.conid_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* DeltaNeutralContract::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const DeltaNeutralContract& this_ = static_cast<const DeltaNeutralContract&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* DeltaNeutralContract::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const DeltaNeutralContract& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.DeltaNeutralContract)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 conId = 1;
          if (cached_has_bits & 0x00000004u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_conid(), target);
          }

          // optional double delta = 2;
          if (cached_has_bits & 0x00000001u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                2, this_._internal_delta(), target);
          }

          // optional double price = 3;
          if (cached_has_bits & 0x00000002u) {
            target = stream->EnsureSpace(target);
            target = ::_pbi::WireFormatLite::WriteDoubleToArray(
                3, this_._internal_price(), target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.DeltaNeutralContract)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t DeltaNeutralContract::ByteSizeLong(const MessageLite& base) {
          const DeltaNeutralContract& this_ = static_cast<const DeltaNeutralContract&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t DeltaNeutralContract::ByteSizeLong() const {
          const DeltaNeutralContract& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.DeltaNeutralContract)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x00000007u) {
            // optional double delta = 2;
            if (cached_has_bits & 0x00000001u) {
              total_size += 9;
            }
            // optional double price = 3;
            if (cached_has_bits & 0x00000002u) {
              total_size += 9;
            }
            // optional int32 conId = 1;
            if (cached_has_bits & 0x00000004u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_conid());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void DeltaNeutralContract::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<DeltaNeutralContract*>(&to_msg);
  auto& from = static_cast<const DeltaNeutralContract&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.DeltaNeutralContract)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      _this->_impl_.delta_ = from._impl_.delta_;
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_impl_.price_ = from._impl_.price_;
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.conid_ = from._impl_.conid_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void DeltaNeutralContract::CopyFrom(const DeltaNeutralContract& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.DeltaNeutralContract)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void DeltaNeutralContract::InternalSwap(DeltaNeutralContract* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.conid_)
      + sizeof(DeltaNeutralContract::_impl_.conid_)
      - PROTOBUF_FIELD_OFFSET(DeltaNeutralContract, _impl_.delta_)>(
          reinterpret_cast<char*>(&_impl_.delta_),
          reinterpret_cast<char*>(&other->_impl_.delta_));
}

::google::protobuf::Metadata DeltaNeutralContract::GetMetadata() const {
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
        (::_pbi::AddDescriptors(&descriptor_table_DeltaNeutralContract_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
