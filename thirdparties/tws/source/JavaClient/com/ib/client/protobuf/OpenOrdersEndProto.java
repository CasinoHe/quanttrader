// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: OpenOrdersEnd.proto
// Protobuf Java Version: 4.29.3

package com.ib.client.protobuf;

public final class OpenOrdersEndProto {
  private OpenOrdersEndProto() {}
  static {
    com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
      com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
      /* major= */ 4,
      /* minor= */ 29,
      /* patch= */ 3,
      /* suffix= */ "",
      OpenOrdersEndProto.class.getName());
  }
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface OpenOrdersEndOrBuilder extends
      // @@protoc_insertion_point(interface_extends:protobuf.OpenOrdersEnd)
      com.google.protobuf.MessageOrBuilder {
  }
  /**
   * <pre>
   * empty
   * </pre>
   *
   * Protobuf type {@code protobuf.OpenOrdersEnd}
   */
  public static final class OpenOrdersEnd extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:protobuf.OpenOrdersEnd)
      OpenOrdersEndOrBuilder {
  private static final long serialVersionUID = 0L;
    static {
      com.google.protobuf.RuntimeVersion.validateProtobufGencodeVersion(
        com.google.protobuf.RuntimeVersion.RuntimeDomain.PUBLIC,
        /* major= */ 4,
        /* minor= */ 29,
        /* patch= */ 3,
        /* suffix= */ "",
        OpenOrdersEnd.class.getName());
    }
    // Use OpenOrdersEnd.newBuilder() to construct.
    private OpenOrdersEnd(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
    }
    private OpenOrdersEnd() {
    }

    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.ib.client.protobuf.OpenOrdersEndProto.internal_static_protobuf_OpenOrdersEnd_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.ib.client.protobuf.OpenOrdersEndProto.internal_static_protobuf_OpenOrdersEnd_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.class, com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.Builder.class);
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
      getUnknownFields().writeTo(output);
    }

    @java.lang.Override
    public int getSerializedSize() {
      int size = memoizedSize;
      if (size != -1) return size;

      size = 0;
      size += getUnknownFields().getSerializedSize();
      memoizedSize = size;
      return size;
    }

    @java.lang.Override
    public boolean equals(final java.lang.Object obj) {
      if (obj == this) {
       return true;
      }
      if (!(obj instanceof com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd)) {
        return super.equals(obj);
      }
      com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd other = (com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd) obj;

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
      hash = (29 * hash) + getUnknownFields().hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input);
    }

    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessage
          .parseWithIOException(PARSER, input);
    }
    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd parseFrom(
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
    public static Builder newBuilder(com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd prototype) {
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
     * <pre>
     * empty
     * </pre>
     *
     * Protobuf type {@code protobuf.OpenOrdersEnd}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:protobuf.OpenOrdersEnd)
        com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEndOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.ib.client.protobuf.OpenOrdersEndProto.internal_static_protobuf_OpenOrdersEnd_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.ib.client.protobuf.OpenOrdersEndProto.internal_static_protobuf_OpenOrdersEnd_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.class, com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.Builder.class);
      }

      // Construct using com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.newBuilder()
      private Builder() {

      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);

      }
      @java.lang.Override
      public Builder clear() {
        super.clear();
        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.ib.client.protobuf.OpenOrdersEndProto.internal_static_protobuf_OpenOrdersEnd_descriptor;
      }

      @java.lang.Override
      public com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd getDefaultInstanceForType() {
        return com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.getDefaultInstance();
      }

      @java.lang.Override
      public com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd build() {
        com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd buildPartial() {
        com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd result = new com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd(this);
        onBuilt();
        return result;
      }

      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd) {
          return mergeFrom((com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd other) {
        if (other == com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd.getDefaultInstance()) return this;
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

      // @@protoc_insertion_point(builder_scope:protobuf.OpenOrdersEnd)
    }

    // @@protoc_insertion_point(class_scope:protobuf.OpenOrdersEnd)
    private static final com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd();
    }

    public static com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static final com.google.protobuf.Parser<OpenOrdersEnd>
        PARSER = new com.google.protobuf.AbstractParser<OpenOrdersEnd>() {
      @java.lang.Override
      public OpenOrdersEnd parsePartialFrom(
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

    public static com.google.protobuf.Parser<OpenOrdersEnd> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<OpenOrdersEnd> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public com.ib.client.protobuf.OpenOrdersEndProto.OpenOrdersEnd getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_protobuf_OpenOrdersEnd_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_protobuf_OpenOrdersEnd_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\023OpenOrdersEnd.proto\022\010protobuf\"\017\n\rOpenO" +
      "rdersEndB=\n\026com.ib.client.protobufB\022Open" +
      "OrdersEndProto\252\002\016IBApi.protobufb\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_protobuf_OpenOrdersEnd_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_protobuf_OpenOrdersEnd_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_protobuf_OpenOrdersEnd_descriptor,
        new java.lang.String[] { });
    descriptor.resolveAllFeaturesImmutable();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
