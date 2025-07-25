// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: IneligibilityReason.proto
// Protobuf Java Version: 4.29.3

package com.ib.client.protobuf;

public final class IneligibilityReasonProto {
  private IneligibilityReasonProto() {}
  static {
    com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
      com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
      /* major= */ 4,
      /* minor= */ 29,
      /* patch= */ 3,
      /* suffix= */ "",
      IneligibilityReasonProto.class.getName());
  }
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface IneligibilityReasonOrBuilder extends
      // @@protoc_insertion_point(interface_extends:protobuf.IneligibilityReason)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>optional string id = 1;</code>
     * @return Whether the id field is set.
     */
    boolean hasId();
    /**
     * <code>optional string id = 1;</code>
     * @return The id.
     */
    java.lang.String getId();
    /**
     * <code>optional string id = 1;</code>
     * @return The bytes for id.
     */
    com.google.protobuf.ByteString
        getIdBytes();

    /**
     * <code>optional string description = 2;</code>
     * @return Whether the description field is set.
     */
    boolean hasDescription();
    /**
     * <code>optional string description = 2;</code>
     * @return The description.
     */
    java.lang.String getDescription();
    /**
     * <code>optional string description = 2;</code>
     * @return The bytes for description.
     */
    com.google.protobuf.ByteString
        getDescriptionBytes();
  }
  /**
   * Protobuf type {@code protobuf.IneligibilityReason}
   */
  public static final class IneligibilityReason extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:protobuf.IneligibilityReason)
      IneligibilityReasonOrBuilder {
  private static final long serialVersionUID = 0L;
    static {
      com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
        com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
        /* major= */ 4,
        /* minor= */ 29,
        /* patch= */ 3,
        /* suffix= */ "",
        IneligibilityReason.class.getName());
    }
    // Use IneligibilityReason.newBuilder() to construct.
    private IneligibilityReason(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
    }
    private IneligibilityReason() {
      id_ = "";
      description_ = "";
    }

    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.ib.client.protobuf.IneligibilityReasonProto.internal_static_protobuf_IneligibilityReason_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.ib.client.protobuf.IneligibilityReasonProto.internal_static_protobuf_IneligibilityReason_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.class, com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.Builder.class);
    }

    private int bitField0_;
    public static final int ID_FIELD_NUMBER = 1;
    @SuppressWarnings("serial")
    private volatile java.lang.Object id_ = "";
    /**
     * <code>optional string id = 1;</code>
     * @return Whether the id field is set.
     */
    @java.lang.Override
    public boolean hasId() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <code>optional string id = 1;</code>
     * @return The id.
     */
    @java.lang.Override
    public java.lang.String getId() {
      java.lang.Object ref = id_;
      if (ref instanceof java.lang.String) {
        return (java.lang.String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        id_ = s;
        return s;
      }
    }
    /**
     * <code>optional string id = 1;</code>
     * @return The bytes for id.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString
        getIdBytes() {
      java.lang.Object ref = id_;
      if (ref instanceof java.lang.String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        id_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }

    public static final int DESCRIPTION_FIELD_NUMBER = 2;
    @SuppressWarnings("serial")
    private volatile java.lang.Object description_ = "";
    /**
     * <code>optional string description = 2;</code>
     * @return Whether the description field is set.
     */
    @java.lang.Override
    public boolean hasDescription() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <code>optional string description = 2;</code>
     * @return The description.
     */
    @java.lang.Override
    public java.lang.String getDescription() {
      java.lang.Object ref = description_;
      if (ref instanceof java.lang.String) {
        return (java.lang.String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        description_ = s;
        return s;
      }
    }
    /**
     * <code>optional string description = 2;</code>
     * @return The bytes for description.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString
        getDescriptionBytes() {
      java.lang.Object ref = description_;
      if (ref instanceof java.lang.String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        description_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
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
        com.google.protobuf.GeneratedMessage.writeString(output, 1, id_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        com.google.protobuf.GeneratedMessage.writeString(output, 2, description_);
      }
      getUnknownFields().writeTo(output);
    }

    @java.lang.Override
    public int getSerializedSize() {
      int size = memoizedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) != 0)) {
        size += com.google.protobuf.GeneratedMessage.computeStringSize(1, id_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        size += com.google.protobuf.GeneratedMessage.computeStringSize(2, description_);
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
      if (!(obj instanceof com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason)) {
        return super.equals(obj);
      }
      com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason other = (com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason) obj;

      if (hasId() != other.hasId()) return false;
      if (hasId()) {
        if (!getId()
            .equals(other.getId())) return false;
      }
      if (hasDescription() != other.hasDescription()) return false;
      if (hasDescription()) {
        if (!getDescription()
            .equals(other.getDescription())) return false;
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
      if (hasId()) {
        hash = (37 * hash) + ID_FIELD_NUMBER;
        hash = (53 * hash) + getId().hashCode();
      }
      if (hasDescription()) {
        hash = (37 * hash) + DESCRIPTION_FIELD_NUMBER;
        hash = (53 * hash) + getDescription().hashCode();
      }
      hash = (29 * hash) + getUnknownFields().hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input);
    }

    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason parseFrom(
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
    public static Builder newBuilder(com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason prototype) {
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
     * Protobuf type {@code protobuf.IneligibilityReason}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:protobuf.IneligibilityReason)
        com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReasonOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.ib.client.protobuf.IneligibilityReasonProto.internal_static_protobuf_IneligibilityReason_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.ib.client.protobuf.IneligibilityReasonProto.internal_static_protobuf_IneligibilityReason_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.class, com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.Builder.class);
      }

      // Construct using com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.newBuilder()
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
        id_ = "";
        description_ = "";
        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.ib.client.protobuf.IneligibilityReasonProto.internal_static_protobuf_IneligibilityReason_descriptor;
      }

      @java.lang.Override
      public com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason getDefaultInstanceForType() {
        return com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.getDefaultInstance();
      }

      @java.lang.Override
      public com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason build() {
        com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason buildPartial() {
        com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason result = new com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason(this);
        if (bitField0_ != 0) { buildPartial0(result); }
        onBuilt();
        return result;
      }

      private void buildPartial0(com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason result) {
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) != 0)) {
          result.id_ = id_;
          to_bitField0_ |= 0x00000001;
        }
        if (((from_bitField0_ & 0x00000002) != 0)) {
          result.description_ = description_;
          to_bitField0_ |= 0x00000002;
        }
        result.bitField0_ |= to_bitField0_;
      }

      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason) {
          return mergeFrom((com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason other) {
        if (other == com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason.getDefaultInstance()) return this;
        if (other.hasId()) {
          id_ = other.id_;
          bitField0_ |= 0x00000001;
          onChanged();
        }
        if (other.hasDescription()) {
          description_ = other.description_;
          bitField0_ |= 0x00000002;
          onChanged();
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
              case 10: {
                id_ = input.readStringRequireUtf8();
                bitField0_ |= 0x00000001;
                break;
              } // case 10
              case 18: {
                description_ = input.readStringRequireUtf8();
                bitField0_ |= 0x00000002;
                break;
              } // case 18
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

      private java.lang.Object id_ = "";
      /**
       * <code>optional string id = 1;</code>
       * @return Whether the id field is set.
       */
      public boolean hasId() {
        return ((bitField0_ & 0x00000001) != 0);
      }
      /**
       * <code>optional string id = 1;</code>
       * @return The id.
       */
      public java.lang.String getId() {
        java.lang.Object ref = id_;
        if (!(ref instanceof java.lang.String)) {
          com.google.protobuf.ByteString bs =
              (com.google.protobuf.ByteString) ref;
          java.lang.String s = bs.toStringUtf8();
          id_ = s;
          return s;
        } else {
          return (java.lang.String) ref;
        }
      }
      /**
       * <code>optional string id = 1;</code>
       * @return The bytes for id.
       */
      public com.google.protobuf.ByteString
          getIdBytes() {
        java.lang.Object ref = id_;
        if (ref instanceof String) {
          com.google.protobuf.ByteString b = 
              com.google.protobuf.ByteString.copyFromUtf8(
                  (java.lang.String) ref);
          id_ = b;
          return b;
        } else {
          return (com.google.protobuf.ByteString) ref;
        }
      }
      /**
       * <code>optional string id = 1;</code>
       * @param value The id to set.
       * @return This builder for chaining.
       */
      public Builder setId(
          java.lang.String value) {
        if (value == null) { throw new NullPointerException(); }
        id_ = value;
        bitField0_ |= 0x00000001;
        onChanged();
        return this;
      }
      /**
       * <code>optional string id = 1;</code>
       * @return This builder for chaining.
       */
      public Builder clearId() {
        id_ = getDefaultInstance().getId();
        bitField0_ = (bitField0_ & ~0x00000001);
        onChanged();
        return this;
      }
      /**
       * <code>optional string id = 1;</code>
       * @param value The bytes for id to set.
       * @return This builder for chaining.
       */
      public Builder setIdBytes(
          com.google.protobuf.ByteString value) {
        if (value == null) { throw new NullPointerException(); }
        checkByteStringIsUtf8(value);
        id_ = value;
        bitField0_ |= 0x00000001;
        onChanged();
        return this;
      }

      private java.lang.Object description_ = "";
      /**
       * <code>optional string description = 2;</code>
       * @return Whether the description field is set.
       */
      public boolean hasDescription() {
        return ((bitField0_ & 0x00000002) != 0);
      }
      /**
       * <code>optional string description = 2;</code>
       * @return The description.
       */
      public java.lang.String getDescription() {
        java.lang.Object ref = description_;
        if (!(ref instanceof java.lang.String)) {
          com.google.protobuf.ByteString bs =
              (com.google.protobuf.ByteString) ref;
          java.lang.String s = bs.toStringUtf8();
          description_ = s;
          return s;
        } else {
          return (java.lang.String) ref;
        }
      }
      /**
       * <code>optional string description = 2;</code>
       * @return The bytes for description.
       */
      public com.google.protobuf.ByteString
          getDescriptionBytes() {
        java.lang.Object ref = description_;
        if (ref instanceof String) {
          com.google.protobuf.ByteString b = 
              com.google.protobuf.ByteString.copyFromUtf8(
                  (java.lang.String) ref);
          description_ = b;
          return b;
        } else {
          return (com.google.protobuf.ByteString) ref;
        }
      }
      /**
       * <code>optional string description = 2;</code>
       * @param value The description to set.
       * @return This builder for chaining.
       */
      public Builder setDescription(
          java.lang.String value) {
        if (value == null) { throw new NullPointerException(); }
        description_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }
      /**
       * <code>optional string description = 2;</code>
       * @return This builder for chaining.
       */
      public Builder clearDescription() {
        description_ = getDefaultInstance().getDescription();
        bitField0_ = (bitField0_ & ~0x00000002);
        onChanged();
        return this;
      }
      /**
       * <code>optional string description = 2;</code>
       * @param value The bytes for description to set.
       * @return This builder for chaining.
       */
      public Builder setDescriptionBytes(
          com.google.protobuf.ByteString value) {
        if (value == null) { throw new NullPointerException(); }
        checkByteStringIsUtf8(value);
        description_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:protobuf.IneligibilityReason)
    }

    // @@protoc_insertion_point(class_scope:protobuf.IneligibilityReason)
    private static final com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason();
    }

    public static com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static final com.google.protobuf.Parser<IneligibilityReason>
        PARSER = new com.google.protobuf.AbstractParser<IneligibilityReason>() {
      @java.lang.Override
      public IneligibilityReason parsePartialFrom(
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

    public static com.google.protobuf.Parser<IneligibilityReason> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<IneligibilityReason> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public com.ib.client.protobuf.IneligibilityReasonProto.IneligibilityReason getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_protobuf_IneligibilityReason_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_protobuf_IneligibilityReason_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\031IneligibilityReason.proto\022\010protobuf\"W\n" +
      "\023IneligibilityReason\022\017\n\002id\030\001 \001(\tH\000\210\001\001\022\030\n" +
      "\013description\030\002 \001(\tH\001\210\001\001B\005\n\003_idB\016\n\014_descr" +
      "iptionBC\n\026com.ib.client.protobufB\030Inelig" +
      "ibilityReasonProto\252\002\016IBApi.protobufb\006pro" +
      "to3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_protobuf_IneligibilityReason_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_protobuf_IneligibilityReason_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_protobuf_IneligibilityReason_descriptor,
        new java.lang.String[] { "Id", "Description", });
    descriptor.resolveAllFeaturesImmutable();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
