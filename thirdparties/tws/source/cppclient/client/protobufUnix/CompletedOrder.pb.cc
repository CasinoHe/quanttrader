// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: CompletedOrder.proto
// Protobuf C++ Version: 5.29.1

#include "CompletedOrder.pb.h"

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

inline constexpr CompletedOrder::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        contract_{nullptr},
        order_{nullptr},
        orderstate_{nullptr} {}

template <typename>
PROTOBUF_CONSTEXPR CompletedOrder::CompletedOrder(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct CompletedOrderDefaultTypeInternal {
  PROTOBUF_CONSTEXPR CompletedOrderDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~CompletedOrderDefaultTypeInternal() {}
  union {
    CompletedOrder _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 CompletedOrderDefaultTypeInternal _CompletedOrder_default_instance_;
}  // namespace protobuf
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_CompletedOrder_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_CompletedOrder_2eproto = nullptr;
const ::uint32_t
    TableStruct_CompletedOrder_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::protobuf::CompletedOrder, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::protobuf::CompletedOrder, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::protobuf::CompletedOrder, _impl_.contract_),
        PROTOBUF_FIELD_OFFSET(::protobuf::CompletedOrder, _impl_.order_),
        PROTOBUF_FIELD_OFFSET(::protobuf::CompletedOrder, _impl_.orderstate_),
        0,
        1,
        2,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 11, -1, sizeof(::protobuf::CompletedOrder)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::protobuf::_CompletedOrder_default_instance_._instance,
};
const char descriptor_table_protodef_CompletedOrder_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\024CompletedOrder.proto\022\010protobuf\032\016Contra"
    "ct.proto\032\013Order.proto\032\020OrderState.proto\""
    "\265\001\n\016CompletedOrder\022)\n\010contract\030\001 \001(\0132\022.p"
    "rotobuf.ContractH\000\210\001\001\022#\n\005order\030\002 \001(\0132\017.p"
    "rotobuf.OrderH\001\210\001\001\022-\n\norderState\030\003 \001(\0132\024"
    ".protobuf.OrderStateH\002\210\001\001B\013\n\t_contractB\010"
    "\n\006_orderB\r\n\013_orderStateB>\n\026com.ib.client"
    ".protobufB\023CompletedOrderProto\252\002\016IBApi.p"
    "rotobufb\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_CompletedOrder_2eproto_deps[3] =
    {
        &::descriptor_table_Contract_2eproto,
        &::descriptor_table_Order_2eproto,
        &::descriptor_table_OrderState_2eproto,
};
static ::absl::once_flag descriptor_table_CompletedOrder_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_CompletedOrder_2eproto = {
    false,
    false,
    335,
    descriptor_table_protodef_CompletedOrder_2eproto,
    "CompletedOrder.proto",
    &descriptor_table_CompletedOrder_2eproto_once,
    descriptor_table_CompletedOrder_2eproto_deps,
    3,
    1,
    schemas,
    file_default_instances,
    TableStruct_CompletedOrder_2eproto::offsets,
    file_level_enum_descriptors_CompletedOrder_2eproto,
    file_level_service_descriptors_CompletedOrder_2eproto,
};
namespace protobuf {
// ===================================================================

class CompletedOrder::_Internal {
 public:
  using HasBits =
      decltype(std::declval<CompletedOrder>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_._has_bits_);
};

void CompletedOrder::clear_contract() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.contract_ != nullptr) _impl_.contract_->Clear();
  _impl_._has_bits_[0] &= ~0x00000001u;
}
void CompletedOrder::clear_order() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.order_ != nullptr) _impl_.order_->Clear();
  _impl_._has_bits_[0] &= ~0x00000002u;
}
void CompletedOrder::clear_orderstate() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  if (_impl_.orderstate_ != nullptr) _impl_.orderstate_->Clear();
  _impl_._has_bits_[0] &= ~0x00000004u;
}
CompletedOrder::CompletedOrder(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protobuf.CompletedOrder)
}
inline PROTOBUF_NDEBUG_INLINE CompletedOrder::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::protobuf::CompletedOrder& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0} {}

