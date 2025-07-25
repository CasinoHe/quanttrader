/* Copyright (C) 2025 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package com.ib.client;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectOutput;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

/** This class is used to build messages so the entire message can be
 *  sent to the socket in a single write. */
class Builder implements ObjectOutput {
	private static final char SEP = 0;
	private static final byte[] EMPTY_LENGTH_HEADER = new byte[ 4 ];

	private final ByteBuffer m_sb;

	public Builder( int size ) {
	    m_sb = new ByteBuffer( size );
	}

	public void send(int a) throws EClientException {
        send( String.valueOf(a) );
	}

    public void sendRawInt(int intValue) throws IOException {
        byte[] byteArray = new byte[4];
        intToBytes(intValue, byteArray, 0);
        m_sb.write(byteArray);
    }

    public void sendByteArray(byte[] byteArray) throws EClientException {
        m_sb.writeBytes(byteArray);
    }

	public void sendMax(int a) throws EClientException {
		send( a == Integer.MAX_VALUE ? "" : String.valueOf( a) );
	}

	public void send(double a) throws EClientException {
        send( String.valueOf( a) );
	}

	public void sendMax(double a) throws EClientException {
		send( a == Double.MAX_VALUE ? "" : String.valueOf( a) );
	}

	public void send(Boolean a) throws EClientException {
		sendMax(a == null ? Integer.MAX_VALUE : a ? 1 : 0);
	}

	public void send( IApiEnum a) throws EClientException {
		send( a == null ? null : a.getApiString() );
	}

	public void send( String a) throws EClientException {
		if (a != null && !isAsciiPrintable(a)) {
			throw new EClientException(EClientErrors.INVALID_SYMBOL, a);
		}
		
		if (a != null) {
		    byte[] buffer = a.getBytes(StandardCharsets.UTF_8);
		    m_sb.write( buffer, 0, buffer.length );
		}
		m_sb.write( SEP);
	}

	public void send( byte[] bytes ) {
        if ( bytes != null ) {
            m_sb.write(  bytes, 0, bytes.length );
        }
    }
	
	public void send(List<TagValue> miscOptions) throws EClientException {
        String miscOptionsString = Optional.ofNullable(miscOptions).orElse(new ArrayList<TagValue>()).stream().
                map(option -> option.m_tag + "=" + option.m_value + ";").reduce("", (sum, option) -> sum + option);

        send(miscOptionsString);
	}
	
	public void send(Contract contract) throws EClientException {
        send(contract.conid());
        send(contract.symbol());
        send(contract.getSecType());
        send(contract.lastTradeDateOrContractMonth());
        sendMax(contract.strike());
        send(contract.getRight());
        send(contract.multiplier());
        send(contract.exchange());
        send(contract.primaryExch());
        send(contract.currency());
        send(contract.localSymbol());
        send(contract.tradingClass());
        send(contract.includeExpired() ? 1 : 0);
	}

    public int allocateLengthHeader() {
        int lengthHeaderPosition = m_sb.size();
        m_sb.write( EMPTY_LENGTH_HEADER, 0, EMPTY_LENGTH_HEADER.length );
        return lengthHeaderPosition;
    }

    public void updateLength( int lengthHeaderPosition ) {
        m_sb.updateLength( lengthHeaderPosition );
    }

    public void writeTo( DataOutputStream dos ) throws IOException {
       m_sb.writeTo( dos );
    }

    // b[] must be at least b[position+4]
    static void intToBytes(int val, byte b[], int position) {
        b[position]   = (byte)(0xff & (val >> 24));
        b[position+1] = (byte)(0xff & (val >> 16));
        b[position+2] = (byte)(0xff & (val >> 8));
        b[position+3] = (byte)(0xff & val);
    }

    public static int bytesToInt(byte b[], int position) {
        int intValue = ( ((b[position    ] & 0xff) << 24)
                       | ((b[position + 1] & 0xff) << 16)
                       | ((b[position + 2] & 0xff) <<  8)
                       |  (b[position + 3] & 0xff) );
        return intValue;
    }
    
    static boolean isAsciiPrintable(String str) {
        if (str == null) {
            return false;
        }
        for (int i = 0; i < str.length(); i++) {
            if (isAsciiPrintable(str.charAt(i)) == false) {
                return false;
            }
        }
        return true;
    }

    private static boolean isAsciiPrintable(char ch) {
        return ch >= 32 && ch < 127 || ch == 9 || ch == 10 || ch == 13;
    }

    /** inner class: ByteBuffer - storage for bytes and direct access to buffer. */
    private static class ByteBuffer extends ByteArrayOutputStream {
        private final int paddingSize; // 1 disables padding, 4 is normal if padding is used

        ByteBuffer(int capacity) {
            super( capacity );
            paddingSize = 1;
        }

        void updateLength(int lengthHeaderPosition) {
            int len = this.count - EMPTY_LENGTH_HEADER.length - lengthHeaderPosition;
            if ( paddingSize > 1 ) {
                int padding = paddingSize - len%paddingSize;
                if ( padding < paddingSize ) {
                    this.write( EMPTY_LENGTH_HEADER, 0, paddingSize ); // extra padding at the end
                    len = this.count - EMPTY_LENGTH_HEADER.length - lengthHeaderPosition;
                }
            }
            intToBytes(len, this.buf, lengthHeaderPosition);
        }

        void writeTo(DataOutputStream out) throws IOException {
            out.write( this.buf, 0, this.count );
        }
    }

	@Override
	public void writeBoolean(boolean arg0) throws IOException { send(arg0); }
	@Override
	public void writeByte(int arg0) throws IOException { send(arg0); }
	@Override
	public void writeBytes(String arg0) throws IOException { send(arg0); }
	@Override
	public void writeChar(int arg0) throws IOException { send(arg0); }
	@Override
	public void writeChars(String arg0) throws IOException { send(arg0); }
	@Override
	public void writeDouble(double arg0) throws IOException { send(arg0); }
	@Override
	public void writeFloat(float arg0) throws IOException { send(arg0); }
	@Override
	public void writeInt(int arg0) throws IOException { send(arg0); }
	@Override
	public void writeLong(long arg0) throws IOException { send(arg0); }
	@Override
	public void writeShort(int arg0) throws IOException { send(arg0); }
	@Override
	public void writeUTF(String arg0) throws IOException { send(arg0); }
	@Override
	public void close() throws IOException {
        m_sb.close();
	}
	@Override
	public void flush() throws IOException { }
	@Override
	public void write(int arg0) throws IOException { }
	@Override
	public void write(byte[] arg0) throws IOException { }
	@Override
	public void write(byte[] arg0, int arg1, int arg2)
			throws IOException { }
	@Override
	public void writeObject(Object arg0) throws IOException { }
}
