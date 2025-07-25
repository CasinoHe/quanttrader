// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: TickGeneric.proto
// Protobuf Java Version: 4.29.3

package com.ib.client.protobuf;

public final class TickGenericProto {
  private TickGenericProto() {}
  static {
    com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
      com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
      /* major= */ 4,
      /* minor= */ 29,
      /* patch= */ 3,
      /* suffix= */ "",
      TickGenericProto.class.getName());
  }
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface TickGenericOrBuilder extends
      // @@protoc_insertion_point(interface_extends:protobuf.TickGeneric)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>optional int32 reqId = 1;</code>
     * @return Whether the reqId field is set.
     */
    boolean hasReqId();
    /**
     * <code>optional int32 reqId = 1;</code>
     * @return The reqId.
     */
    int getReqId();

    /**
     * <code>optional int32 tickType = 2;</code>
     * @return Whether the tickType field is set.
     */
    boolean hasTickType();
    /**
     * <code>optional int32 tickType = 2;</code>
     * @return The tickType.
     */
    int getTickType();

    /**
     * <code>optional double value = 3;</code>
     * @return Whether the value field is set.
     */
    boolean hasValue();
    /**
     * <code>optional double value = 3;</code>
     * @return The value.
     */
    double getValue();
  }
  /**
   * Protobuf type {@code protobuf.TickGeneric}
   */
  public static final class TickGeneric extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:protobuf.TickGeneric)
      TickGenericOrBuilder {
  private static final long serialVersionUID = 0L;
    static {
      com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
        com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
        /* major= */ 4,
        /* minor= */ 29,
        /* patch= */ 3,
        /* suffix= */ "",
        TickGeneric.class.getName());
    }
    // Use TickGeneric.newBuilder() to construct.
    private TickGeneric(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
    }
    private TickGeneric() {
    }

    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.ib.client.protobuf.TickGenericProto.internal_static_protobuf_TickGeneric_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.ib.client.protobuf.TickGenericProto.internal_static_protobuf_TickGeneric_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.ib.client.protobuf.TickGenericProto.TickGeneric.class, com.ib.client.protobuf.TickGenericProto.TickGeneric.Builder.class);
    }

    private int bitField0_;
    public static final int REQID_FIELD_NUMBER = 1;
    private int reqId_ = 0;
    /**
     * <code>optional int32 reqId = 1;</code>
     * @return Whether the reqId field is set.
     */
    @java.lang.Override
    public boolean hasReqId() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <code>optional int32 reqId = 1;</code>
     * @return The reqId.
     */
    @java.lang.Override
    public int getReqId() {
      return reqId_;
    }

    public static final int TICKTYPE_FIELD_NUMBER = 2;
    private int tickType_ = 0;
    /**
     * <code>optional int32 tickType = 2;</code>
     * @return Whether the tickType field is set.
     */
    @java.lang.Override
    public boolean hasTickType() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <code>optional int32 tickType = 2;</code>
     * @return The tickType.
     */
    @java.lang.Override
    public int getTickType() {
      return tickType_;
    }

    public static final int VALUE_FIELD_NUMBER = 3;
    private double value_ = 0D;
    /**
     * <code>optional double value = 3;</code>
     * @return Whether the value field is set.
     */
    @java.lang.Override
    public boolean hasValue() {
      return ((bitField0_ & 0x00000004) != 0);
    }
    /**
     * <code>optional double value = 3;</code>
     * @return The value.
     */
    @java.lang.Override
    public double getValue() {
      return value_;
    }

    private byte memoizedIsInitialized = -1;
    @java.lang.Override
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized == 1) return true;
      if (isInitialized == 0) return false;

      memoizedIsInitialized = 1;
      return true;
    }

    @java.lang.Override
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (((bitField0_ & 0x00000001) != 0)) {
        output.writeInt32(1, reqId_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        output.writeInt32(2, tickType_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        output.writeDouble(3, value_);
      }
      getUnknownFields().writeTo(output);
    }

    @java.lang.Override
    public int getSerializedSize() {
      int size = memoizedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, reqId_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(2, tickType_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeDoubleSize(3, value_);
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSize = size;
      return size;
    }

    @java.lang.Override
    public boolean equals(final java.lang.Object obj) {
      if (obj == this) {
       return true;
      }
      if (!(obj instanceof com.ib.client.protobuf.TickGenericProto.TickGeneric)) {
        return super.equals(obj);
      }
      com.ib.client.protobuf.TickGenericProto.TickGeneric other = (com.ib.client.protobuf.TickGenericProto.TickGeneric) obj;

      if (hasReqId() != other.hasReqId()) return false;
      if (hasReqId()) {
        if (getReqId()
            != other.getReqId()) return false;
      }
      if (hasTickType() != other.hasTickType()) return false;
      if (hasTickType()) {
        if (getTickType()
            != other.getTickType()) return false;
      }
      if (hasValue() != other.hasValue()) return false;
      if (hasValue()) {
        if (java.lang.Double.doubleToLongBits(getValue())
            != java.lang.Double.doubleToLongBits(
                other.getValue())) return false;
      }
      if (!getUnknownFields().equals(other.getUnknownFields())) return false;
      return true;
    }

    @java.lang.Override
    public int hashCode() {
      if (memoizedHashCode != 0) {
        return memoizedHashCode;
      }
      int hash = 41;
      hash = (19 * hash) + getDescriptor().hashCode();
      if (hasReqId()) {
        hash = (37 * hash) + REQID_FIELD_NUMBER;
        hash = (53 * hash) + getReqId();
      }
      if (hasTickType()) {
        hash = (37 * hash) + TICKTYPE_FIELD_NUMBER;
        hash = (53 * hash) + getTickType();
      }
      if (hasValue()) {
        hash = (37 * hash) + VALUE_FIELD_NUMBER;
        hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
            java.lang.Double.doubleToLongBits(getValue()));
      }
      hash = (29 * hash) + getUnknownFields().hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input);
    }

    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.TickGenericProto.TickGeneric parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    @java.lang.Override
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder() {
      return DEFAULT_INSTANCE.toBuilder();
    }
    public static Builder newBuilder(com.ib.client.protobuf.TickGenericProto.TickGeneric prototype) {
      return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
    }
    @java.lang.Override
    public Builder toBuilder() {
      return this == DEFAULT_INSTANCE
          ? new Builder() : new Builder().mergeFrom(this);
    }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code protobuf.TickGeneric}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:protobuf.TickGeneric)
        com.ib.client.protobuf.TickGenericProto.TickGenericOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.ib.client.protobuf.TickGenericProto.internal_static_protobuf_TickGeneric_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.ib.client.protobuf.TickGenericProto.internal_static_protobuf_TickGeneric_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.ib.client.protobuf.TickGenericProto.TickGeneric.class, com.ib.client.protobuf.TickGenericProto.TickGeneric.Builder.class);
      }

      // Construct using com.ib.client.protobuf.TickGenericProto.TickGeneric.newBuilder()
      private Builder() {

      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);

      }
      @java.lang.Override
      public Builder clear() {
        super.clear();
        bitField0_ = 0;
        reqId_ = 0;
        tickType_ = 0;
        value_ = 0D;
        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.ib.client.protobuf.TickGenericProto.internal_static_protobuf_TickGeneric_descriptor;
      }

      @java.lang.Override
      public com.ib.client.protobuf.TickGenericProto.TickGeneric getDefaultInstanceForType() {
        return com.ib.client.protobuf.TickGenericProto.TickGeneric.getDefaultInstance();
      }

      @java.lang.Override
      public com.ib.client.protobuf.TickGenericProto.TickGeneric build() {
        com.ib.client.protobuf.TickGenericProto.TickGeneric result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public com.ib.client.protobuf.TickGenericProto.TickGeneric buildPartial() {
        com.ib.client.protobuf.TickGenericProto.TickGeneric result = new com.ib.client.protobuf.TickGenericProto.TickGeneric(this);
        if (bitField0_ != 0) { buildPartial0(result); }
        onBuilt();
        return result;
      }

      private void buildPartial0(com.ib.client.protobuf.TickGenericProto.TickGeneric result) {
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) != 0)) {
          result.reqId_ = reqId_;
          to_bitField0_ |= 0x00000001;
        }
        if (((from_bitField0_ & 0x00000002) != 0)) {
          result.tickType_ = tickType_;
          to_bitField0_ |= 0x00000002;
        }
        if (((from_bitField0_ & 0x00000004) != 0)) {
          result.value_ = value_;
          to_bitField0_ |= 0x00000004;
        }
        result.bitField0_ |= to_bitField0_;
      }

      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.ib.client.protobuf.TickGenericProto.TickGeneric) {
          return mergeFrom((com.ib.client.protobuf.TickGenericProto.TickGeneric)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.ib.client.protobuf.TickGenericProto.TickGeneric other) {
        if (other == com.ib.client.protobuf.TickGenericProto.TickGeneric.getDefaultInstance()) return this;
        if (other.hasReqId()) {
          setReqId(other.getReqId());
        }
        if (other.hasTickType()) {
          setTickType(other.getTickType());
        }
        if (other.hasValue()) {
          setValue(other.getValue());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        onChanged();
        return this;
      }

      @java.lang.Override
      public final boolean isInitialized() {
        return true;
      }

      @java.lang.Override
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        if (extensionRegistry == null) {
          throw new java.lang.NullPointerException();
        }
        try {
          boolean done = false;
          while (!done) {
            int tag = input.readTag();
            switch (tag) {
              case 0:
                done = true;
                break;
              case 8: {
                reqId_ = input.readInt32();
                bitField0_ |= 0x00000001;
                break;
              } // case 8
              case 16: {
                tickType_ = input.readInt32();
                bitField0_ |= 0x00000002;
                break;
              } // case 16
              case 25: {
                value_ = input.readDouble();
                bitField0_ |= 0x00000004;
                break;
              } // case 25
              default: {
                if (!super.parseUnknownField(input, extensionRegistry, tag)) {
                  done = true; // was an endgroup tag
                }
                break;
              } // default:
            } // switch (tag)
          } // while (!done)
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          throw e.unwrapIOException();
        } finally {
          onChanged();
        } // finally
        return this;
      }
      private int bitField0_;

      private int reqId_ ;
      /**
       * <code>optional int32 reqId = 1;</code>
       * @return Whether the reqId field is set.
       */
      @java.lang.Override
      public boolean hasReqId() {
        return ((bitField0_ & 0x00000001) != 0);
      }
      /**
       * <code>optional int32 reqId = 1;</code>
       * @return The reqId.
       */
      @java.lang.Override
      public int getReqId() {
        return reqId_;
      }
      /**
       * <code>optional int32 reqId = 1;</code>
       * @param value The reqId to set.
       * @return This builder for chaining.
       */
      public Builder setReqId(int value) {

        reqId_ = value;
        bitField0_ |= 0x00000001;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 reqId = 1;</code>
       * @return This builder for chaining.
       */
      public Builder clearReqId() {
        bitField0_ = (bitField0_ & ~0x00000001);
        reqId_ = 0;
        onChanged();
        return this;
      }

      private int tickType_ ;
      /**
       * <code>optional int32 tickType = 2;</code>
       * @return Whether the tickType field is set.
       */
      @java.lang.Override
      public boolean hasTickType() {
        return ((bitField0_ & 0x00000002) != 0);
      }
      /**
       * <code>optional int32 tickType = 2;</code>
       * @return The tickType.
       */
      @java.lang.Override
      public int getTickType() {
        return tickType_;
      }
      /**
       * <code>optional int32 tickType = 2;</code>
       * @param value The tickType to set.
       * @return This builder for chaining.
       */
      public Builder setTickType(int value) {

        tickType_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 tickType = 2;</code>
       * @return This builder for chaining.
       */
      public Builder clearTickType() {
        bitField0_ = (bitField0_ & ~0x00000002);
        tickType_ = 0;
        onChanged();
        return this;
      }

      private double value_ ;
      /**
       * <code>optional double value = 3;</code>
       * @return Whether the value field is set.
       */
      @java.lang.Override
      public boolean hasValue() {
        return ((bitField0_ & 0x00000004) != 0);
      }
      /**
       * <code>optional double value = 3;</code>
       * @return The value.
       */
      @java.lang.Override
      public double getValue() {
        return value_;
      }
      /**
       * <code>optional double value = 3;</code>
       * @param value The value to set.
       * @return This builder for chaining.
       */
      public Builder setValue(double value) {

        value_ = value;
        bitField0_ |= 0x00000004;
        onChanged();
        return this;
      }
      /**
       * <code>optional double value = 3;</code>
       * @return This builder for chaining.
       */
      public Builder clearValue() {
        bitField0_ = (bitField0_ & ~0x00000004);
        value_ = 0D;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:protobuf.TickGeneric)
    }

    // @@protoc_insertion_point(class_scope:protobuf.TickGeneric)
    private static final com.ib.client.protobuf.TickGenericProto.TickGeneric DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new com.ib.client.protobuf.TickGenericProto.TickGeneric();
    }

    public static com.ib.client.protobuf.TickGenericProto.TickGeneric getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static final com.google.protobuf.Parser<TickGeneric>
        PARSER = new com.google.protobuf.AbstractParser<TickGeneric>() {
      @java.lang.Override
      public TickGeneric parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        Builder builder = newBuilder();
        try {
          builder.mergeFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          throw e.setUnfinishedMessage(builder.buildPartial());
        } catch (com.google.protobuf.UninitializedMessageException e) {
          throw e.asInvalidProtocolBufferException().setUnfinishedMessage(builder.buildPartial());
        } catch (java.io.IOException e) {
          throw new com.google.protobuf.InvalidProtocolBufferException(e)
              .setUnfinishedMessage(builder.buildPartial());
        }
        return builder.buildPartial();
      }
    };

    public static com.google.protobuf.Parser<TickGeneric> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<TickGeneric> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public com.ib.client.protobuf.TickGenericProto.TickGeneric getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_protobuf_TickGeneric_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_protobuf_TickGeneric_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\021TickGeneric.proto\022\010protobuf\"m\n\013TickGen" +
      "eric\022\022\n\005reqId\030\001 \001(\005H\000\210\001\001\022\025\n\010tickType\030\002 \001" +
      "(\005H\001\210\001\001\022\022\n\005value\030\003 \001(\001H\002\210\001\001B\010\n\006_reqIdB\013\n" +
      "\t_tickTypeB\010\n\006_valueB;\n\026com.ib.client.pr" +
      "otobufB\020TickGenericProto\252\002\016IBApi.protobu" +
      "fb\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_protobuf_TickGeneric_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_protobuf_TickGeneric_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_protobuf_TickGeneric_descriptor,
        new java.lang.String[] { "ReqId", "TickType", "Value", });
    descriptor.resolveAllFeaturesImmutable();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