CompletedOrder::CompletedOrder(
    ::google::protobuf::Arena* arena,
    const CompletedOrder& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  CompletedOrder* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  ::uint32_t cached_has_bits = _impl_._has_bits_[0];
  _impl_.contract_ = (cached_has_bits & 0x00000001u) ? ::google::protobuf::Message::CopyConstruct<::protobuf::Contract>(
                              arena, *from._impl_.contract_)
                        : nullptr;
  _impl_.order_ = (cached_has_bits & 0x00000002u) ? ::google::protobuf::Message::CopyConstruct<::protobuf::Order>(
                              arena, *from._impl_.order_)
                        : nullptr;
  _impl_.orderstate_ = (cached_has_bits & 0x00000004u) ? ::google::protobuf::Message::CopyConstruct<::protobuf::OrderState>(
                              arena, *from._impl_.orderstate_)
                        : nullptr;

  // @@protoc_insertion_point(copy_constructor:protobuf.CompletedOrder)
}
inline PROTOBUF_NDEBUG_INLINE CompletedOrder::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void CompletedOrder::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, contract_),
           0,
           offsetof(Impl_, orderstate_) -
               offsetof(Impl_, contract_) +
               sizeof(Impl_::orderstate_));
}
CompletedOrder::~CompletedOrder() {
  // @@protoc_insertion_point(destructor:protobuf.CompletedOrder)
  SharedDtor(*this);
}
inline void CompletedOrder::SharedDtor(MessageLite& self) {
  CompletedOrder& this_ = static_cast<CompletedOrder&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  delete this_._impl_.contract_;
  delete this_._impl_.order_;
  delete this_._impl_.orderstate_;
  this_._impl_.~Impl_();
}

inline void* CompletedOrder::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) CompletedOrder(arena);
}
constexpr auto CompletedOrder::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::ZeroInit(sizeof(CompletedOrder),
                                            alignof(CompletedOrder));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull CompletedOrder::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_CompletedOrder_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &CompletedOrder::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<CompletedOrder>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &CompletedOrder::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<CompletedOrder>(), &CompletedOrder::ByteSizeLong,
            &CompletedOrder::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_._cached_size_),
        false,
    },
    &CompletedOrder::kDescriptorMethods,
    &descriptor_table_CompletedOrder_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* CompletedOrder::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 3, 3, 0, 2> CompletedOrder::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_._has_bits_),
    0, // no _extensions_
    3, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967288,  // skipmap
    offsetof(decltype(_table_), field_entries),
    3,  // num_field_entries
    3,  // num_aux_entries
    offsetof(decltype(_table_), aux_entries),
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::protobuf::CompletedOrder>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // optional .protobuf.Contract contract = 1;
    {::_pbi::TcParser::FastMtS1,
     {10, 0, 0, PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.contract_)}},
    // optional .protobuf.Order order = 2;
    {::_pbi::TcParser::FastMtS1,
     {18, 1, 1, PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.order_)}},
    // optional .protobuf.OrderState orderState = 3;
    {::_pbi::TcParser::FastMtS1,
     {26, 2, 2, PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.orderstate_)}},
  }}, {{
    65535, 65535
  }}, {{
    // optional .protobuf.Contract contract = 1;
    {PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.contract_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kMessage | ::_fl::kTvTable)},
    // optional .protobuf.Order order = 2;
    {PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.order_), _Internal::kHasBitsOffset + 1, 1,
    (0 | ::_fl::kFcOptional | ::_fl::kMessage | ::_fl::kTvTable)},
    // optional .protobuf.OrderState orderState = 3;
    {PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.orderstate_), _Internal::kHasBitsOffset + 2, 2,
    (0 | ::_fl::kFcOptional | ::_fl::kMessage | ::_fl::kTvTable)},
  }}, {{
    {::_pbi::TcParser::GetTable<::protobuf::Contract>()},
    {::_pbi::TcParser::GetTable<::protobuf::Order>()},
    {::_pbi::TcParser::GetTable<::protobuf::OrderState>()},
  }}, {{
  }},
};

