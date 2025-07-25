// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: OrderBound.proto
// Protobuf Java Version: 4.29.3

package com.ib.client.protobuf;

public final class OrderBoundProto {
  private OrderBoundProto() {}
  static {
    com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
      com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
      /* major= */ 4,
      /* minor= */ 29,
      /* patch= */ 3,
      /* suffix= */ "",
      OrderBoundProto.class.getName());
  }
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface OrderBoundOrBuilder extends
      // @@protoc_insertion_point(interface_extends:protobuf.OrderBound)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>optional int64 permId = 1;</code>
     * @return Whether the permId field is set.
     */
    boolean hasPermId();
    /**
     * <code>optional int64 permId = 1;</code>
     * @return The permId.
     */
    long getPermId();

    /**
     * <code>optional int32 clientId = 2;</code>
     * @return Whether the clientId field is set.
     */
    boolean hasClientId();
    /**
     * <code>optional int32 clientId = 2;</code>
     * @return The clientId.
     */
    int getClientId();

    /**
     * <code>optional int32 orderId = 3;</code>
     * @return Whether the orderId field is set.
     */
    boolean hasOrderId();
    /**
     * <code>optional int32 orderId = 3;</code>
     * @return The orderId.
     */
    int getOrderId();
  }
  /**
   * Protobuf type {@code protobuf.OrderBound}
   */
  public static final class OrderBound extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:protobuf.OrderBound)
      OrderBoundOrBuilder {
  private static final long serialVersionUID = 0L;
    static {
      com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
        com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
        /* major= */ 4,
        /* minor= */ 29,
        /* patch= */ 3,
        /* suffix= */ "",
        OrderBound.class.getName());
    }
    // Use OrderBound.newBuilder() to construct.
    private OrderBound(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
    }
    private OrderBound() {
    }

    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.ib.client.protobuf.OrderBoundProto.internal_static_protobuf_OrderBound_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.ib.client.protobuf.OrderBoundProto.internal_static_protobuf_OrderBound_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.ib.client.protobuf.OrderBoundProto.OrderBound.class, com.ib.client.protobuf.OrderBoundProto.OrderBound.Builder.class);
    }

    private int bitField0_;
    public static final int PERMID_FIELD_NUMBER = 1;
    private long permId_ = 0L;
    /**
     * <code>optional int64 permId = 1;</code>
     * @return Whether the permId field is set.
     */
    @java.lang.Override
    public boolean hasPermId() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <code>optional int64 permId = 1;</code>
     * @return The permId.
     */
    @java.lang.Override
    public long getPermId() {
      return permId_;
    }

    public static final int CLIENTID_FIELD_NUMBER = 2;
    private int clientId_ = 0;
    /**
     * <code>optional int32 clientId = 2;</code>
     * @return Whether the clientId field is set.
     */
    @java.lang.Override
    public boolean hasClientId() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <code>optional int32 clientId = 2;</code>
     * @return The clientId.
     */
    @java.lang.Override
    public int getClientId() {
      return clientId_;
    }

    public static final int ORDERID_FIELD_NUMBER = 3;
    private int orderId_ = 0;
    /**
     * <code>optional int32 orderId = 3;</code>
     * @return Whether the orderId field is set.
     */
    @java.lang.Override
    public boolean hasOrderId() {
      return ((bitField0_ & 0x00000004) != 0);
    }
    /**
     * <code>optional int32 orderId = 3;</code>
     * @return The orderId.
     */
    @java.lang.Override
    public int getOrderId() {
      return orderId_;
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
        output.writeInt64(1, permId_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        output.writeInt32(2, clientId_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        output.writeInt32(3, orderId_);
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
          .computeInt64Size(1, permId_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(2, clientId_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(3, orderId_);
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
      if (!(obj instanceof com.ib.client.protobuf.OrderBoundProto.OrderBound)) {
        return super.equals(obj);
      }
      com.ib.client.protobuf.OrderBoundProto.OrderBound other = (com.ib.client.protobuf.OrderBoundProto.OrderBound) obj;

      if (hasPermId() != other.hasPermId()) return false;
      if (hasPermId()) {
        if (getPermId()
            != other.getPermId()) return false;
      }
      if (hasClientId() != other.hasClientId()) return false;
      if (hasClientId()) {
        if (getClientId()
            != other.getClientId()) return false;
      }
      if (hasOrderId() != other.hasOrderId()) return false;
      if (hasOrderId()) {
        if (getOrderId()
            != other.getOrderId()) return false;
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
      if (hasPermId()) {
        hash = (37 * hash) + PERMID_FIELD_NUMBER;
        hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
            getPermId());
      }
      if (hasClientId()) {
        hash = (37 * hash) + CLIENTID_FIELD_NUMBER;
        hash = (53 * hash) + getClientId();
      }
      if (hasOrderId()) {
        hash = (37 * hash) + ORDERID_FIELD_NUMBER;
        hash = (53 * hash) + getOrderId();
      }
      hash = (29 * hash) + getUnknownFields().hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input);
    }

    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.OrderBoundProto.OrderBound parseFrom(
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
    public static Builder newBuilder(com.ib.client.protobuf.OrderBoundProto.OrderBound prototype) {
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
     * Protobuf type {@code protobuf.OrderBound}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:protobuf.OrderBound)
        com.ib.client.protobuf.OrderBoundProto.OrderBoundOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.ib.client.protobuf.OrderBoundProto.internal_static_protobuf_OrderBound_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.ib.client.protobuf.OrderBoundProto.internal_static_protobuf_OrderBound_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.ib.client.protobuf.OrderBoundProto.OrderBound.class, com.ib.client.protobuf.OrderBoundProto.OrderBound.Builder.class);
      }

      // Construct using com.ib.client.protobuf.OrderBoundProto.OrderBound.newBuilder()
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
        permId_ = 0L;
        clientId_ = 0;
        orderId_ = 0;
        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.ib.client.protobuf.OrderBoundProto.internal_static_protobuf_OrderBound_descriptor;
      }

      @java.lang.Override
      public com.ib.client.protobuf.OrderBoundProto.OrderBound getDefaultInstanceForType() {
        return com.ib.client.protobuf.OrderBoundProto.OrderBound.getDefaultInstance();
      }

      @java.lang.Override
      public com.ib.client.protobuf.OrderBoundProto.OrderBound build() {
        com.ib.client.protobuf.OrderBoundProto.OrderBound result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public com.ib.client.protobuf.OrderBoundProto.OrderBound buildPartial() {
        com.ib.client.protobuf.OrderBoundProto.OrderBound result = new com.ib.client.protobuf.OrderBoundProto.OrderBound(this);
        if (bitField0_ != 0) { buildPartial0(result); }
        onBuilt();
        return result;
      }

      private void buildPartial0(com.ib.client.protobuf.OrderBoundProto.OrderBound result) {
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) != 0)) {
          result.permId_ = permId_;
          to_bitField0_ |= 0x00000001;
        }
        if (((from_bitField0_ & 0x00000002) != 0)) {
          result.clientId_ = clientId_;
          to_bitField0_ |= 0x00000002;
        }
        if (((from_bitField0_ & 0x00000004) != 0)) {
          result.orderId_ = orderId_;
          to_bitField0_ |= 0x00000004;
        }
        result.bitField0_ |= to_bitField0_;
      }

      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.ib.client.protobuf.OrderBoundProto.OrderBound) {
          return mergeFrom((com.ib.client.protobuf.OrderBoundProto.OrderBound)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.ib.client.protobuf.OrderBoundProto.OrderBound other) {
        if (other == com.ib.client.protobuf.OrderBoundProto.OrderBound.getDefaultInstance()) return this;
        if (other.hasPermId()) {
          setPermId(other.getPermId());
        }
        if (other.hasClientId()) {
          setClientId(other.getClientId());
        }
        if (other.hasOrderId()) {
          setOrderId(other.getOrderId());
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
                permId_ = input.readInt64();
                bitField0_ |= 0x00000001;
                break;
              } // case 8
              case 16: {
                clientId_ = input.readInt32();
                bitField0_ |= 0x00000002;
                break;
              } // case 16
              case 24: {
                orderId_ = input.readInt32();
                bitField0_ |= 0x00000004;
                break;
              } // case 24
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

      private long permId_ ;
      /**
       * <code>optional int64 permId = 1;</code>
       * @return Whether the permId field is set.
       */
      @java.lang.Override
      public boolean hasPermId() {
        return ((bitField0_ & 0x00000001) != 0);
      }
      /**
       * <code>optional int64 permId = 1;</code>
       * @return The permId.
       */
      @java.lang.Override
      public long getPermId() {
        return permId_;
      }
      /**
       * <code>optional int64 permId = 1;</code>
       * @param value The permId to set.
       * @return This builder for chaining.
       */
      public Builder setPermId(long value) {

        permId_ = value;
        bitField0_ |= 0x00000001;
        onChanged();
        return this;
      }
      /**
       * <code>optional int64 permId = 1;</code>
       * @return This builder for chaining.
       */
      public Builder clearPermId() {
        bitField0_ = (bitField0_ & ~0x00000001);
        permId_ = 0L;
        onChanged();
        return this;
      }

      private int clientId_ ;
      /**
       * <code>optional int32 clientId = 2;</code>
       * @return Whether the clientId field is set.
       */
      @java.lang.Override
      public boolean hasClientId() {
        return ((bitField0_ & 0x00000002) != 0);
      }
      /**
       * <code>optional int32 clientId = 2;</code>
       * @return The clientId.
       */
      @java.lang.Override
      public int getClientId() {
        return clientId_;
      }
      /**
       * <code>optional int32 clientId = 2;</code>
       * @param value The clientId to set.
       * @return This builder for chaining.
       */
      public Builder setClientId(int value) {

        clientId_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 clientId = 2;</code>
       * @return This builder for chaining.
       */
      public Builder clearClientId() {
        bitField0_ = (bitField0_ & ~0x00000002);
        clientId_ = 0;
        onChanged();
        return this;
      }

      private int orderId_ ;
      /**
       * <code>optional int32 orderId = 3;</code>
       * @return Whether the orderId field is set.
       */
      @java.lang.Override
      public boolean hasOrderId() {
        return ((bitField0_ & 0x00000004) != 0);
      }
      /**
       * <code>optional int32 orderId = 3;</code>
       * @return The orderId.
       */
      @java.lang.Override
      public int getOrderId() {
        return orderId_;
      }
      /**
       * <code>optional int32 orderId = 3;</code>
       * @param value The orderId to set.
       * @return This builder for chaining.
       */
      public Builder setOrderId(int value) {

        orderId_ = value;
        bitField0_ |= 0x00000004;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 orderId = 3;</code>
       * @return This builder for chaining.
       */
      public Builder clearOrderId() {
        bitField0_ = (bitField0_ & ~0x00000004);
        orderId_ = 0;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:protobuf.OrderBound)
    }

    // @@protoc_insertion_point(class_scope:protobuf.OrderBound)
    private static final com.ib.client.protobuf.OrderBoundProto.OrderBound DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new com.ib.client.protobuf.OrderBoundProto.OrderBound();
    }

    public static com.ib.client.protobuf.OrderBoundProto.OrderBound getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static final com.google.protobuf.Parser<OrderBound>
        PARSER = new com.google.protobuf.AbstractParser<OrderBound>() {
      @java.lang.Override
      public OrderBound parsePartialFrom(
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

    public static com.google.protobuf.Parser<OrderBound> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<OrderBound> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public com.ib.client.protobuf.OrderBoundProto.OrderBound getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_protobuf_OrderBound_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_protobuf_OrderBound_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\020OrderBound.proto\022\010protobuf\"r\n\nOrderBou" +
      "nd\022\023\n\006permId\030\001 \001(\003H\000\210\001\001\022\025\n\010clientId\030\002 \001(" +
      "\005H\001\210\001\001\022\024\n\007orderId\030\003 \001(\005H\002\210\001\001B\t\n\007_permIdB" +
      "\013\n\t_clientIdB\n\n\010_orderIdB:\n\026com.ib.clien" +
      "t.protobufB\017OrderBoundProto\252\002\016IBApi.prot" +
      "obufb\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_protobuf_OrderBound_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_protobuf_OrderBound_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_protobuf_OrderBound_descriptor,
        new java.lang.String[] { "PermId", "ClientId", "OrderId", });
    descriptor.resolveAllFeaturesImmutable();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