PROTOBUF_NOINLINE void CompletedOrder::Clear() {
// @@protoc_insertion_point(message_clear_start:protobuf.CompletedOrder)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      ABSL_DCHECK(_impl_.contract_ != nullptr);
      _impl_.contract_->Clear();
    }
    if (cached_has_bits & 0x00000002u) {
      ABSL_DCHECK(_impl_.order_ != nullptr);
      _impl_.order_->Clear();
    }
    if (cached_has_bits & 0x00000004u) {
      ABSL_DCHECK(_impl_.orderstate_ != nullptr);
      _impl_.orderstate_->Clear();
    }
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* CompletedOrder::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const CompletedOrder& this_ = static_cast<const CompletedOrder&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* CompletedOrder::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const CompletedOrder& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:protobuf.CompletedOrder)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional .protobuf.Contract contract = 1;
          if (cached_has_bits & 0x00000001u) {
            target = ::google::protobuf::internal::WireFormatLite::InternalWriteMessage(
                1, *this_._impl_.contract_, this_._impl_.contract_->GetCachedSize(), target,
                stream);
          }

          // optional .protobuf.Order order = 2;
          if (cached_has_bits & 0x00000002u) {
            target = ::google::protobuf::internal::WireFormatLite::InternalWriteMessage(
                2, *this_._impl_.order_, this_._impl_.order_->GetCachedSize(), target,
                stream);
          }

          // optional .protobuf.OrderState orderState = 3;
          if (cached_has_bits & 0x00000004u) {
            target = ::google::protobuf::internal::WireFormatLite::InternalWriteMessage(
                3, *this_._impl_.orderstate_, this_._impl_.orderstate_->GetCachedSize(), target,
                stream);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:protobuf.CompletedOrder)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t CompletedOrder::ByteSizeLong(const MessageLite& base) {
          const CompletedOrder& this_ = static_cast<const CompletedOrder&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t CompletedOrder::ByteSizeLong() const {
          const CompletedOrder& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:protobuf.CompletedOrder)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
          cached_has_bits = this_._impl_._has_bits_[0];
          if (cached_has_bits & 0x00000007u) {
            // optional .protobuf.Contract contract = 1;
            if (cached_has_bits & 0x00000001u) {
              total_size += 1 +
                            ::google::protobuf::internal::WireFormatLite::MessageSize(*this_._impl_.contract_);
            }
            // optional .protobuf.Order order = 2;
            if (cached_has_bits & 0x00000002u) {
              total_size += 1 +
                            ::google::protobuf::internal::WireFormatLite::MessageSize(*this_._impl_.order_);
            }
            // optional .protobuf.OrderState orderState = 3;
            if (cached_has_bits & 0x00000004u) {
              total_size += 1 +
                            ::google::protobuf::internal::WireFormatLite::MessageSize(*this_._impl_.orderstate_);
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void CompletedOrder::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<CompletedOrder*>(&to_msg);
  auto& from = static_cast<const CompletedOrder&>(from_msg);
  ::google::protobuf::Arena* arena = _this->GetArena();
  // @@protoc_insertion_point(class_specific_merge_from_start:protobuf.CompletedOrder)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      ABSL_DCHECK(from._impl_.contract_ != nullptr);
      if (_this->_impl_.contract_ == nullptr) {
        _this->_impl_.contract_ =
            ::google::protobuf::Message::CopyConstruct<::protobuf::Contract>(arena, *from._impl_.contract_);
      } else {
        _this->_impl_.contract_->MergeFrom(*from._impl_.contract_);
      }
    }
    if (cached_has_bits & 0x00000002u) {
      ABSL_DCHECK(from._impl_.order_ != nullptr);
      if (_this->_impl_.order_ == nullptr) {
        _this->_impl_.order_ =
            ::google::protobuf::Message::CopyConstruct<::protobuf::Order>(arena, *from._impl_.order_);
      } else {
        _this->_impl_.order_->MergeFrom(*from._impl_.order_);
      }
    }
    if (cached_has_bits & 0x00000004u) {
      ABSL_DCHECK(from._impl_.orderstate_ != nullptr);
      if (_this->_impl_.orderstate_ == nullptr) {
        _this->_impl_.orderstate_ =
            ::google::protobuf::Message::CopyConstruct<::protobuf::OrderState>(arena, *from._impl_.orderstate_);
      } else {
        _this->_impl_.orderstate_->MergeFrom(*from._impl_.orderstate_);
      }
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void CompletedOrder::CopyFrom(const CompletedOrder& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protobuf.CompletedOrder)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void CompletedOrder::InternalSwap(CompletedOrder* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.orderstate_)
      + sizeof(CompletedOrder::_impl_.orderstate_)
      - PROTOBUF_FIELD_OFFSET(CompletedOrder, _impl_.contract_)>(
          reinterpret_cast<char*>(&_impl_.contract_),
          reinterpret_cast<char*>(&other->_impl_.contract_));
}

::google::protobuf::Metadata CompletedOrder::GetMetadata() const {
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
        (::_pbi::AddDescriptors(&descriptor_table_CompletedOrder_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
