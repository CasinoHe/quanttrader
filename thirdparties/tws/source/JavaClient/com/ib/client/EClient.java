/* Copyright (C) 2025 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package com.ib.client;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.ib.client.Types.SecType;
import com.ib.client.Types.WhatToShow;
import com.ib.client.protobuf.AllOpenOrdersRequestProto;
import com.ib.client.protobuf.AutoOpenOrdersRequestProto;
import com.ib.client.protobuf.CancelMarketDataProto;
import com.ib.client.protobuf.CancelMarketDepthProto;
import com.ib.client.protobuf.CancelOrderRequestProto;
import com.ib.client.protobuf.CompletedOrdersRequestProto;
import com.ib.client.protobuf.ContractDataRequestProto;
import com.ib.client.protobuf.ExecutionRequestProto;
import com.ib.client.protobuf.GlobalCancelRequestProto;
import com.ib.client.protobuf.MarketDataRequestProto;
import com.ib.client.protobuf.MarketDataTypeRequestProto;
import com.ib.client.protobuf.MarketDepthRequestProto;
import com.ib.client.protobuf.OpenOrdersRequestProto;
import com.ib.client.protobuf.PlaceOrderRequestProto;

public abstract class EClient {

    // Client version history
    //
    // 	6 = Added parentId to orderStatus
    // 	7 = The new execDetails event returned for an order filled status and reqExecDetails
    //     Also market depth is available.
    // 	8 = Added lastFillPrice to orderStatus() event and permId to execution details
    //  9 = Added 'averageCost', 'unrealizedPNL', and 'unrealizedPNL' to updatePortfolio event
    // 10 = Added 'serverId' to the 'open order' & 'order status' events.
    //      We send back all the API open orders upon connection.
    //      Added new methods reqAllOpenOrders, reqAutoOpenOrders()
    //      Added FA support - reqExecution has filter.
    //                       - reqAccountUpdates takes acct code.
    // 11 = Added permId to openOrder event.
    // 12 = requesting open order attributes ignoreRth, hidden, and discretionary
    // 13 = added goodAfterTime
    // 14 = always send size on bid/ask/last tick
    // 15 = send allocation description string on openOrder
    // 16 = can receive account name in account and portfolio updates, and fa params in openOrder
    // 17 = can receive liquidation field in exec reports, and notAutoAvailable field in mkt data
    // 18 = can receive good till date field in open order messages, and request intraday backfill
    // 19 = can receive rthOnly flag in ORDER_STATUS
    // 20 = expects TWS time string on connection after server version >= 20.
    // 21 = can receive bond contract details.
    // 22 = can receive price magnifier in version 2 contract details message
    // 23 = support for scanner
    // 24 = can receive volatility order parameters in open order messages
	// 25 = can receive HMDS query start and end times
	// 26 = can receive option vols in option market data messages
	// 27 = can receive delta neutral order type and delta neutral aux price in place order version 20: API 8.85
	// 28 = can receive option model computation ticks: API 8.9
	// 29 = can receive trail stop limit price in open order and can place them: API 8.91
	// 30 = can receive extended bond contract def, new ticks, and trade count in bars
	// 31 = can receive EFP extensions to scanner and market data, and combo legs on open orders
	//    ; can receive RT bars
	// 32 = can receive TickType.LAST_TIMESTAMP
	//    ; can receive "whyHeld" in order status messages
	// 33 = can receive ScaleNumComponents and ScaleComponentSize is open order messages
	// 34 = can receive whatIf orders / order state
	// 35 = can receive contId field for Contract objects
	// 36 = can receive outsideRth field for Order objects
	// 37 = can receive clearingAccount and clearingIntent for Order objects
	// 38 = can receive multiplier and primaryExchange in portfolio updates
	//    ; can receive cumQty and avgPrice in execution
	//    ; can receive fundamental data
	//    ; can receive deltaNeutralContract for Contract objects
	//    ; can receive reqId and end marker in contractDetails/bondContractDetails
 	//    ; can receive ScaleInitComponentSize and ScaleSubsComponentSize for Order objects
	// 39 = can receive underConId in contractDetails
	// 40 = can receive algoStrategy/algoParams in openOrder
	// 41 = can receive end marker for openOrder
	//    ; can receive end marker for account download
	//    ; can receive end marker for executions download
	// 42 = can receive deltaNeutralValidation
	// 43 = can receive longName(companyName)
	//    ; can receive listingExchange
	//    ; can receive RTVolume tick
	// 44 = can receive end market for ticker snapshot
	// 45 = can receive notHeld field in openOrder
	// 46 = can receive contractMonth, industry, category, subcategory fields in contractDetails
	//    ; can receive timeZoneId, tradingHours, liquidHours fields in contractDetails
	// 47 = can receive gamma, vega, theta, undPrice fields in TICK_OPTION_COMPUTATION
	// 48 = can receive exemptCode in openOrder
	// 49 = can receive hedgeType and hedgeParam in openOrder
	// 50 = can receive optOutSmartRouting field in openOrder
	// 51 = can receive smartComboRoutingParams in openOrder
	// 52 = can receive deltaNeutralConId, deltaNeutralSettlingFirm, deltaNeutralClearingAccount and deltaNeutralClearingIntent in openOrder
	// 53 = can receive orderRef in execution
	// 54 = can receive scale order fields (PriceAdjustValue, PriceAdjustInterval, ProfitOffset, AutoReset,
	//      InitPosition, InitFillQty and RandomPercent) in openOrder
	// 55 = can receive orderComboLegs (price) in openOrder
	// 56 = can receive trailingPercent in openOrder
	// 57 = can receive commissionAndFeesReport message
	// 58 = can receive CUSIP/ISIN/etc. in contractDescription/bondContractDescription
	// 59 = can receive evRule, evMultiplier in contractDescription/bondContractDescription/executionDetails
	//      can receive multiplier in executionDetails
	// 60 = can receive deltaNeutralOpenClose, deltaNeutralShortSale, deltaNeutralShortSaleSlot and deltaNeutralDesignatedLocation in openOrder
	// 61 = can receive multiplier in openOrder
	//      can receive tradingClass in openOrder, updatePortfolio, execDetails and position
	// 62 = can receive avgCost in position message
	// 63 = can receive verifyMessageAPI, verifyCompleted, displayGroupList and displayGroupUpdated messages
	// 64 = can receive solicited attrib in openOrder message
	// 65 = can receive verifyAndAuthMessageAPI and verifyAndAuthCompleted messages
	// 66 = can receive randomize size and randomize price order fields

    protected static final int CLIENT_VERSION = 66;
    protected static final int MIN_SERVER_VER_SUPPORTED = 38; //all supported server versions are listed below
    
    // outgoing msg id's
    private static final int REQ_MKT_DATA = 1;
    private static final int CANCEL_MKT_DATA = 2;
    protected static final int PLACE_ORDER = 3;
    private static final int CANCEL_ORDER = 4;
    private static final int REQ_OPEN_ORDERS = 5;
    private static final int REQ_ACCOUNT_DATA = 6;
    private static final int REQ_EXECUTIONS = 7;
    private static final int REQ_IDS = 8;
    private static final int REQ_CONTRACT_DATA = 9;
    private static final int REQ_MKT_DEPTH = 10;
    private static final int CANCEL_MKT_DEPTH = 11;
    private static final int REQ_NEWS_BULLETINS = 12;
    private static final int CANCEL_NEWS_BULLETINS = 13;
    private static final int SET_SERVER_LOGLEVEL = 14;
    private static final int REQ_AUTO_OPEN_ORDERS = 15;
    private static final int REQ_ALL_OPEN_ORDERS = 16;
    private static final int REQ_MANAGED_ACCTS = 17;
    private static final int REQ_FA = 18;
    private static final int REPLACE_FA = 19;
    private static final int REQ_HISTORICAL_DATA = 20;
    private static final int EXERCISE_OPTIONS = 21;
    private static final int REQ_SCANNER_SUBSCRIPTION = 22;
    private static final int CANCEL_SCANNER_SUBSCRIPTION = 23;
    private static final int REQ_SCANNER_PARAMETERS = 24;
    private static final int CANCEL_HISTORICAL_DATA = 25;
    private static final int REQ_CURRENT_TIME = 49;
    private static final int REQ_REAL_TIME_BARS = 50;
    private static final int CANCEL_REAL_TIME_BARS = 51;
    private static final int REQ_FUNDAMENTAL_DATA = 52;
    private static final int CANCEL_FUNDAMENTAL_DATA = 53;
    private static final int REQ_CALC_IMPLIED_VOLAT = 54;
    private static final int REQ_CALC_OPTION_PRICE = 55;
    private static final int CANCEL_CALC_IMPLIED_VOLAT = 56;
    private static final int CANCEL_CALC_OPTION_PRICE = 57;
    private static final int REQ_GLOBAL_CANCEL = 58;
    private static final int REQ_MARKET_DATA_TYPE = 59;
    private static final int REQ_POSITIONS = 61;
    private static final int REQ_ACCOUNT_SUMMARY = 62;
    private static final int CANCEL_ACCOUNT_SUMMARY = 63;
    private static final int CANCEL_POSITIONS = 64;
    private static final int VERIFY_REQUEST = 65;
    private static final int VERIFY_MESSAGE = 66;
    private static final int QUERY_DISPLAY_GROUPS = 67;
    private static final int SUBSCRIBE_TO_GROUP_EVENTS = 68;
    private static final int UPDATE_DISPLAY_GROUP = 69;
    private static final int UNSUBSCRIBE_FROM_GROUP_EVENTS = 70;
    private static final int START_API = 71;
    private static final int VERIFY_AND_AUTH_REQUEST = 72;
    private static final int VERIFY_AND_AUTH_MESSAGE = 73;
    private static final int REQ_POSITIONS_MULTI = 74;
    private static final int CANCEL_POSITIONS_MULTI = 75;
    private static final int REQ_ACCOUNT_UPDATES_MULTI = 76;
    private static final int CANCEL_ACCOUNT_UPDATES_MULTI = 77;
    private static final int REQ_SEC_DEF_OPT_PARAMS     = 78;
    private static final int REQ_SOFT_DOLLAR_TIERS     = 79;
    private static final int REQ_FAMILY_CODES = 80;
    private static final int REQ_MATCHING_SYMBOLS = 81;
    private static final int REQ_MKT_DEPTH_EXCHANGES = 82;
    private static final int REQ_SMART_COMPONENTS = 83;
    private static final int REQ_NEWS_ARTICLE = 84;
    private static final int REQ_NEWS_PROVIDERS = 85;
    private static final int REQ_HISTORICAL_NEWS = 86;
  	private static final int REQ_HEAD_TIMESTAMP = 87;
  	private static final int REQ_HISTOGRAM_DATA = 88;
    private static final int CANCEL_HISTOGRAM_DATA = 89;
    private static final int CANCEL_HEAD_TIMESTAMP = 90;
    private static final int REQ_MARKET_RULE = 91;
    private static final int REQ_PNL = 92;
    private static final int CANCEL_PNL = 93;
    private static final int REQ_PNL_SINGLE = 94;
    private static final int CANCEL_PNL_SINGLE = 95;
    private static final int REQ_HISTORICAL_TICKS = 96;
    private static final int REQ_TICK_BY_TICK_DATA = 97;
    private static final int CANCEL_TICK_BY_TICK_DATA = 98;
    private static final int REQ_COMPLETED_ORDERS = 99;
    private static final int REQ_WSH_META_DATA = 100;
    private static final int CANCEL_WSH_META_DATA = 101;
    private static final int REQ_WSH_EVENT_DATA = 102;
    private static final int CANCEL_WSH_EVENT_DATA = 103;
    private static final int REQ_USER_INFO = 104;
    private static final int REQ_CURRENT_TIME_IN_MILLIS = 105;

	private static final int MIN_SERVER_VER_REAL_TIME_BARS = 34;
	private static final int MIN_SERVER_VER_SCALE_ORDERS = 35;
	private static final int MIN_SERVER_VER_SNAPSHOT_MKT_DATA = 35;
	private static final int MIN_SERVER_VER_SSHORT_COMBO_LEGS = 35;
	private static final int MIN_SERVER_VER_WHAT_IF_ORDERS = 36;
	private static final int MIN_SERVER_VER_CONTRACT_CONID = 37;
	private static final int MIN_SERVER_VER_PTA_ORDERS = 39;
	private static final int MIN_SERVER_VER_FUNDAMENTAL_DATA = 40;
	private static final int MIN_SERVER_VER_DELTA_NEUTRAL = 40;
	private static final int MIN_SERVER_VER_CONTRACT_DATA_CHAIN = 40;
	private static final int MIN_SERVER_VER_SCALE_ORDERS2 = 40;
	private static final int MIN_SERVER_VER_ALGO_ORDERS = 41;
	private static final int MIN_SERVER_VER_EXECUTION_DATA_CHAIN = 42;
	private static final int MIN_SERVER_VER_NOT_HELD = 44;
	private static final int MIN_SERVER_VER_SEC_ID_TYPE = 45;
	private static final int MIN_SERVER_VER_PLACE_ORDER_CONID = 46;
	private static final int MIN_SERVER_VER_REQ_MKT_DATA_CONID = 47;
    private static final int MIN_SERVER_VER_REQ_CALC_IMPLIED_VOLAT = 49;
    private static final int MIN_SERVER_VER_REQ_CALC_OPTION_PRICE = 50;
    private static final int MIN_SERVER_VER_CANCEL_CALC_IMPLIED_VOLAT = 50;
    private static final int MIN_SERVER_VER_CANCEL_CALC_OPTION_PRICE = 50;
    private static final int MIN_SERVER_VER_SSHORTX_OLD = 51;
    private static final int MIN_SERVER_VER_SSHORTX = 52;
    private static final int MIN_SERVER_VER_REQ_GLOBAL_CANCEL = 53;
    private static final int MIN_SERVER_VER_HEDGE_ORDERS = 54;
    private static final int MIN_SERVER_VER_REQ_MARKET_DATA_TYPE = 55;
    private static final int MIN_SERVER_VER_OPT_OUT_SMART_ROUTING = 56;
    private static final int MIN_SERVER_VER_SMART_COMBO_ROUTING_PARAMS = 57;
    private static final int MIN_SERVER_VER_DELTA_NEUTRAL_CONID = 58;
    private static final int MIN_SERVER_VER_SCALE_ORDERS3 = 60;
    private static final int MIN_SERVER_VER_ORDER_COMBO_LEGS_PRICE = 61;
    private static final int MIN_SERVER_VER_TRAILING_PERCENT = 62;
    protected static final int MIN_SERVER_VER_DELTA_NEUTRAL_OPEN_CLOSE = 66;
    private static final int MIN_SERVER_VER_ACCT_SUMMARY = 67;
    protected static final int MIN_SERVER_VER_TRADING_CLASS = 68;
    protected static final int MIN_SERVER_VER_SCALE_TABLE = 69;
    protected static final int MIN_SERVER_VER_LINKING = 70;
    protected static final int MIN_SERVER_VER_ALGO_ID = 71;
    protected static final int MIN_SERVER_VER_OPTIONAL_CAPABILITIES = 72;
    protected static final int MIN_SERVER_VER_ORDER_SOLICITED = 73;
    protected static final int MIN_SERVER_VER_LINKING_AUTH = 74;
    protected static final int MIN_SERVER_VER_PRIMARYEXCH = 75;
    protected static final int MIN_SERVER_VER_RANDOMIZE_SIZE_AND_PRICE = 76;
    protected static final int MIN_SERVER_VER_FRACTIONAL_POSITIONS = 101;
    protected static final int MIN_SERVER_VER_PEGGED_TO_BENCHMARK = 102;
    protected static final int MIN_SERVER_VER_MODELS_SUPPORT = 103;
    protected static final int MIN_SERVER_VER_SEC_DEF_OPT_PARAMS_REQ = 104;
    protected static final int MIN_SERVER_VER_EXT_OPERATOR = 105;
    protected static final int MIN_SERVER_VER_SOFT_DOLLAR_TIER = 106;
    protected static final int MIN_SERVER_VER_REQ_FAMILY_CODES = 107;
    protected static final int MIN_SERVER_VER_REQ_MATCHING_SYMBOLS = 108;
    protected static final int MIN_SERVER_VER_PAST_LIMIT = 109;
    protected static final int MIN_SERVER_VER_MD_SIZE_MULTIPLIER = 110;
    protected static final int MIN_SERVER_VER_CASH_QTY = 111;
    protected static final int MIN_SERVER_VER_REQ_MKT_DEPTH_EXCHANGES = 112;
    protected static final int MIN_SERVER_VER_TICK_NEWS = 113;
    protected static final int MIN_SERVER_VER_REQ_SMART_COMPONENTS = 114;
    protected static final int MIN_SERVER_VER_REQ_NEWS_PROVIDERS = 115;
    protected static final int MIN_SERVER_VER_REQ_NEWS_ARTICLE = 116;
    protected static final int MIN_SERVER_VER_REQ_HISTORICAL_NEWS = 117;
    protected static final int MIN_SERVER_VER_REQ_HEAD_TIMESTAMP = 118;
    protected static final int MIN_SERVER_VER_REQ_HISTOGRAM = 119;
    protected static final int MIN_SERVER_VER_SERVICE_DATA_TYPE = 120;
    protected static final int MIN_SERVER_VER_AGG_GROUP = 121;
    protected static final int MIN_SERVER_VER_UNDERLYING_INFO = 122;
    protected static final int MIN_SERVER_VER_CANCEL_HEADTIMESTAMP = 123;
    protected static final int MIN_SERVER_VER_SYNT_REALTIME_BARS = 124;
    protected static final int MIN_SERVER_VER_CFD_REROUTE = 125;
    protected static final int MIN_SERVER_VER_MARKET_RULES = 126;
    protected static final int MIN_SERVER_VER_PNL = 127;
    protected static final int MIN_SERVER_VER_NEWS_QUERY_ORIGINS = 128;
    protected static final int MIN_SERVER_VER_UNREALIZED_PNL = 129;
    protected static final int MIN_SERVER_VER_HISTORICAL_TICKS = 130;
    protected static final int MIN_SERVER_VER_MARKET_CAP_PRICE = 131;
    protected static final int MIN_SERVER_VER_PRE_OPEN_BID_ASK = 132;
    protected static final int MIN_SERVER_VER_REAL_EXPIRATION_DATE = 134;
    protected static final int MIN_SERVER_VER_REALIZED_PNL = 135;
    protected static final int MIN_SERVER_VER_LAST_LIQUIDITY = 136;
    protected static final int MIN_SERVER_VER_TICK_BY_TICK = 137;
    protected static final int MIN_SERVER_VER_DECISION_MAKER = 138;
    protected static final int MIN_SERVER_VER_MIFID_EXECUTION = 139;
    protected static final int MIN_SERVER_VER_TICK_BY_TICK_IGNORE_SIZE = 140;
    protected static final int MIN_SERVER_VER_AUTO_PRICE_FOR_HEDGE = 141;
    protected static final int MIN_SERVER_VER_WHAT_IF_EXT_FIELDS = 142;
    protected static final int MIN_SERVER_VER_SCANNER_GENERIC_OPTS = 143;
    protected static final int MIN_SERVER_VER_API_BIND_ORDER = 144;
    protected static final int MIN_SERVER_VER_ORDER_CONTAINER = 145;
    protected static final int MIN_SERVER_VER_SMART_DEPTH = 146;
    protected static final int MIN_SERVER_VER_REMOVE_NULL_ALL_CASTING = 147;
    protected static final int MIN_SERVER_VER_D_PEG_ORDERS = 148;
    protected static final int MIN_SERVER_VER_MKT_DEPTH_PRIM_EXCHANGE = 149;
    protected static final int MIN_SERVER_VER_REQ_COMPLETED_ORDERS = 150;
    protected static final int MIN_SERVER_VER_PRICE_MGMT_ALGO = 151;
    protected static final int MIN_SERVER_VER_STOCK_TYPE = 152;
    protected static final int MIN_SERVER_VER_ENCODE_MSG_ASCII7 = 153;
    protected static final int MIN_SERVER_VER_SEND_ALL_FAMILY_CODES = 154;
    protected static final int MIN_SERVER_VER_NO_DEFAULT_OPEN_CLOSE = 155;
    protected static final int MIN_SERVER_VER_PRICE_BASED_VOLATILITY = 156;
    protected static final int MIN_SERVER_VER_REPLACE_FA_END = 157;
    protected static final int MIN_SERVER_VER_DURATION = 158;
    protected static final int MIN_SERVER_VER_MARKET_DATA_IN_SHARES = 159;
    protected static final int MIN_SERVER_VER_POST_TO_ATS = 160;
    protected static final int MIN_SERVER_VER_WSHE_CALENDAR = 161;
    protected static final int MIN_SERVER_VER_AUTO_CANCEL_PARENT = 162;
    protected static final int MIN_SERVER_VER_FRACTIONAL_SIZE_SUPPORT = 163;
    protected static final int MIN_SERVER_VER_SIZE_RULES = 164;
    protected static final int MIN_SERVER_VER_HISTORICAL_SCHEDULE = 165;
    protected static final int MIN_SERVER_VER_ADVANCED_ORDER_REJECT = 166;
    protected static final int MIN_SERVER_VER_USER_INFO = 167;
    protected static final int MIN_SERVER_VER_CRYPTO_AGGREGATED_TRADES = 168;
    protected static final int MIN_SERVER_VER_MANUAL_ORDER_TIME = 169;
    protected static final int MIN_SERVER_VER_PEGBEST_PEGMID_OFFSETS = 170;
    protected static final int MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS = 171;
    protected static final int MIN_SERVER_VER_IPO_PRICES = 172;
    protected static final int MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS_DATE = 173;
    protected static final int MIN_SERVER_VER_INSTRUMENT_TIMEZONE = 174;
    protected static final int MIN_SERVER_VER_HMDS_MARKET_DATA_IN_SHARES = 175;
    protected static final int MIN_SERVER_VER_BOND_ISSUERID = 176;
    protected static final int MIN_SERVER_VER_FA_PROFILE_DESUPPORT = 177;
    protected static final int MIN_SERVER_VER_PENDING_PRICE_REVISION = 178;
    protected static final int MIN_SERVER_VER_FUND_DATA_FIELDS = 179;
    protected static final int MIN_SERVER_VER_MANUAL_ORDER_TIME_EXERCISE_OPTIONS = 180;
    protected static final int MIN_SERVER_VER_OPEN_ORDER_AD_STRATEGY = 181;
    protected static final int MIN_SERVER_VER_LAST_TRADE_DATE = 182;
    protected static final int MIN_SERVER_VER_CUSTOMER_ACCOUNT = 183;
    protected static final int MIN_SERVER_VER_PROFESSIONAL_CUSTOMER = 184;
    protected static final int MIN_SERVER_VER_BOND_ACCRUED_INTEREST = 185;
    protected static final int MIN_SERVER_VER_INELIGIBILITY_REASONS = 186;
    protected static final int MIN_SERVER_VER_RFQ_FIELDS = 187;
    protected static final int MIN_SERVER_VER_BOND_TRADING_HOURS = 188;
    protected static final int MIN_SERVER_VER_INCLUDE_OVERNIGHT = 189;
    protected static final int MIN_SERVER_VER_UNDO_RFQ_FIELDS = 190;
    protected static final int MIN_SERVER_VER_PERM_ID_AS_LONG = 191;
    protected static final int MIN_SERVER_VER_CME_TAGGING_FIELDS = 192;
    protected static final int MIN_SERVER_VER_CME_TAGGING_FIELDS_IN_OPEN_ORDER = 193;
    protected static final int MIN_SERVER_VER_ERROR_TIME = 194;
    protected static final int MIN_SERVER_VER_FULL_ORDER_PREVIEW_FIELDS = 195;
    protected static final int MIN_SERVER_VER_HISTORICAL_DATA_END = 196;
    protected static final int MIN_SERVER_VER_CURRENT_TIME_IN_MILLIS = 197;
    protected static final int MIN_SERVER_VER_SUBMITTER = 198;
    protected static final int MIN_SERVER_VER_IMBALANCE_ONLY = 199;
    protected static final int MIN_SERVER_VER_PARAMETRIZED_DAYS_OF_EXECUTIONS = 200;
    protected static final int MIN_SERVER_VER_PROTOBUF = 201;
    protected static final int MIN_SERVER_VER_ZERO_STRIKE = 202;
    protected static final int MIN_SERVER_VER_PROTOBUF_PLACE_ORDER = 203;
    protected static final int MIN_SERVER_VER_PROTOBUF_COMPLETED_ORDER = 204;
    protected static final int MIN_SERVER_VER_PROTOBUF_CONTRACT_DATA = 205;
    protected static final int MIN_SERVER_VER_PROTOBUF_MARKET_DATA = 206;
    
    public static final int MIN_VERSION = 100; // envelope encoding, applicable to useV100Plus mode only
    public static final int MAX_VERSION = MIN_SERVER_VER_PROTOBUF_MARKET_DATA; // ditto
    
    public static final int PROTOBUF_MSG_ID = 200;
    public static final Map<Integer, Integer> PROTOBUF_MSG_IDS = new HashMap<Integer, Integer>();
    static {
        PROTOBUF_MSG_IDS.put(REQ_EXECUTIONS, MIN_SERVER_VER_PROTOBUF);
        PROTOBUF_MSG_IDS.put(PLACE_ORDER, MIN_SERVER_VER_PROTOBUF_PLACE_ORDER);
        PROTOBUF_MSG_IDS.put(CANCEL_ORDER, MIN_SERVER_VER_PROTOBUF_PLACE_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_GLOBAL_CANCEL, MIN_SERVER_VER_PROTOBUF_PLACE_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_ALL_OPEN_ORDERS, MIN_SERVER_VER_PROTOBUF_COMPLETED_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_AUTO_OPEN_ORDERS, MIN_SERVER_VER_PROTOBUF_COMPLETED_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_COMPLETED_ORDERS, MIN_SERVER_VER_PROTOBUF_COMPLETED_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_OPEN_ORDERS, MIN_SERVER_VER_PROTOBUF_COMPLETED_ORDER);
        PROTOBUF_MSG_IDS.put(REQ_CONTRACT_DATA, MIN_SERVER_VER_PROTOBUF_CONTRACT_DATA);
        PROTOBUF_MSG_IDS.put(REQ_MKT_DATA, MIN_SERVER_VER_PROTOBUF_MARKET_DATA);
        PROTOBUF_MSG_IDS.put(CANCEL_MKT_DATA, MIN_SERVER_VER_PROTOBUF_MARKET_DATA);
        PROTOBUF_MSG_IDS.put(REQ_MKT_DEPTH, MIN_SERVER_VER_PROTOBUF_MARKET_DATA);
        PROTOBUF_MSG_IDS.put(CANCEL_MKT_DEPTH, MIN_SERVER_VER_PROTOBUF_MARKET_DATA);
        PROTOBUF_MSG_IDS.put(REQ_MARKET_DATA_TYPE, MIN_SERVER_VER_PROTOBUF_MARKET_DATA);
    }

    protected EReaderSignal m_signal;
    protected EWrapper m_eWrapper;    // msg handler
    protected int m_serverVersion;
    protected String m_TwsTime;
    protected int m_clientId;
    protected boolean m_extraAuth;
    protected boolean m_useV100Plus = true;
    private String m_optionalCapabilities;
    private String m_connectOptions = ""; // iServer rails are used for Connection if this is not null
	protected String m_host;
	protected ETransport m_socketTransport;
	
	public boolean isUseV100Plus() {
		return m_useV100Plus;
	}
	
    private boolean useProtoBuf(int msgId) {
        Integer unifiedVersion = PROTOBUF_MSG_IDS.get(msgId);
        return unifiedVersion != null && unifiedVersion.intValue() <= serverVersion();
    }

    public int serverVersion()          { return m_serverVersion;   }
    public String getTwsConnectionTime()   { return m_TwsTime; }
    public EWrapper wrapper()           { return m_eWrapper; }
    public abstract boolean isConnected();

    // set
    protected synchronized void setExtraAuth(boolean extraAuth) { m_extraAuth = extraAuth; }
    public void optionalCapabilities(String val) 		{ m_optionalCapabilities = val; }

    // get
    public String optionalCapabilities() { return m_optionalCapabilities; }

    public EClient( EWrapper eWrapper, EReaderSignal signal) {
        m_eWrapper = eWrapper;
        m_signal = signal;
        m_clientId = -1;
        m_extraAuth = false;
        m_optionalCapabilities = "";
        m_serverVersion = 0;
    }
    
    protected void sendConnectRequest() throws IOException {
	    // send client version (unless logon via iserver and/or Version > 100)
	    if( !m_useV100Plus || m_connectOptions == null ) {
	    	send( CLIENT_VERSION); // Do not add length prefix here, because Server does not know Client's version yet
	    }
	    else {
	    	// Switch to GW API (Version 100+ requires length prefix)
	    	sendV100APIHeader();
	    }
    }
    
    public void disableUseV100Plus() {
    	if( isConnected() ) {
            m_eWrapper.error(EClientErrors.NO_VALID_ID, Util.currentTimeMillis(), EClientErrors.ALREADY_CONNECTED.code(),
                    EClientErrors.ALREADY_CONNECTED.msg(), null);
    		return;
  		}
    	
    	m_connectOptions = "";
    	m_useV100Plus = false;
    }   
    
    public void setConnectOptions(String options) {
    	if( isConnected() ) {
            m_eWrapper.error(EClientErrors.NO_VALID_ID, Util.currentTimeMillis(), EClientErrors.ALREADY_CONNECTED.code(),
                    EClientErrors.ALREADY_CONNECTED.msg(), null);
    		return;
  		}
    	
    	m_connectOptions = options;
    }

    protected void connectionError() {
        m_eWrapper.error( EClientErrors.NO_VALID_ID, Util.currentTimeMillis(), EClientErrors.CONNECT_FAIL.code(),
                EClientErrors.CONNECT_FAIL.msg(), null);
    }

    protected String checkConnected(String host) {
        if( isConnected()) {
            m_eWrapper.error(EClientErrors.NO_VALID_ID, Util.currentTimeMillis(), EClientErrors.ALREADY_CONNECTED.code(),
                    EClientErrors.ALREADY_CONNECTED.msg(), null);
            return null;
        }
        if( IsEmpty( host) ) {
            host = "127.0.0.1";
        }
        return host;
    }
    
    public abstract void eDisconnect();
    
    public synchronized void startAPI() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 2;

        try {
        	Builder b = prepareBuffer(); 
        	
            sendMsgId(b, START_API);
            b.send(VERSION);
            b.send(m_clientId);
            
            if (m_serverVersion >= MIN_SERVER_VER_OPTIONAL_CAPABILITIES) {
                b.send(m_optionalCapabilities);
            }
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_STARTAPI, e.toString());
            close();
        }
    }

    public synchronized void cancelScannerSubscription( int tickerId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < 24) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support API scanner subscription.");
          return;
        }

        final int VERSION = 1;

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_SCANNER_SUBSCRIPTION);
            b.send( VERSION);
            b.send( tickerId);
            
            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANSCANNER, e.toString());
            close();
        }
    }

    public synchronized void reqScannerParameters() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < 24) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support API scanner subscription.");
          return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_SCANNER_PARAMETERS);
            b.send(VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_REQSCANNERPARAMETERS, e.toString());
            close();
        }
    }

    public synchronized void reqScannerSubscription(int tickerId, 
            ScannerSubscription subscription, 
            List<TagValue> scannerSubscriptionOptions, 
            List<TagValue> scannerSubscriptionFilterOptions) {
        // not connected?
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < 24) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support API scanner subscription.");
          
          return;
        }
        
        if (m_serverVersion < MIN_SERVER_VER_SCANNER_GENERIC_OPTS && scannerSubscriptionFilterOptions != null) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS, 
                    " It does not support API scanner subscription generic filter options");
            
            return;
        }

        final int VERSION = 4;

        try {
            Builder b = prepareBuffer();

            sendMsgId(b, REQ_SCANNER_SUBSCRIPTION);
            
            if (m_serverVersion < MIN_SERVER_VER_SCANNER_GENERIC_OPTS) {
                b.send(VERSION);
            }
            
            b.send(tickerId);
            b.sendMax(subscription.numberOfRows());
            b.send(subscription.instrument());
            b.send(subscription.locationCode());
            b.send(subscription.scanCode());

            b.sendMax(subscription.abovePrice());
            b.sendMax(subscription.belowPrice());
            b.sendMax(subscription.aboveVolume());
            b.sendMax(subscription.marketCapAbove());
            b.sendMax(subscription.marketCapBelow());
            b.send(subscription.moodyRatingAbove());
            b.send(subscription.moodyRatingBelow());
            b.send(subscription.spRatingAbove());
            b.send(subscription.spRatingBelow());
            b.send(subscription.maturityDateAbove());
            b.send(subscription.maturityDateBelow());
            b.sendMax(subscription.couponRateAbove());
            b.sendMax(subscription.couponRateBelow());
            b.send(subscription.excludeConvertible());           

            if (m_serverVersion >= 25) {
                b.sendMax(subscription.averageOptionVolumeAbove());
                b.send(subscription.scannerSettingPairs());
            }

            if (m_serverVersion >= 27) {
                b.send(subscription.stockTypeFilter());
            }

            if (m_serverVersion >= MIN_SERVER_VER_SCANNER_GENERIC_OPTS) {
                b.send(scannerSubscriptionFilterOptions);
            }
            
            // send scannerSubscriptionOptions parameter
            if (m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(scannerSubscriptionOptions);
            }
            
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch(Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQSCANNER, e.toString());
            close();
        }
    }

    public synchronized void reqMktData(int tickerId, Contract contract,
    		String genericTickList, boolean snapshot, boolean regulatorySnapshot, List<TagValue> mktDataOptions) {
        if (useProtoBuf(REQ_MKT_DATA)) {
            reqMarketDataProtoBuf(EClientUtils.createMarketDataRequestProto(tickerId, contract, genericTickList, snapshot, regulatorySnapshot, mktDataOptions));
            return;
        }

        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_SNAPSHOT_MKT_DATA && snapshot) {
        	error(tickerId, EClientErrors.UPDATE_TWS,
        			"  It does not support snapshot market data requests.");
        	return;
        }

        if (m_serverVersion < MIN_SERVER_VER_DELTA_NEUTRAL) {
        	if (contract.deltaNeutralContract() != null) {
        		error(tickerId, EClientErrors.UPDATE_TWS,
        			"  It does not support delta-neutral orders.");
        		return;
        	}
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_MKT_DATA_CONID) {
            if (contract.conid() > 0) {
                error(tickerId, EClientErrors.UPDATE_TWS,
                    "  It does not support conId parameter.");
                return;
            }
        }

        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass())) {
                error(tickerId, EClientErrors.UPDATE_TWS,
                    "  It does not support tradingClass parameter in reqMarketData.");
                return;
            }
        }

        final int VERSION = 11;

        try {
            // send req mkt data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_MKT_DATA);
            b.send(VERSION);
            b.send(tickerId);

            // send contract fields
            if (m_serverVersion >= MIN_SERVER_VER_REQ_MKT_DATA_CONID) {
                b.send(contract.conid());
            }
            
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            
            if (m_serverVersion >= 15) {
                b.send(contract.multiplier());
            }
            
            b.send(contract.exchange());
            
            if (m_serverVersion >= 14) {
                b.send(contract.primaryExch());
            }
            
            b.send(contract.currency());
            
            if (m_serverVersion >= 2) {
                b.send(contract.localSymbol());
            }
            
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            
            if (m_serverVersion >= 8 && SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
                if (contract.comboLegs() == null) {
                    b.send(0);
                }
                else {
                    b.send(contract.comboLegs().size());

                    ComboLeg comboLeg;
                    
                    for (int i = 0; i < contract.comboLegs().size(); i++) {
                        comboLeg = contract.comboLegs().get(i);
                        
                        b.send(comboLeg.conid());
                        b.send(comboLeg.ratio());
                        b.send(comboLeg.getAction());
                        b.send(comboLeg.exchange());
                    }
                }
            }

            if (m_serverVersion >= MIN_SERVER_VER_DELTA_NEUTRAL) {
         	   if (contract.deltaNeutralContract() != null) {
         		   DeltaNeutralContract deltaNeutralContract = contract.deltaNeutralContract();
         		   
         		   b.send(true);
         		   b.send(deltaNeutralContract.conid());
         		   b.send(deltaNeutralContract.delta());
         		   b.send(deltaNeutralContract.price());
         	   }
         	   else {
         		   b.send( false);
         	   }
            }

            if (m_serverVersion >= 31) {
            	/*
            	 * Note: Even though SHORTABLE tick type supported only
            	 *       starting server version 33 it would be relatively
            	 *       expensive to expose this restriction here.
            	 *
            	 *       Therefore we are relying on TWS doing validation.
            	 */
            	b.send(genericTickList);
            }
            
            if (m_serverVersion >= MIN_SERVER_VER_SNAPSHOT_MKT_DATA) {
            	b.send(snapshot);
            }
            
            if (m_serverVersion >= MIN_SERVER_VER_REQ_SMART_COMPONENTS) {
            	b.send(regulatorySnapshot);
            }
            
            // send mktDataOptions parameter
            if(m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(mktDataOptions);
            }
            
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch(Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQMKT, e.toString());
            close();
        }
    }

    public synchronized void reqMarketDataProtoBuf(MarketDataRequestProto.MarketDataRequest marketDataRequestProto) {
        if (marketDataRequestProto == null) {
            return;
        }

        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = marketDataRequestProto.hasReqId() ? marketDataRequestProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send req market data msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_MKT_DATA + PROTOBUF_MSG_ID);

            // send market data request
            byte[] byteArray = marketDataRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQMKT, e.toString());
            close();
        }
    }

    public synchronized void cancelHistoricalData( int tickerId ) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < 24) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support historical data query cancellation.");
          return;
        }

        final int VERSION = 1;

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_HISTORICAL_DATA);
            b.send( VERSION);
            b.send( tickerId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANHISTDATA, e.toString());
            close();
        }
    }

    public synchronized void cancelRealTimeBars(int tickerId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REAL_TIME_BARS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                  "  It does not support realtime bar data query cancellation.");
            return;
        }

        final int VERSION = 1;

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_REAL_TIME_BARS);
            b.send( VERSION);
            b.send( tickerId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANRTBARS, e.toString());
            close();
        }
    }

    /** Note that formatData parameter affects intra-day bars only; 1-day bars always return with date in YYYYMMDD format. */
    public synchronized void reqHistoricalData( int tickerId, Contract contract,
                                                String endDateTime, String durationStr,
                                                String barSizeSetting, String whatToShow,
                                                int useRTH, int formatDate, boolean keepUpToDate, List<TagValue> chartOptions) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 6;

        try {
          if (m_serverVersion < 16) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                  "  It does not support historical data backfill.");
            return;
          }

          if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
              if (!IsEmpty(contract.tradingClass()) || (contract.conid() > 0)) {
                  error(tickerId, EClientErrors.UPDATE_TWS,
                      "  It does not support conId and tradingClass parameters in reqHistoricalData.");
                  return;
              }
          }

          if (m_serverVersion < MIN_SERVER_VER_HISTORICAL_SCHEDULE) {
              if (!IsEmpty(whatToShow) && whatToShow.equalsIgnoreCase(WhatToShow.SCHEDULE.name())) {
                  error(tickerId, EClientErrors.UPDATE_TWS,
                      "  It does not support requesting of historical schedule.");
                  return;
              }
          }

          Builder b = prepareBuffer(); 

          sendMsgId(b, REQ_HISTORICAL_DATA);
          
          if (m_serverVersion < MIN_SERVER_VER_SYNT_REALTIME_BARS) {
              b.send(VERSION);
          }
          
          b.send(tickerId);

          // send contract fields
          if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
              b.send(contract.conid());
          }
          
          b.send(contract.symbol());
          b.send(contract.getSecType());
          b.send(contract.lastTradeDateOrContractMonth());
          b.sendMax(contract.strike());
          b.send(contract.getRight());
          b.send(contract.multiplier());
          b.send(contract.exchange());
          b.send(contract.primaryExch());
          b.send(contract.currency());
          b.send(contract.localSymbol());
          
          if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
              b.send(contract.tradingClass());
          }
          
          if (m_serverVersion >= 31) {
        	  b.send(contract.includeExpired() ? 1 : 0);
          }
          
          if (m_serverVersion >= 20) {
              b.send(endDateTime);
              b.send(barSizeSetting);
          }
          
          b.send(durationStr);
          b.send(useRTH);
          b.send(whatToShow);
          
          if (m_serverVersion > 16) {
              b.send(formatDate);
          }
          
          if (SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
              if (contract.comboLegs() == null) {
                  b.send(0);
              }
              else {
                  b.send(contract.comboLegs().size());

                  ComboLeg comboLeg;
                  
                  for (int i = 0; i < contract.comboLegs().size(); i++) {
                      comboLeg = contract.comboLegs().get(i);
                      
                      b.send(comboLeg.conid());
                      b.send(comboLeg.ratio());
                      b.send(comboLeg.getAction());
                      b.send(comboLeg.exchange());
                  }
              }
          }
          
          if (m_serverVersion >= MIN_SERVER_VER_SYNT_REALTIME_BARS) {
              b.send(keepUpToDate);
          }
          
          // send chartOptions parameter
          if(m_serverVersion >= MIN_SERVER_VER_LINKING) {
              b.send(chartOptions);
          }
          
          closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch(Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQHISTDATA, e.toString());
            close();
        }
    }

    /** Note that formatData parameter affects intra-day bars only; 1-day bars always return with date in YYYYMMDD format. */
    public synchronized void reqHeadTimestamp(int tickerId, Contract contract,
                                                String whatToShow, int useRTH, int formatDate) {
    	// not connected?
    	if( !isConnected()) {
    		notConnected();
    		return;
    	}

    	try {
    		if (m_serverVersion < MIN_SERVER_VER_REQ_HEAD_TIMESTAMP) {              
    			error(tickerId, EClientErrors.UPDATE_TWS,
    					"  It does not support head time stamp requests.");
    			return;
    		}

    		Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_HEAD_TIMESTAMP);
    		b.send(tickerId);
    		b.send(contract);
    		b.send(useRTH);
    		b.send(whatToShow);          
    		b.send(formatDate);

        	closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch (Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQHEADTIMESTAMP, e.toString());
            close();
        }
    }
    
    public synchronized void cancelHeadTimestamp(int tickerId) {
    	// not connected?
    	if( !isConnected()) {
    		notConnected();
    		return;
    	}

    	try {
    		if (m_serverVersion < MIN_SERVER_VER_CANCEL_HEADTIMESTAMP) {
    			error(tickerId, EClientErrors.UPDATE_TWS,
    					"  It does not support head time stamp requests canceling.");
    			return;
    		}

    		Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_HEAD_TIMESTAMP);
    		b.send(tickerId);
    		closeAndSend(b);
    	}
    	catch (Exception e) {
    		error(tickerId, EClientErrors.FAIL_SEND_CANCELHEADTIMESTAMP, e.toString());
    		close();
        }
   }
    
    
    public synchronized void reqRealTimeBars(int tickerId, Contract contract, int barSize, String whatToShow, boolean useRTH, List<TagValue> realTimeBarsOptions) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REAL_TIME_BARS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                  "  It does not support real time bars.");
            return;
        }
        
        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass()) || (contract.conid() > 0)) {
                  error(tickerId, EClientErrors.UPDATE_TWS,
                      "  It does not support conId and tradingClass parameters in reqRealTimeBars.");
                  return;
            }
        }

        final int VERSION = 3;

        try {
            // send req mkt data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_REAL_TIME_BARS);
            b.send(VERSION);
            b.send(tickerId);

            // send contract fields
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.conid());
            }
            
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            b.send(contract.multiplier());
            b.send(contract.exchange());
            b.send(contract.primaryExch());
            b.send(contract.currency());
            b.send(contract.localSymbol());
            
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            
            b.send(barSize);  // this parameter is not currently used
            b.send(whatToShow);
            b.send(useRTH);

            // send realTimeBarsOptions parameter
            if(m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(realTimeBarsOptions);
            }
            
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch(Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQRTBARS, e.toString());
            close();
        }
    }

    public synchronized void reqContractDetails(int reqId, Contract contract) {
        if (useProtoBuf(REQ_CONTRACT_DATA)) {
            reqContractDataProtoBuf(EClientUtils.createContractDataRequestProto(reqId, contract));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // This feature is only available for versions of TWS >=4
        if( m_serverVersion < 4) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS.code(),
                            EClientErrors.UPDATE_TWS.msg());
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_SEC_ID_TYPE) {
            if (!IsEmpty(contract.getSecIdType()) || !IsEmpty(contract.secId())) {
        		error(reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support secIdType and secId parameters.");
        		return;
        	}
        }

        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass())) {
                  error(reqId, EClientErrors.UPDATE_TWS,
                      "  It does not support tradingClass parameter in reqContractDetails.");
                  return;
            }
        }
        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            if (!IsEmpty(contract.primaryExch())) {
        		error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support primaryExchange parameter in reqContractDetails.");
                return;
            }
        }

        if (m_serverVersion < MIN_SERVER_VER_BOND_ISSUERID) {
            if (!IsEmpty(contract.issuerId())) {
        		error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support issuerId parameter in reqContractDetails.");
                return;
            }
        }
        
        final int VERSION = 8;

        try {
            // send req mkt data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_CONTRACT_DATA);
            b.send( VERSION);

            if (m_serverVersion >= MIN_SERVER_VER_CONTRACT_DATA_CHAIN) {
            	b.send( reqId);
            }

            // send contract fields
            if (m_serverVersion >= MIN_SERVER_VER_CONTRACT_CONID) {
            	b.send(contract.conid());
            }
            b.send( contract.symbol());
            b.send( contract.getSecType());
            b.send( contract.lastTradeDateOrContractMonth());
            b.sendMax( contract.strike());
            b.send( contract.getRight());
            if (m_serverVersion >= 15) {
                b.send(contract.multiplier());
            }
            
            if (m_serverVersion >= MIN_SERVER_VER_PRIMARYEXCH)
            {
            	b.send(contract.exchange());
            	b.send(contract.primaryExch());
            }
            else if (m_serverVersion >= MIN_SERVER_VER_LINKING) {
                if (!IsEmpty(contract.primaryExch())
                        && ("BEST".equals(contract.exchange()) || "SMART".equals(contract.exchange()))) {
                   	b.send(contract.exchange() + ":" + contract.primaryExch());
                } else {
                	b.send(contract.exchange());
                }
            }
            
            b.send( contract.currency());
            b.send( contract.localSymbol());
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            if (m_serverVersion >= 31) {
                b.send(contract.includeExpired());
            }
            if (m_serverVersion >= MIN_SERVER_VER_SEC_ID_TYPE) {
            	b.send( contract.getSecIdType());
            	b.send( contract.secId());
            }
            if (m_serverVersion >= MIN_SERVER_VER_BOND_ISSUERID) {
                b.send(contract.issuerId());
            }
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQCONTRACT, e.toString());
            close();
        }
    }

    public synchronized void reqContractDataProtoBuf(ContractDataRequestProto.ContractDataRequest contractDataRequestProto) {
        if (contractDataRequestProto == null) {
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = contractDataRequestProto.hasReqId() ? contractDataRequestProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send req contract data msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_CONTRACT_DATA + PROTOBUF_MSG_ID);

            // send contract data request
            byte[] byteArray = contractDataRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQCONTRACT, e.toString());
            close();
        }
    }

    public synchronized void reqMktDepth( int tickerId, Contract contract, int numRows, boolean isSmartDepth, List<TagValue> mktDepthOptions) {
        if (useProtoBuf(REQ_MKT_DEPTH)) {
            reqMarketDepthProtoBuf(EClientUtils.createMarketDepthRequestProto(tickerId, contract, numRows, isSmartDepth, mktDepthOptions));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // This feature is only available for versions of TWS >=6
        if( m_serverVersion < 6) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS.code(),
                    EClientErrors.UPDATE_TWS.msg());
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass()) || (contract.conid() > 0)) {
                  error(tickerId, EClientErrors.UPDATE_TWS,
                      "  It does not support conId and tradingClass parameters in reqMktDepth.");
                  return;
            }
        }
        
        if (m_serverVersion < MIN_SERVER_VER_SMART_DEPTH && isSmartDepth) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support SMART depth request.");
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MKT_DEPTH_PRIM_EXCHANGE && !IsEmpty(contract.primaryExch())) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support primaryExch parameter in reqMktDepth.");
            return;
        }
        
        final int VERSION = 5;

        try {
            // send req mkt data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_MKT_DEPTH);
            b.send(VERSION);
            b.send(tickerId);

            // send contract fields
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.conid());
            }
            
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            
            if (m_serverVersion >= 15) {
              b.send(contract.multiplier());
            }
            
            b.send(contract.exchange());
            if (m_serverVersion >= MIN_SERVER_VER_MKT_DEPTH_PRIM_EXCHANGE) {
                b.send(contract.primaryExch());
            }
            
            b.send(contract.currency());
            b.send(contract.localSymbol());
            
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            
            if (m_serverVersion >= 19) {
                b.send(numRows);
            }

            if (m_serverVersion >= MIN_SERVER_VER_SMART_DEPTH) {
                b.send(isSmartDepth);
            }
            
            // send mktDepthOptions parameter
            if(m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(mktDepthOptions);
            }
            
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch(Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQMKTDEPTH, e.toString());
            close();
        }
    }

    public synchronized void reqMarketDepthProtoBuf(MarketDepthRequestProto.MarketDepthRequest marketDepthRequestProto) {
        if (marketDepthRequestProto == null) {
            return;
        }

        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = marketDepthRequestProto.hasReqId() ? marketDepthRequestProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send req market depth msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_MKT_DEPTH + PROTOBUF_MSG_ID);

            // send market depth request
            byte[] byteArray = marketDepthRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQMKTDEPTH, e.toString());
            close();
        }
    }

    public synchronized void cancelMktData( int tickerId) {
        if (useProtoBuf(CANCEL_MKT_DATA)) {
            cancelMarketDataProtoBuf(EClientUtils.createCancelMarketDataProto(tickerId));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_MKT_DATA);
            b.send( VERSION);
            b.send( tickerId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANMKT, e.toString());
            close();
        }
    }

    public synchronized void cancelMarketDataProtoBuf(CancelMarketDataProto.CancelMarketData cancelMarketDataProto) {
        if (cancelMarketDataProto == null) {
            return;
        }

        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = cancelMarketDataProto.hasReqId() ? cancelMarketDataProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send cancel market data msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, CANCEL_MKT_DATA + PROTOBUF_MSG_ID);

            // send cancel market data
            byte[] byteArray = cancelMarketDataProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_CANMKT, e.toString());
            close();
        }
    }

    public synchronized void cancelMktDepth( int tickerId, boolean isSmartDepth) {
        if (useProtoBuf(CANCEL_MKT_DEPTH)) {
            cancelMarketDepthProtoBuf(EClientUtils.createCancelMarketDepthProto(tickerId, isSmartDepth));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // This feature is only available for versions of TWS >=6
        if( m_serverVersion < 6) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS.code(),
                    EClientErrors.UPDATE_TWS.msg());
            return;
        }
        
        if (m_serverVersion < MIN_SERVER_VER_SMART_DEPTH && isSmartDepth) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support SMART depth cancel.");
            return;
        }
        

        final int VERSION = 1;

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_MKT_DEPTH);
            b.send( VERSION);
            b.send( tickerId);
            
            if (m_serverVersion >= MIN_SERVER_VER_SMART_DEPTH) {
                b.send( isSmartDepth);
            }

            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANMKTDEPTH, e.toString());
            close();
        }
    }

    public synchronized void cancelMarketDepthProtoBuf(CancelMarketDepthProto.CancelMarketDepth cancelMarketDepthProto) {
        if (cancelMarketDepthProto == null) {
            return;
        }

        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = cancelMarketDepthProto.hasReqId() ? cancelMarketDepthProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send cancel market depth msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, CANCEL_MKT_DEPTH + PROTOBUF_MSG_ID);

            // send cancel market depth
            byte[] byteArray = cancelMarketDepthProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_CANMKTDEPTH, e.toString());
            close();
        }
    }

    public synchronized void exerciseOptions( int tickerId, Contract contract,
                                              int exerciseAction, int exerciseQuantity,
                                              String account, int override, String manualOrderTime, 
                                              String customerAccount, boolean professionalCustomer) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 2;

        try {
            if (m_serverVersion < 21) {
              error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                    "  It does not support options exercise from the API.");
              return;
            }

            if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
                if (!IsEmpty(contract.tradingClass()) || (contract.conid() > 0)) {
                      error(tickerId, EClientErrors.UPDATE_TWS,
                          "  It does not support conId and tradingClass parameters in exerciseOptions.");
                      return;
                }
            }

            if (m_serverVersion < MIN_SERVER_VER_MANUAL_ORDER_TIME_EXERCISE_OPTIONS && !IsEmpty(manualOrderTime)) {
                  error(tickerId, EClientErrors.UPDATE_TWS,
                      "  It does not support manual order time parameter in exerciseOptions.");
                  return;
            }

            if (m_serverVersion < MIN_SERVER_VER_CUSTOMER_ACCOUNT) {
                if (!IsEmpty(customerAccount)) {
                    error(tickerId, EClientErrors.UPDATE_TWS, "  It does not support customer account parameter in exerciseOptions.");
                    return;
                }
            }

            if (m_serverVersion < MIN_SERVER_VER_PROFESSIONAL_CUSTOMER) {
                if (professionalCustomer) {
                    error(tickerId, EClientErrors.UPDATE_TWS, "  It does not support professional customer parameter in exerciseOptions.");
                    return;
                }
            }
            
            Builder b = prepareBuffer(); 

            sendMsgId(b, EXERCISE_OPTIONS);
            b.send(VERSION);
            b.send(tickerId);

            // send contract fields
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.conid());
            }
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            b.send(contract.multiplier());
            b.send(contract.exchange());
            b.send(contract.currency());
            b.send(contract.localSymbol());
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            b.send(exerciseAction);
            b.send(exerciseQuantity);
            b.send(account);
            b.send(override);
            if (m_serverVersion >= MIN_SERVER_VER_MANUAL_ORDER_TIME_EXERCISE_OPTIONS) {
                b.send(manualOrderTime);
            }
            if (m_serverVersion >= MIN_SERVER_VER_CUSTOMER_ACCOUNT) {
                b.send(customerAccount);
            }
            if (m_serverVersion >= MIN_SERVER_VER_PROFESSIONAL_CUSTOMER) {
                b.send(professionalCustomer);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch (Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQMKT, e.toString());
            close();
        }
    }

    public synchronized void placeOrderProtoBuf(PlaceOrderRequestProto.PlaceOrderRequest placeOrderRequestProto) {
        if (placeOrderRequestProto == null) {
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        int orderId = placeOrderRequestProto.hasOrderId() ? placeOrderRequestProto.getOrderId() : Integer.MAX_VALUE;

        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, PLACE_ORDER + PROTOBUF_MSG_ID);

            byte[] byteArray = placeOrderRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(orderId, e.error(), e.text());
        }
        catch(Exception e) {
            error(orderId, EClientErrors.FAIL_SEND_ORDER, e.toString());
            close();
        }
    }

    public synchronized void placeOrder(int id, Contract contract, Order order) {
        try {
	        if (useProtoBuf(PLACE_ORDER)) {
	            placeOrderProtoBuf(EClientUtils.createPlaceOrderRequestProto(id, contract, order));
	            return;
	        }
	
	        // not connected?
	        if( !isConnected()) {
	            notConnected();
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SCALE_ORDERS) {
	        	if (order.scaleInitLevelSize() != Integer.MAX_VALUE ||
	        		order.scalePriceIncrement() != Double.MAX_VALUE) {
	        		error(id, EClientErrors.UPDATE_TWS,
	            		"  It does not support Scale orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SSHORT_COMBO_LEGS) {
	        	if (!contract.comboLegs().isEmpty()) {
	                for( ComboLeg comboLeg : contract.comboLegs() ) {
	                    if (comboLeg.shortSaleSlot() != 0 ||
	                    	!IsEmpty(comboLeg.designatedLocation())) {
	                		error(id, EClientErrors.UPDATE_TWS,
	                			"  It does not support SSHORT flag for combo legs.");
	                		return;
	                    }
	                }
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_WHAT_IF_ORDERS) {
	        	if (order.whatIf()) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support what-if orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_DELTA_NEUTRAL) {
	        	if (contract.deltaNeutralContract() != null) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support delta-neutral orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SCALE_ORDERS2) {
	        	if (order.scaleSubsLevelSize() != Integer.MAX_VALUE) {
	        		error(id, EClientErrors.UPDATE_TWS,
	            		"  It does not support Subsequent Level Size for Scale orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_ALGO_ORDERS) {
	        	if (!IsEmpty(order.getAlgoStrategy())) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support algo orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_NOT_HELD) {
	        	if (order.notHeld()) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support notHeld parameter.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SEC_ID_TYPE) {
	        	if (!IsEmpty(contract.getSecIdType()) || !IsEmpty(contract.secId())) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support secIdType and secId parameters.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_PLACE_ORDER_CONID) {
	        	if (contract.conid() > 0) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support conId parameter.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SSHORTX) {
	        	if (order.exemptCode() != -1) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support exemptCode parameter.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SSHORTX) {
	        	if (!contract.comboLegs().isEmpty()) {
	                for( ComboLeg comboLeg : contract.comboLegs() ) {
	                    if (comboLeg.exemptCode() != -1) {
	                		error(id, EClientErrors.UPDATE_TWS,
	                			"  It does not support exemptCode parameter.");
	                		return;
	                    }
	                }
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_HEDGE_ORDERS) {
	        	if (!IsEmpty(order.getHedgeType())) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support hedge orders.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_OPT_OUT_SMART_ROUTING) {
	        	if (order.optOutSmartRouting()) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support optOutSmartRouting parameter.");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_DELTA_NEUTRAL_CONID) {
	        	if (order.deltaNeutralConId() > 0
	        			|| !IsEmpty(order.deltaNeutralSettlingFirm())
	        			|| !IsEmpty(order.deltaNeutralClearingAccount())
	        			|| !IsEmpty(order.deltaNeutralClearingIntent())
	        			) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support deltaNeutral parameters: ConId, SettlingFirm, ClearingAccount, ClearingIntent");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_DELTA_NEUTRAL_OPEN_CLOSE) {
	        	if (!IsEmpty(order.deltaNeutralOpenClose())
	        			|| order.deltaNeutralShortSale()
	        			|| order.deltaNeutralShortSaleSlot() > 0
	        			|| !IsEmpty(order.deltaNeutralDesignatedLocation())
	        			) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support deltaNeutral parameters: OpenClose, ShortSale, ShortSaleSlot, DesignatedLocation");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SCALE_ORDERS3) {
	        	if (order.scalePriceIncrement() > 0 && order.scalePriceIncrement() != Double.MAX_VALUE) {
	        		if (order.scalePriceAdjustValue() != Double.MAX_VALUE ||
	        			order.scalePriceAdjustInterval() != Integer.MAX_VALUE ||
	        			order.scaleProfitOffset() != Double.MAX_VALUE ||
	        			order.scaleAutoReset() ||
	        			order.scaleInitPosition() != Integer.MAX_VALUE ||
	        			order.scaleInitFillQty() != Integer.MAX_VALUE ||
	        			order.scaleRandomPercent()) {
	        			error(id, EClientErrors.UPDATE_TWS,
	        				"  It does not support Scale order parameters: PriceAdjustValue, PriceAdjustInterval, " +
	        				"ProfitOffset, AutoReset, InitPosition, InitFillQty and RandomPercent");
	        			return;
	        		}
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_ORDER_COMBO_LEGS_PRICE && SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
	        	if (!order.orderComboLegs().isEmpty()) {
	                for( OrderComboLeg orderComboLeg : order.orderComboLegs() ) {
	        			if (orderComboLeg.price() != Double.MAX_VALUE) {
	        			error(id, EClientErrors.UPDATE_TWS,
	        				"  It does not support per-leg prices for order combo legs.");
	        			return;
	        			}
	        		}
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_TRAILING_PERCENT) {
	        	if (order.trailingPercent() != Double.MAX_VALUE) {
	        		error(id, EClientErrors.UPDATE_TWS,
	        			"  It does not support trailing percent parameter");
	        		return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
	            if (!IsEmpty(contract.tradingClass())) {
	                  error(id, EClientErrors.UPDATE_TWS,
	                      "  It does not support tradingClass parameters in placeOrder.");
	                  return;
	            }
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_ALGO_ID && !IsEmpty(order.algoId()) ) {
	            error(id, EClientErrors.UPDATE_TWS, " It does not support algoId parameter");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SCALE_TABLE) {
	            if (!IsEmpty(order.scaleTable()) || !IsEmpty(order.activeStartTime()) || !IsEmpty(order.activeStopTime())) {
	                  error(id, EClientErrors.UPDATE_TWS,
	                      "  It does not support scaleTable, activeStartTime and activeStopTime parameters.");
	                  return;
	            }
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_ORDER_SOLICITED) {
	        	if (order.solicited()) {
	        		error(id, EClientErrors.UPDATE_TWS,
	                        "  It does not support order solicited parameter.");
	                return;
	        	}
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_MODELS_SUPPORT) {
	            if (!IsEmpty(order.modelCode())) {
	                error(id, EClientErrors.UPDATE_TWS,
	                        "  It does not support model code parameter.");
	                return;
	            }
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_EXT_OPERATOR && !IsEmpty(order.extOperator()) ) {
	            error(id, EClientErrors.UPDATE_TWS, " It does not support ext operator");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_SOFT_DOLLAR_TIER && 
	                (!IsEmpty(order.softDollarTier().name()) || !IsEmpty(order.softDollarTier().value()))) {
	            error(id, EClientErrors.UPDATE_TWS, " It does not support soft dollar tier");
	            return;
	        }
	        
	
	        if (m_serverVersion < MIN_SERVER_VER_CASH_QTY) {
	            if (order.cashQty() != Double.MAX_VALUE) {
	                error(id, EClientErrors.UPDATE_TWS,
	                    " It does not support cash quantity parameter");
	                return;
	            }
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_DECISION_MAKER
	            && (!IsEmpty(order.mifid2DecisionMaker())
	                || !IsEmpty(order.mifid2DecisionAlgo()))) {
	            error(id, EClientErrors.UPDATE_TWS,
	                    " It does not support MIFID II decision maker parameters");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_MIFID_EXECUTION
	                && (!IsEmpty(order.mifid2ExecutionTrader())
	                        || !IsEmpty(order.mifid2ExecutionAlgo()))) {
	            error(id, EClientErrors.UPDATE_TWS,
	                    " It does not support MIFID II execution parameters");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_AUTO_PRICE_FOR_HEDGE
	                && order.dontUseAutoPriceForHedge()) {
	            error(id, EClientErrors.UPDATE_TWS,
	                "  It does not support don't use auto price for hedge parameter.");
	            return;
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_ORDER_CONTAINER
	                && order.isOmsContainer()) {
	            error(id, EClientErrors.UPDATE_TWS,
	                    "  It does not support oms container parameter.");
	            return;           
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_D_PEG_ORDERS
	                && order.discretionaryUpToLimitPrice()) {
	            error(id, EClientErrors.UPDATE_TWS,
	                    "  It does not support D-Peg orders.");
	            return;           
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_PRICE_MGMT_ALGO 
	                && order.usePriceMgmtAlgo() != null) {
	            error(id, EClientErrors.UPDATE_TWS, "  It does not support price management algo parameter");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_DURATION 
	                && order.duration() != Integer.MAX_VALUE) {
	            error(id, EClientErrors.UPDATE_TWS, "  It does not support duration attribute");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_POST_TO_ATS 
	                && order.postToAts() != Integer.MAX_VALUE) {
	            error(id, EClientErrors.UPDATE_TWS, "  It does not support postToAts attribute");
	            return;
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_AUTO_CANCEL_PARENT 
	                && order.autoCancelParent()) {
	            error(id, EClientErrors.UPDATE_TWS, "  It does not support autoCancelParent attribute");
	            return;
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_ADVANCED_ORDER_REJECT) {
	            if (!IsEmpty(order.advancedErrorOverride())) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support advanced error override attribute");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_MANUAL_ORDER_TIME) {
	            if (!IsEmpty(order.manualOrderTime())) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support manual order time attribute");
	                return;
	            }
	        }
	        
	        if (m_serverVersion < MIN_SERVER_VER_PEGBEST_PEGMID_OFFSETS) {
	            if (order.minTradeQty() != Integer.MAX_VALUE ||
	                order.minCompeteSize() != Integer.MAX_VALUE ||
	                order.competeAgainstBestOffset() != Double.MAX_VALUE ||
	                order.midOffsetAtWhole() != Double.MAX_VALUE ||
	                order.midOffsetAtHalf() != Double.MAX_VALUE) {
	                error(id, EClientErrors.UPDATE_TWS,
	                    "  It does not support PEG BEST / PEG MID order parameters: minTradeQty, minCompeteSize, " +
	                    "competeAgainstBestOffset, midOffsetAtWhole and midOffsetAtHalf");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_CUSTOMER_ACCOUNT) {
	            if (!IsEmpty(order.customerAccount())) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support customer account parameter");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_PROFESSIONAL_CUSTOMER) {
	            if (order.professionalCustomer()) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support professional customer parameter");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_INCLUDE_OVERNIGHT) {
	            if (order.includeOvernight()) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support include overnight parameter");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_CME_TAGGING_FIELDS) {
	            if (order.manualOrderIndicator() != Integer.MAX_VALUE) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support manual order indicator parameter");
	                return;
	            }
	        }
	
	        if (m_serverVersion < MIN_SERVER_VER_IMBALANCE_ONLY) {
	            if (order.imbalanceOnly()) {
	                error(id, EClientErrors.UPDATE_TWS, "  It does not support imbalance only parameter");
	                return;
	            }
	        }
	
	        int VERSION = (m_serverVersion < MIN_SERVER_VER_NOT_HELD) ? 27 : 45;

	        // send place order msg
            final Builder b = prepareBuffer(); 

            sendMsgId(b, PLACE_ORDER);
            
            if (m_serverVersion < MIN_SERVER_VER_ORDER_CONTAINER) {
                b.send( VERSION);
            }
            
            b.send( id);

            // send contract fields
            if( m_serverVersion >= MIN_SERVER_VER_PLACE_ORDER_CONID) {
                b.send(contract.conid());
            }
            b.send( contract.symbol());
            b.send( contract.getSecType());
            b.send( contract.lastTradeDateOrContractMonth());
            b.sendMax( contract.strike());
            b.send( contract.getRight());
            if (m_serverVersion >= 15) {
                b.send(contract.multiplier());
            }
            b.send( contract.exchange());
            if( m_serverVersion >= 14) {
              b.send(contract.primaryExch());
            }
            b.send( contract.currency());
            if( m_serverVersion >= 2) {
                b.send (contract.localSymbol());
            }
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }
            if( m_serverVersion >= MIN_SERVER_VER_SEC_ID_TYPE){
            	b.send( contract.getSecIdType());
            	b.send( contract.secId());
            }

            // send main order fields
            b.send( order.getAction());
            
			if (m_serverVersion >= MIN_SERVER_VER_FRACTIONAL_POSITIONS)
				b.send(order.totalQuantity().toString());
			else
				b.send((int) order.totalQuantity().longValue());
            
			b.send( order.getOrderType());
            if (m_serverVersion < MIN_SERVER_VER_ORDER_COMBO_LEGS_PRICE) {
                b.send( order.lmtPrice() == Double.MAX_VALUE ? 0 : order.lmtPrice());
            }
            else {
                b.sendMax( order.lmtPrice());
            }
            if (m_serverVersion < MIN_SERVER_VER_TRAILING_PERCENT) {
                b.send( order.auxPrice() == Double.MAX_VALUE ? 0 : order.auxPrice());
            }
            else {
                b.sendMax( order.auxPrice());
            }

            // send extended order fields
            b.send( order.getTif());
            b.send( order.ocaGroup());
            b.send( order.account());
            b.send( order.openClose());
            b.send( order.origin());
            b.send( order.orderRef());
            b.send( order.transmit());
            if( m_serverVersion >= 4 ) {
                b.send (order.parentId());
            }

            if( m_serverVersion >= 5 ) {
                b.send (order.blockOrder());
                b.send (order.sweepToFill());
                b.send (order.displaySize());
                b.send (order.getTriggerMethod());
                if (m_serverVersion < 38) {
                	// will never happen
                	b.send(/* order.m_ignoreRth */ false);
                }
                else {
                	b.send (order.outsideRth());
                }
            }

            if(m_serverVersion >= 7 ) {
                b.send(order.hidden());
            }

            // Send combo legs for BAG requests
            if(m_serverVersion >= 8 && SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
                if ( contract.comboLegs() == null ) {
                    b.send( 0);
                }
                else {
                    b.send( contract.comboLegs().size());

                    for( ComboLeg comboLeg : contract.comboLegs() ) {
                        b.send( comboLeg.conid());
                        b.send( comboLeg.ratio());
                        b.send( comboLeg.getAction());
                        b.send( comboLeg.exchange());
                        b.send( comboLeg.getOpenClose());

                        if (m_serverVersion >= MIN_SERVER_VER_SSHORT_COMBO_LEGS) {
                        	b.send( comboLeg.shortSaleSlot());
                        	b.send( comboLeg.designatedLocation());
                        }
                        if (m_serverVersion >= MIN_SERVER_VER_SSHORTX_OLD) {
                            b.send( comboLeg.exemptCode());
                        }
                    }
                }
            }

            // Send order combo legs for BAG requests
            if(m_serverVersion >= MIN_SERVER_VER_ORDER_COMBO_LEGS_PRICE && SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
                if ( order.orderComboLegs() == null ) {
                    b.send( 0);
                }
                else {
                    b.send( order.orderComboLegs().size());

                    for( OrderComboLeg orderComboLeg : order.orderComboLegs() ) {
                        b.sendMax( orderComboLeg.price());
                    }
                }
            }

            if(m_serverVersion >= MIN_SERVER_VER_SMART_COMBO_ROUTING_PARAMS && SecType.BAG.name().equalsIgnoreCase(contract.getSecType())) {
                List<TagValue> smartComboRoutingParams = order.smartComboRoutingParams();
                int smartComboRoutingParamsCount = smartComboRoutingParams == null ? 0 : smartComboRoutingParams.size();
                b.send( smartComboRoutingParamsCount);
                if( smartComboRoutingParamsCount > 0) {
                    for( TagValue tagValue : smartComboRoutingParams ) {
                        b.send( tagValue.m_tag);
                        b.send( tagValue.m_value);
                    }
                }
            }

            if ( m_serverVersion >= 9 ) {
            	// send deprecated sharesAllocation field
                b.send( "");
            }

            if ( m_serverVersion >= 10 ) {
                b.send( order.discretionaryAmt());
            }

            if ( m_serverVersion >= 11 ) {
                b.send( order.goodAfterTime());
            }

            if ( m_serverVersion >= 12 ) {
                b.send( order.goodTillDate());
            }

            if ( m_serverVersion >= 13 ) {
               b.send( order.faGroup());
               b.send( order.getFaMethod());
               b.send( order.faPercentage());
               if ( m_serverVersion < MIN_SERVER_VER_FA_PROFILE_DESUPPORT ) {
                   b.send( ""); // send deprecated faProfile field
               }
           }

           if ( m_serverVersion >= MIN_SERVER_VER_MODELS_SUPPORT ) {
               b.send( order.modelCode());
           }

           if (m_serverVersion >= 18) { // institutional short sale slot fields.
               b.send( order.shortSaleSlot());      // 0 only for retail, 1 or 2 only for institution.
               b.send( order.designatedLocation()); // only populate when order.m_shortSaleSlot = 2.
           }
           if (m_serverVersion >= MIN_SERVER_VER_SSHORTX_OLD) {
               b.send( order.exemptCode());
           }
           if (m_serverVersion >= 19) {
               b.send( order.getOcaType());
               if (m_serverVersion < 38) {
            	   // will never happen
            	   b.send( /* order.m_rthOnly */ false);
               }
               b.send( order.getRule80A());
               b.send( order.settlingFirm());
               b.send( order.allOrNone());
               b.sendMax( order.minQty());
               b.sendMax( order.percentOffset());
               b.send( false); 
               b.send( false);
               b.sendMax( Double.MAX_VALUE);
               b.sendMax( order.auctionStrategy());
               b.sendMax( order.startingPrice());
               b.sendMax( order.stockRefPrice());
               b.sendMax( order.delta());
        	   // Volatility orders had specific watermark price attribs in server version 26
        	   double lower = (m_serverVersion == 26 && Util.IsVolOrder(order.orderType()))
        	   		? Double.MAX_VALUE
        	   		: order.stockRangeLower();
        	   double upper = (m_serverVersion == 26 && Util.IsVolOrder(order.orderType()))
   	   				? Double.MAX_VALUE
   	   				: order.stockRangeUpper();
               b.sendMax( lower);
               b.sendMax( upper);
           }

           if (m_serverVersion >= 22) {
               b.send( order.overridePercentageConstraints());
           }

           if (m_serverVersion >= 26) { // Volatility orders
               b.sendMax( order.volatility());
               b.send(order.getVolatilityType());
               if (m_serverVersion < 28) {
            	   b.send( order.getDeltaNeutralOrderType().equalsIgnoreCase("MKT"));
               } else {
            	   b.send( order.getDeltaNeutralOrderType());
            	   b.sendMax( order.deltaNeutralAuxPrice());

                   if (m_serverVersion >= MIN_SERVER_VER_DELTA_NEUTRAL_CONID && !IsEmpty(order.getDeltaNeutralOrderType())){
                       b.send( order.deltaNeutralConId());
                       b.send( order.deltaNeutralSettlingFirm());
                       b.send( order.deltaNeutralClearingAccount());
                       b.send( order.deltaNeutralClearingIntent());
                   }

                   if (m_serverVersion >= MIN_SERVER_VER_DELTA_NEUTRAL_OPEN_CLOSE && !IsEmpty(order.getDeltaNeutralOrderType())){
                       b.send( order.deltaNeutralOpenClose());
                       b.send( order.deltaNeutralShortSale());
                       b.send( order.deltaNeutralShortSaleSlot());
                       b.send( order.deltaNeutralDesignatedLocation());
                   }
               }
               b.send( order.continuousUpdate());
               if (m_serverVersion == 26) {
            	   // Volatility orders had specific watermark price attribs in server version 26
            	   double lower = Util.IsVolOrder(order.orderType()) ? order.stockRangeLower() : Double.MAX_VALUE;
            	   double upper = Util.IsVolOrder(order.orderType()) ? order.stockRangeUpper() : Double.MAX_VALUE;
                   b.sendMax( lower);
                   b.sendMax( upper);
               }
               b.send(order.getReferencePriceType());
           }

           if (m_serverVersion >= 30) { // TRAIL_STOP_LIMIT stop price
               b.sendMax( order.trailStopPrice());
           }

           if( m_serverVersion >= MIN_SERVER_VER_TRAILING_PERCENT){
               b.sendMax( order.trailingPercent());
           }

           if (m_serverVersion >= MIN_SERVER_VER_SCALE_ORDERS) {
        	   if (m_serverVersion >= MIN_SERVER_VER_SCALE_ORDERS2) {
        		   b.sendMax (order.scaleInitLevelSize());
        		   b.sendMax (order.scaleSubsLevelSize());
        	   }
        	   else {
        		   b.send ("");
        		   b.sendMax (order.scaleInitLevelSize());

        	   }
        	   b.sendMax (order.scalePriceIncrement());
           }

           if (m_serverVersion >= MIN_SERVER_VER_SCALE_ORDERS3 && order.scalePriceIncrement() > 0.0 && order.scalePriceIncrement() != Double.MAX_VALUE) {
               b.sendMax (order.scalePriceAdjustValue());
               b.sendMax (order.scalePriceAdjustInterval());
               b.sendMax (order.scaleProfitOffset());
               b.send (order.scaleAutoReset());
               b.sendMax (order.scaleInitPosition());
               b.sendMax (order.scaleInitFillQty());
               b.send (order.scaleRandomPercent());
           }

           if (m_serverVersion >= MIN_SERVER_VER_SCALE_TABLE) {
               b.send (order.scaleTable());
               b.send (order.activeStartTime());
               b.send (order.activeStopTime());
           }

           if (m_serverVersion >= MIN_SERVER_VER_HEDGE_ORDERS) {
        	   b.send (order.getHedgeType());
               if (!IsEmpty(order.getHedgeType())) {
        		   b.send (order.hedgeParam());
        	   }
           }

           if (m_serverVersion >= MIN_SERVER_VER_OPT_OUT_SMART_ROUTING) {
               b.send (order.optOutSmartRouting());
           }

           if (m_serverVersion >= MIN_SERVER_VER_PTA_ORDERS) {
        	   b.send (order.clearingAccount());
        	   b.send (order.clearingIntent());
           }

           if (m_serverVersion >= MIN_SERVER_VER_NOT_HELD) {
        	   b.send (order.notHeld());
           }

           if (m_serverVersion >= MIN_SERVER_VER_DELTA_NEUTRAL) {
        	   if (contract.deltaNeutralContract() != null) {
        		   DeltaNeutralContract deltaNeutralContract = contract.deltaNeutralContract();
        		   b.send( true);
        		   b.send( deltaNeutralContract.conid());
        		   b.send( deltaNeutralContract.delta());
        		   b.send( deltaNeutralContract.price());
        	   }
        	   else {
        		   b.send( false);
        	   }
           }

           if (m_serverVersion >= MIN_SERVER_VER_ALGO_ORDERS) {
        	   b.send( order.getAlgoStrategy());
               if( !IsEmpty(order.getAlgoStrategy())) {
        		   List<TagValue> algoParams = order.algoParams();
        		   int algoParamsCount = algoParams.size();
        		   b.send( algoParamsCount);
        		   for( TagValue tagValue : algoParams ) {
                       b.send( tagValue.m_tag);
                       b.send( tagValue.m_value);
        		   }
        	   }
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_ALGO_ID) {
        	   b.send(order.algoId());
           }

           if (m_serverVersion >= MIN_SERVER_VER_WHAT_IF_ORDERS) {
        	   b.send (order.whatIf());
           }
           
           // send orderMiscOptions parameter
           if(m_serverVersion >= MIN_SERVER_VER_LINKING) {
               b.send(order.orderMiscOptions());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_ORDER_SOLICITED) {
        	   b.send(order.solicited());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_RANDOMIZE_SIZE_AND_PRICE) {
        	   b.send(order.randomizeSize());
        	   b.send(order.randomizePrice());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_PEGGED_TO_BENCHMARK) {
        	   if (Util.IsPegBenchOrder(order.orderType())) {
        		   b.send(order.referenceContractId());
        		   b.send(order.isPeggedChangeAmountDecrease());
        		   b.send(order.peggedChangeAmount());
        		   b.send(order.referenceChangeAmount());
        		   b.send(order.referenceExchangeId());
        	   }
        	   
        	   b.send(order.conditions().size());
        	           	   
        	   if (order.conditions().size() > 0) {
        		   for (OrderCondition item : order.conditions()) {
        			   b.send(item.type().val());
        			   item.writeTo(b);
        		   }
        		   
        		   b.send(order.conditionsIgnoreRth());
        		   b.send(order.conditionsCancelOrder());
        	   }
        	   
        	   b.send(order.adjustedOrderType());
        	   b.send(order.triggerPrice());
        	   b.send(order.lmtPriceOffset());
        	   b.send(order.adjustedStopPrice());
        	   b.send(order.adjustedStopLimitPrice());
        	   b.send(order.adjustedTrailingAmount());
        	   b.send(order.adjustableTrailingUnit());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_EXT_OPERATOR) {
        	   b.send(order.extOperator());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_SOFT_DOLLAR_TIER) {
        	   SoftDollarTier tier = order.softDollarTier();
        	   
        	   b.send(tier.name());
        	   b.send(tier.value());
           }           

           if (m_serverVersion >= MIN_SERVER_VER_CASH_QTY) {
               b.sendMax(order.cashQty());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_DECISION_MAKER) {
               b.send(order.mifid2DecisionMaker());
               b.send(order.mifid2DecisionAlgo());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_MIFID_EXECUTION) {
               b.send(order.mifid2ExecutionTrader());
               b.send(order.mifid2ExecutionAlgo());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_AUTO_PRICE_FOR_HEDGE) {
               b.send(order.dontUseAutoPriceForHedge());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_ORDER_CONTAINER) {
               b.send(order.isOmsContainer());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_D_PEG_ORDERS) {
               b.send(order.discretionaryUpToLimitPrice());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_PRICE_MGMT_ALGO) {
               b.send(order.usePriceMgmtAlgo());
           }

           if (m_serverVersion >= MIN_SERVER_VER_DURATION) {
               b.send(order.duration());
           }

           if (m_serverVersion >= MIN_SERVER_VER_POST_TO_ATS) {
               b.send(order.postToAts());
           }

           if (m_serverVersion >= MIN_SERVER_VER_AUTO_CANCEL_PARENT) {
               b.send(order.autoCancelParent());
           }
           
           if (m_serverVersion >= MIN_SERVER_VER_ADVANCED_ORDER_REJECT) {
               b.send(order.advancedErrorOverride());
           }

           if (m_serverVersion >= MIN_SERVER_VER_MANUAL_ORDER_TIME) {
               b.send(order.manualOrderTime());
           }

           if (m_serverVersion >= MIN_SERVER_VER_PEGBEST_PEGMID_OFFSETS) {
               if (contract.exchange().equals("IBKRATS")) {
                   b.sendMax(order.minTradeQty());
               }
               boolean sendMidOffsets = false;
               if (Util.IsPegBestOrder(order.orderType())) {
                   b.sendMax(order.minCompeteSize());
                   b.sendMax(order.competeAgainstBestOffset());
                   if (order.isCompeteAgainstBestOffsetUpToMid()) {
                       sendMidOffsets = true;
                   }
               } else if (Util.IsPegMidOrder(order.orderType())) {
                   sendMidOffsets = true;
               }
               if (sendMidOffsets) {
                   b.sendMax(order.midOffsetAtWhole());
                   b.sendMax(order.midOffsetAtHalf());
               }
           }

           if (m_serverVersion >= MIN_SERVER_VER_CUSTOMER_ACCOUNT) {
               b.send(order.customerAccount());
           }

           if (m_serverVersion >= MIN_SERVER_VER_PROFESSIONAL_CUSTOMER) {
               b.send(order.professionalCustomer());
           }

           if (m_serverVersion >= MIN_SERVER_VER_RFQ_FIELDS && m_serverVersion < MIN_SERVER_VER_UNDO_RFQ_FIELDS) {
               b.send("");
               b.send(Integer.MAX_VALUE);
           }

           if (m_serverVersion >= MIN_SERVER_VER_INCLUDE_OVERNIGHT) {
               b.send(order.includeOvernight());
           }

           if (m_serverVersion >= MIN_SERVER_VER_CME_TAGGING_FIELDS) {
               b.send(order.manualOrderIndicator());
           }

           if (m_serverVersion >= MIN_SERVER_VER_IMBALANCE_ONLY) {
               b.send(order.imbalanceOnly());
           }

           closeAndSend(b);
        }
        catch(EClientException e) {
            error(id, e.error(), e.text());
        }
        catch(Exception e) {
            error(id, EClientErrors.FAIL_SEND_ORDER, e.toString());
            close();
        }
    }

    public synchronized void reqAccountUpdates(boolean subscribe, String acctCode) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 2;

        // send account data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_ACCOUNT_DATA);
            b.send( VERSION);
            b.send( subscribe);

            // Send the account code. This will only be used for FA clients
            if ( m_serverVersion >= 9 ) {
                b.send( acctCode);
            }
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_ACCT, e.toString());
            close();
        }
    }

    public synchronized void reqExecutionsProtoBuf(ExecutionRequestProto.ExecutionRequest executionRequestProto) {
        if (executionRequestProto == null) {
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        int reqId = executionRequestProto.hasReqId() ? executionRequestProto.getReqId() : EClientErrors.NO_VALID_ID;

        // send req executions msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_EXECUTIONS + PROTOBUF_MSG_ID);

            // send execution request
            byte[] byteArray = executionRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_EXEC, e.toString());
            close();
        }
    }

    
    public synchronized void reqExecutions(int reqId, ExecutionFilter filter) {
        if (useProtoBuf(REQ_EXECUTIONS)) {
            reqExecutionsProtoBuf(EClientUtils.createExecutionRequestProto(reqId, filter));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_PARAMETRIZED_DAYS_OF_EXECUTIONS) {
            if (filter.lastNDays() != Integer.MAX_VALUE || (filter.specificDates() != null && !filter.specificDates().isEmpty())) {
                error(reqId, EClientErrors.UPDATE_TWS, "  It does not support last N days and specific dates parameters");
                return;
            }
        }

        final int VERSION = 3;

        // send executions msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_EXECUTIONS);
            b.send( VERSION);

            if (m_serverVersion >= MIN_SERVER_VER_EXECUTION_DATA_CHAIN) {
            	b.send( reqId);
            }

            // Send the execution rpt filter data
            if ( m_serverVersion >= 9 ) {
                b.send( filter.clientId());
                b.send( filter.acctCode());

                // Note that the valid format for m_time is "yyyyMMdd-HH:mm:ss" (UTC) or "yyyyMMdd HH:mm:ss timezone"
                b.send( filter.time());
                b.send( filter.symbol());
                b.send( filter.secType());
                b.send( filter.exchange());
                b.send( filter.side());

                if (m_serverVersion >= MIN_SERVER_VER_PARAMETRIZED_DAYS_OF_EXECUTIONS) {
                    b.send( filter.lastNDays());
                    if (filter.specificDates() != null && !filter.specificDates().isEmpty()) {
	                    b.send( filter.specificDates().size());
	                    for (Integer specificDate : filter.specificDates()) {
	                        b.send( specificDate);
	                    }
                    } else {
                        b.send( 0);
                    }
                }
            }
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_EXEC, e.toString());
            close();
        }
    }

    public synchronized void cancelOrderProtoBuf(CancelOrderRequestProto.CancelOrderRequest cancelOrderRequestProto) {
        if (cancelOrderRequestProto == null) {
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        int orderId = cancelOrderRequestProto.hasOrderId() ? cancelOrderRequestProto.getOrderId() : Integer.MAX_VALUE;

        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, CANCEL_ORDER + PROTOBUF_MSG_ID);

            byte[] byteArray = cancelOrderRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(orderId, e.error(), e.text());
        }
        catch(Exception e) {
            error(orderId, EClientErrors.FAIL_SEND_CORDER, e.toString());
            close();
        }
    }

    public synchronized void cancelOrder( int id, OrderCancel orderCancel) {
        if (useProtoBuf(CANCEL_ORDER)) {
            cancelOrderProtoBuf(EClientUtils.createCancelOrderRequestProto(id, orderCancel));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MANUAL_ORDER_TIME) {
            if (!IsEmpty(orderCancel.manualOrderCancelTime())) {
                error(id, EClientErrors.UPDATE_TWS, "  It does not support manual order cancel time attribute");
                return;
            }
        }

        if (m_serverVersion < MIN_SERVER_VER_CME_TAGGING_FIELDS) {
            if (!IsEmpty(orderCancel.extOperator()) || orderCancel.manualOrderIndicator() != Integer.MAX_VALUE) {
                error(id, EClientErrors.UPDATE_TWS, "  It does not support ext operator and manual order indicator parameters");
                return;
            }
        }

        final int VERSION = 1;

        // send cancel order msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_ORDER);
            if (m_serverVersion < MIN_SERVER_VER_CME_TAGGING_FIELDS) {
                b.send( VERSION);
            }
            b.send( id);

            if (m_serverVersion >= MIN_SERVER_VER_MANUAL_ORDER_TIME) {
                b.send(orderCancel.manualOrderCancelTime());
            }

            if (m_serverVersion >= MIN_SERVER_VER_RFQ_FIELDS && m_serverVersion < MIN_SERVER_VER_UNDO_RFQ_FIELDS) {
                b.send("");
                b.send("");
                b.send(Integer.MAX_VALUE);
            }

            if (m_serverVersion >= MIN_SERVER_VER_CME_TAGGING_FIELDS) {
                b.send(orderCancel.extOperator());
                b.send(orderCancel.manualOrderIndicator());
            }

            closeAndSend(b);
        }
        catch( EClientException e) {
            error( id, e.error(), e.text());
        }
        catch( Exception e) {
            error( id, EClientErrors.FAIL_SEND_CORDER, e.toString());
            close();
        }
    }

    public synchronized void reqOpenOrders() {
        if (useProtoBuf(REQ_OPEN_ORDERS)) {
            reqOpenOrdersProtoBuf(EClientUtils.createOpenOrdersRequestProto());
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send open orders msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_OPEN_ORDERS);
            b.send( VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public synchronized void reqOpenOrdersProtoBuf(OpenOrdersRequestProto.OpenOrdersRequest openOrdersRequest) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // send req open orders msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_OPEN_ORDERS + PROTOBUF_MSG_ID);

            // send open orders request
            byte[] byteArray = openOrdersRequest.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public synchronized void reqIds( int numIds) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_IDS);
            b.send( VERSION);
            b.send( numIds);

            closeAndSend(b);
       }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CORDER, e.toString());
            close();
        }
    }

    public synchronized void reqNewsBulletins( boolean allMsgs) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_NEWS_BULLETINS);
            b.send( VERSION);
            b.send( allMsgs);

            closeAndSend(b);
       }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CORDER, e.toString());
            close();
        }
    }

    public synchronized void cancelNewsBulletins() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send cancel news bulletins msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_NEWS_BULLETINS);
            b.send( VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CORDER, e.toString());
            close();
        }
    }

    public synchronized void setServerLogLevel(int logLevel) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

                // send the set server logging level message
                try {
                    Builder b = prepareBuffer(); 

                    sendMsgId(b, SET_SERVER_LOGLEVEL);
                    b.send( VERSION);
                    b.send( logLevel);

                    closeAndSend(b);
               }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_SERVER_LOG_LEVEL, e.toString());
            close();
        }
    }

    public synchronized void reqAutoOpenOrders(boolean bAutoBind) {
        if (useProtoBuf(REQ_AUTO_OPEN_ORDERS)) {
            reqAutoOpenOrdersProtoBuf(EClientUtils.createAutoOpenOrdersRequestProto(bAutoBind));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send req open orders msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_AUTO_OPEN_ORDERS);
            b.send( VERSION);
            b.send( bAutoBind);

            closeAndSend(b);
        }
        catch( Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public synchronized void reqAutoOpenOrdersProtoBuf(AutoOpenOrdersRequestProto.AutoOpenOrdersRequest autoOpenOrdersRequest) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // send req auto open orders msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_AUTO_OPEN_ORDERS + PROTOBUF_MSG_ID);

            // send auto open orders request
            byte[] byteArray = autoOpenOrdersRequest.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public synchronized void reqAllOpenOrders() {
        if (useProtoBuf(REQ_ALL_OPEN_ORDERS)) {
            reqAllOpenOrdersProtoBuf(EClientUtils.createAllOpenOrdersRequestProto());
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send req all open orders msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_ALL_OPEN_ORDERS);
            b.send( VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }
    
    public synchronized void reqAllOpenOrdersProtoBuf(AllOpenOrdersRequestProto.AllOpenOrdersRequest allOpenOrdersRequest) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // send req all open orders msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_ALL_OPEN_ORDERS + PROTOBUF_MSG_ID);

            // send all open orders request
            byte[] byteArray = allOpenOrdersRequest.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public synchronized void reqManagedAccts() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        final int VERSION = 1;

        // send req FA managed accounts msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_MANAGED_ACCTS);
            b.send( VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_OORDER, e.toString());
            close();
        }
    }

    public void requestFA( Types.FADataType faDataType ) {
        requestFA(faDataType.id());
    }

    public synchronized void requestFA( int faDataType ) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion >= MIN_SERVER_VER_FA_PROFILE_DESUPPORT && faDataType == 2) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FA_PROFILE_NOT_SUPPORTED.code(),
                    EClientErrors.FA_PROFILE_NOT_SUPPORTED.msg());
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_FA);
            b.send( VERSION);
            b.send( faDataType);

            closeAndSend(b);
        }
        catch(Exception e) {
            error( faDataType, EClientErrors.FAIL_SEND_FA_REQUEST, e.toString());
            close();
        }
    }

    public void replaceFA ( int reqId, Types.FADataType faDataType, String xml) {
        replaceFA (reqId, faDataType.id(), xml);
    }

    public synchronized void replaceFA( int reqId, int faDataType, String xml ) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion >= MIN_SERVER_VER_FA_PROFILE_DESUPPORT && faDataType == 2) {
            error(reqId, EClientErrors.FA_PROFILE_NOT_SUPPORTED.code(),
                    EClientErrors.FA_PROFILE_NOT_SUPPORTED.msg());
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REPLACE_FA);
            b.send( VERSION);
            b.send( faDataType);
            b.send( xml);
            if(m_serverVersion >= MIN_SERVER_VER_REPLACE_FA_END) {
                b.send(reqId);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_FA_REPLACE, e.toString());
            close();
        }
    }

    public synchronized void reqCurrentTime() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // This feature is only available for versions of TWS >= 33
        if( m_serverVersion < 33) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                  "  It does not support current time requests.");
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_CURRENT_TIME);
            b.send( VERSION);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQCURRTIME, e.toString());
            close();
        }
    }

    public synchronized void reqFundamentalData(int reqId, Contract contract, String reportType,
            //reserved for future use, must be blank
            List<TagValue> fundamentalDataOptions) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_FUNDAMENTAL_DATA) {
        	error( reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support fundamental data requests.");
        	return;
        }

        if( m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if( contract.conid() > 0) {
                  error(reqId, EClientErrors.UPDATE_TWS,
                      "  It does not support conId parameter in reqFundamentalData.");
                  return;
            }
        }

        final int VERSION = 2;

        try {
            // send req fund data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_FUNDAMENTAL_DATA);
            b.send(VERSION);
            b.send(reqId);

            // send contract fields
            if(m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.conid());
            }
            
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.exchange());
            b.send(contract.primaryExch());
            b.send(contract.currency());
            b.send(contract.localSymbol());

            b.send(reportType);
            
            if (m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(fundamentalDataOptions);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQFUNDDATA, e.toString());
            close();
        }
    }

    public synchronized void cancelFundamentalData(int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_FUNDAMENTAL_DATA) {
        	error( reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support fundamental data requests.");
        	return;
        }

        final int VERSION = 1;

        try {
            // send cancel fundamental data msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_FUNDAMENTAL_DATA);
            b.send( VERSION);
            b.send( reqId);

            closeAndSend(b);
       }
        catch( Exception e) {
            error( reqId, EClientErrors.FAIL_SEND_CANFUNDDATA, e.toString());
            close();
        }
    }

    public synchronized void calculateImpliedVolatility(int reqId, Contract contract,
            double optionPrice, double underPrice,
            //reserved for future use, must be blank
            List<TagValue> impliedVolatilityOptions) {

        // not connected?
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_CALC_IMPLIED_VOLAT) {
            error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support calculate implied volatility requests.");
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass())) {
                  error(reqId, EClientErrors.UPDATE_TWS,
                      "  It does not support tradingClass parameter in calculateImpliedVolatility.");
                  return;
            }
        }

        final int VERSION = 2;

        try {
            // send calculate implied volatility msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_CALC_IMPLIED_VOLAT);
            b.send(VERSION);
            b.send(reqId);

            // send contract fields
            b.send(contract.conid());
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            b.send(contract.multiplier());
            b.send(contract.exchange());
            b.send(contract.primaryExch());
            b.send(contract.currency());
            b.send(contract.localSymbol());
            
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }

            b.send(optionPrice);
            b.send(underPrice);
            
            if (m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(impliedVolatilityOptions);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQCALCIMPLIEDVOLAT, e.toString());
            close();
        }
    }

    public synchronized void cancelCalculateImpliedVolatility(int reqId) {

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_CANCEL_CALC_IMPLIED_VOLAT) {
            error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support calculate implied volatility cancellation.");
            return;
        }

        final int VERSION = 1;

        try {
            // send cancel calculate implied volatility msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_CALC_IMPLIED_VOLAT);
            b.send( VERSION);
            b.send( reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( reqId, EClientErrors.FAIL_SEND_CANCALCIMPLIEDVOLAT, e.toString());
            close();
        }
    }

    public synchronized void calculateOptionPrice(int reqId, Contract contract,
            double volatility, double underPrice,
            //reserved for future use, must be blank
            List<TagValue> optionPriceOptions) {

        // not connected?
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_CALC_OPTION_PRICE) {
            error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support calculate option price requests.");
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TRADING_CLASS) {
            if (!IsEmpty(contract.tradingClass())) {
                  error(reqId, EClientErrors.UPDATE_TWS,
                      "  It does not support tradingClass parameter in calculateOptionPrice.");
                  return;
            }
        }

        final int VERSION = 2;

        try {
            // send calculate option price msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_CALC_OPTION_PRICE);
            b.send(VERSION);
            b.send(reqId);

            // send contract fields
            b.send(contract.conid());
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            b.send(contract.multiplier());
            b.send(contract.exchange());
            b.send(contract.primaryExch());
            b.send(contract.currency());
            b.send(contract.localSymbol());
            
            if (m_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
                b.send(contract.tradingClass());
            }

            b.send(volatility);
            b.send(underPrice);
            
            if (m_serverVersion >= MIN_SERVER_VER_LINKING) {
                b.send(optionPriceOptions);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQCALCOPTIONPRICE, e.toString());
            close();
        }
    }

    public synchronized void cancelCalculateOptionPrice(int reqId) {

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_CANCEL_CALC_OPTION_PRICE) {
            error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support calculate option price cancellation.");
            return;
        }

        final int VERSION = 1;

        try {
            // send cancel calculate option price msg
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_CALC_OPTION_PRICE);
            b.send( VERSION);
            b.send( reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( reqId, EClientErrors.FAIL_SEND_CANCALCOPTIONPRICE, e.toString());
            close();
        }
    }

    public synchronized void reqGlobalCancelProtoBuf(GlobalCancelRequestProto.GlobalCancelRequest globalCancelRequestProto) {
        if (globalCancelRequestProto == null) {
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_GLOBAL_CANCEL + PROTOBUF_MSG_ID);

            byte[] byteArray = globalCancelRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQGLOBALCANCEL, e.toString());
            close();
        }
    }

    public synchronized void reqGlobalCancel(OrderCancel orderCancel) {
        if (useProtoBuf(REQ_GLOBAL_CANCEL)) {
            reqGlobalCancelProtoBuf(EClientUtils.createGlobalCancelRequestProto(orderCancel));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_GLOBAL_CANCEL) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                    "  It does not support globalCancel requests.");
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_CME_TAGGING_FIELDS) {
            if (!IsEmpty(orderCancel.extOperator()) || orderCancel.manualOrderIndicator() != Integer.MAX_VALUE) {
                error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS, "  It does not support ext operator and manual order indicator parameters");
                return;
            }
        }

        final int VERSION = 1;

        // send request global cancel msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_GLOBAL_CANCEL);
            if (m_serverVersion < MIN_SERVER_VER_CME_TAGGING_FIELDS) {
                b.send( VERSION);
            }

            if (m_serverVersion >= MIN_SERVER_VER_CME_TAGGING_FIELDS) {
                b.send(orderCancel.extOperator());
                b.send(orderCancel.manualOrderIndicator());
            }

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQGLOBALCANCEL, e.toString());
            close();
        }
    }

    public synchronized void reqMarketDataType(int marketDataType) {
        if (useProtoBuf(REQ_MARKET_DATA_TYPE)) {
            reqMarketDataTypeProtoBuf(EClientUtils.createMarketDataTypeRequestProto(marketDataType));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_MARKET_DATA_TYPE) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                    "  It does not support marketDataType requests.");
            return;
        }

        final int VERSION = 1;

        // send the reqMarketDataType message
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_MARKET_DATA_TYPE);
            b.send( VERSION);
            b.send( marketDataType);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQMARKETDATATYPE, e.toString());
            close();
        }
    }

    public synchronized void reqMarketDataTypeProtoBuf(MarketDataTypeRequestProto.MarketDataTypeRequest marketDataTypeRequestProto) {
        if (marketDataTypeRequestProto == null) {
            return;
        }

        if( !isConnected()) {
            notConnected();
            return;
        }

        // send req market data type msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_MARKET_DATA_TYPE + PROTOBUF_MSG_ID);

            // send market data type request
            byte[] byteArray = marketDataTypeRequestProto.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQMARKETDATATYPE, e.toString());
            close();
        }
    }

    public synchronized void reqPositions() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_ACCT_SUMMARY) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support position requests.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_POSITIONS);
            b.send( VERSION);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQPOSITIONS, e.toString());
        }
    }
    

	public synchronized void reqSecDefOptParams(int reqId, String underlyingSymbol, String futFopExchange, String underlyingSecType, int underlyingConId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }
        
        if (m_serverVersion < MIN_SERVER_VER_SEC_DEF_OPT_PARAMS_REQ) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support security definition option requests.");
            return;
        }
        
        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_SEC_DEF_OPT_PARAMS);
            b.send(reqId);
            b.send(underlyingSymbol); 
            b.send(futFopExchange);
            b.send(underlyingSecType);
            b.send(underlyingConId);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQSECDEFOPTPARAMS, e.toString());
            close();
        }
	}
	
	public synchronized void reqSoftDollarTiers(int reqId) {
		if (!isConnected()) {
			notConnected();
			return;
		}
		
        if (m_serverVersion < MIN_SERVER_VER_SOFT_DOLLAR_TIER) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support soft dollar tier requests.");
            return;
        }
        
        Builder b = prepareBuffer();
        
        try {
            sendMsgId(b, REQ_SOFT_DOLLAR_TIERS);
            b.send(reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQSOFTDOLLARTIERS, e.toString());
        }
	}

    public synchronized void cancelPositions() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_ACCT_SUMMARY) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support position cancellation.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, CANCEL_POSITIONS);
            b.send( VERSION);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CANPOSITIONS, e.toString());
        }
    }
    
    public synchronized void reqPositionsMulti( int reqId, String account, String modelCode) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MODELS_SUPPORT) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support positions multi request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_POSITIONS_MULTI);
            b.send( VERSION);
            b.send( reqId);
            b.send( account);
            b.send( modelCode);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQPOSITIONSMULTI, e.toString());
            close();
        }
    }    
    
    public synchronized void cancelPositionsMulti( int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MODELS_SUPPORT) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support positions multi cancellation.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, CANCEL_POSITIONS_MULTI);
            b.send( VERSION);
            b.send( reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CANPOSITIONSMULTI, e.toString());
        }
    }
    
	public synchronized void cancelAccountUpdatesMulti( int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MODELS_SUPPORT) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support account updates multi cancellation.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, CANCEL_ACCOUNT_UPDATES_MULTI);
            b.send( VERSION);
            b.send( reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_CANACCOUNTUPDATESMULTI, e.toString());
        }
    }

    public synchronized void reqAccountUpdatesMulti( int reqId, String account, String modelCode, boolean ledgerAndNLV) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MODELS_SUPPORT) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support account updates multi requests.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_ACCOUNT_UPDATES_MULTI);
            b.send( VERSION);
            b.send( reqId);
            b.send( account);
            b.send( modelCode);
            b.send( ledgerAndNLV);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQACCOUNTUPDATESMULTI, e.toString());
            close();
        }
    }
    
    public synchronized void reqAccountSummary( int reqId, String group, String tags) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_ACCT_SUMMARY) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support account summary requests.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_ACCOUNT_SUMMARY);
            b.send( VERSION);
            b.send( reqId);
            b.send( group);
            b.send( tags);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQACCOUNTDATA, e.toString());
            close();
        }
    }

	public synchronized void cancelAccountSummary( int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_ACCT_SUMMARY) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support account summary cancellation.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, CANCEL_ACCOUNT_SUMMARY);
            b.send( VERSION);
            b.send( reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_CANACCOUNTDATA, e.toString());
        }
    }
    public synchronized void verifyRequest( String apiName, String apiVersion) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support verification request.");
            return;
        }

        if (!m_extraAuth) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYMESSAGE,
            "  Intent to authenticate needs to be expressed during initial connect request.");
            return;
        }

        final int VERSION = 1;

        try {
            Builder b = prepareBuffer();
            sendMsgId(b, VERIFY_REQUEST);
            b.send( VERSION);
            b.send( apiName);
            b.send( apiVersion);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch (IOException e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYREQUEST, e.toString());
            close();
        }
    }

    public synchronized void verifyMessage( String apiData) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support verification message sending.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, VERIFY_MESSAGE);
            b.send( VERSION);
            b.send( apiData);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch (IOException e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYMESSAGE, e.toString());
            close();
        }
    }

    public synchronized void verifyAndAuthRequest( String apiName, String apiVersion, String opaqueIsvKey) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_LINKING_AUTH) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support verification request.");
            return;
        }

        if( !m_extraAuth) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYANDAUTHREQUEST,
            "  Intent to authenticate needs to be expressed during initial connect request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, VERIFY_AND_AUTH_REQUEST);
            b.send( VERSION);
            b.send( apiName);
            b.send( apiVersion);
            b.send( opaqueIsvKey);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch (IOException e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYANDAUTHREQUEST, e.toString());
            close();
        }
    }

    public synchronized void verifyAndAuthMessage( String apiData, String xyzResponse) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_LINKING_AUTH) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support verification message sending.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, VERIFY_AND_AUTH_MESSAGE);
            b.send( VERSION);
            b.send( apiData);
            b.send( xyzResponse);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(EClientErrors.NO_VALID_ID, e.error(), e.text());
        }
        catch (IOException e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_VERIFYANDAUTHMESSAGE, e.toString());
            close();
        }
    }

	public synchronized void queryDisplayGroups( int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support queryDisplayGroups request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, QUERY_DISPLAY_GROUPS);
            b.send( VERSION);
            b.send( reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_QUERYDISPLAYGROUPS, e.toString());
        }
    }
	
	public synchronized void subscribeToGroupEvents( int reqId, int groupId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support subscribeToGroupEvents request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, SUBSCRIBE_TO_GROUP_EVENTS);
            b.send( VERSION);
            b.send( reqId);
            b.send( groupId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_SUBSCRIBETOGROUPEVENTS, e.toString());
        }
    }	

	public synchronized void updateDisplayGroup( int reqId, String contractInfo) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support updateDisplayGroup request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, UPDATE_DISPLAY_GROUP);
            b.send( VERSION);
            b.send( reqId);
            b.send( contractInfo);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_UPDATEDISPLAYGROUP, e.toString());
            close();
        }
    }	

	public synchronized void unsubscribeFromGroupEvents( int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_LINKING) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support unsubscribeFromGroupEvents request.");
            return;
        }

        final int VERSION = 1;

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, UNSUBSCRIBE_FROM_GROUP_EVENTS);
            b.send( VERSION);
            b.send( reqId);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_UNSUBSCRIBEFROMGROUPEVENTS, e.toString());
        }
    }	

    public synchronized void reqMatchingSymbols( int reqId, String pattern) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_MATCHING_SYMBOLS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support matching symbols request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_MATCHING_SYMBOLS);
            b.send( reqId);
            b.send( pattern);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQMATCHINGSYMBOLS, e.toString());
            close();
        }
    }	

    public synchronized void reqFamilyCodes() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_FAMILY_CODES) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support family codes request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_FAMILY_CODES);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQFAMILYCODES, e.toString());
        }
    }

    public synchronized void reqMktDepthExchanges() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_MKT_DEPTH_EXCHANGES) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support market depth exchanges request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_MKT_DEPTH_EXCHANGES);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQMKTDEPTHEXCHANGES, e.toString());
        }
    }
    
    public synchronized void reqSmartComponents(int reqId, String bboExchange) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_SMART_COMPONENTS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support smart components request.");
            return;
        }

        Builder b = prepareBuffer();
        
        try {
            sendMsgId(b, REQ_SMART_COMPONENTS);
            b.send(reqId);
            b.send(bboExchange);
            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch (IOException e) {
            error(reqId, EClientErrors.FAIL_SEND_REQSMARTCOMPONENTS, e.toString());
            close();
        }
    }

    public synchronized void reqNewsProviders() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_NEWS_PROVIDERS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support news providers request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_NEWS_PROVIDERS);
            closeAndSend(b);
        }
        catch (IOException e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQNEWSPROVIDERS, e.toString());
        }
    }

    public synchronized void reqNewsArticle(int requestId, String providerCode, String articleId, List<TagValue> newsArticleOptions) {
        // not connected?
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_NEWS_ARTICLE) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support news article request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_NEWS_ARTICLE);
            b.send(requestId);
            b.send(providerCode);
            b.send(articleId);

            // send newsArticleOptions parameter
            if (m_serverVersion >= MIN_SERVER_VER_NEWS_QUERY_ORIGINS) {
                b.send(newsArticleOptions);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(requestId, e.error(), e.text());
        }
        catch (IOException e) {
            error(requestId, EClientErrors.FAIL_SEND_REQNEWSARTICLE, e.toString());
            close();
        }
    }

    public synchronized void reqHistoricalNews( int requestId, int conId, String providerCodes, 
            String startDateTime, String endDateTime, int totalResults, List<TagValue> historicalNewsOptions) {

        // not connected?
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_HISTORICAL_NEWS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
            "  It does not support historical news request.");
            return;
        }

        Builder b = prepareBuffer();

        try {
            sendMsgId(b, REQ_HISTORICAL_NEWS);
            b.send(requestId);
            b.send(conId);
            b.send(providerCodes);
            b.send(startDateTime);
            b.send(endDateTime);
            b.send(totalResults);

            // send historicalNewsOptions parameter
            if (m_serverVersion >= MIN_SERVER_VER_NEWS_QUERY_ORIGINS) {
                b.send(historicalNewsOptions);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(requestId, e.error(), e.text());
        }
        catch(IOException e) {
            error(requestId, EClientErrors.FAIL_SEND_REQHISTORICALNEWS, e.toString());
            close();
        }
    }

    public synchronized void reqHistogramData(int tickerId, Contract contract,
    		boolean useRTH, String timePeriod) {
    	// not connected?
    	if( !isConnected()) {
    		notConnected();
    		return;
    	}

    	try {
    		if (m_serverVersion < MIN_SERVER_VER_REQ_HISTOGRAM) {
    			error(tickerId, EClientErrors.UPDATE_TWS,
    					"  It does not support histogram requests.");
    			return;
    		}

    		Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_HISTOGRAM_DATA);
    		b.send(tickerId);
    		b.send(contract);
    		b.send(useRTH ? 1 : 0);
    		b.send(timePeriod);

    		closeAndSend(b);
        }
        catch(EClientException e) {
            error(tickerId, e.error(), e.text());
        }
        catch (Exception e) {
            error(tickerId, EClientErrors.FAIL_SEND_REQHISTOGRAMDATA, e.toString());
            close();
        }
    }
    
    public synchronized void cancelHistogramData( int tickerId ) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_HISTOGRAM) {
        	error(tickerId, EClientErrors.UPDATE_TWS,
        			"  It does not support head time stamp requests.");
        	return;
        }

        // send cancel mkt data msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_HISTOGRAM_DATA);
            b.send(tickerId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( tickerId, EClientErrors.FAIL_SEND_CANCELHISTOGRAMDATA, e.toString());
            close();
        }
    }

    public synchronized void reqMarketRule( int marketRuleId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_MARKET_RULES) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                    "  It does not support market rule requests.");
            return;
        }

        // send request market rule msg
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_MARKET_RULE);
            b.send(marketRuleId);

            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQMARKETRULE, e.toString());
            close();
        }
    }
    
    public synchronized void reqPnL(int reqId, String account, String modelCode) {
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_PNL) {
        	error(reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support PnL requests.");
        	return;
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_PNL);
            b.send(reqId);
            b.send(account);
            b.send(modelCode);

            closeAndSend(b);
        } 
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQPNL, e.toString());
            close();
        }
    }
    
    public synchronized void cancelPnL(int reqId) {
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_PNL) {
        	error(reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support PnL requests.");
        	return;
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_PNL);
            b.send(reqId);

            closeAndSend(b);
        } 
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_CANCELPNL, e.toString());
            close();
        }
    }

    public synchronized void reqPnLSingle(int reqId, String account, String modelCode, int conId) {
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_PNL) {
        	error(reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support PnL requests.");
        	return;
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_PNL_SINGLE);
            b.send(reqId);
            b.send(account);
            b.send(modelCode);
            b.send(conId);

            closeAndSend(b);
        } 
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQPNLSINGLE, e.toString());
            close();
        }
    }
    
    public synchronized void cancelPnLSingle(int reqId) {
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_PNL) {
        	error(reqId, EClientErrors.UPDATE_TWS,
        			"  It does not support PnL requests.");
        	return;
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_PNL_SINGLE);
            b.send(reqId);

            closeAndSend(b);
        } catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_CANCELPNLSINGLE, e.toString());
            close();
        }
    }
    
    public synchronized void reqHistoricalTicks(int reqId, Contract contract, String startDateTime,
            String endDateTime, int numberOfTicks, String whatToShow, int useRth, boolean ignoreSize,
            List<TagValue> miscOptions) {
        if (!isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_HISTORICAL_TICKS) {
            error(reqId, EClientErrors.UPDATE_TWS,
                    "  It does not support historical ticks request.");
            return;
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_HISTORICAL_TICKS);
            b.send(reqId);
            b.send(contract);
            b.send(startDateTime);
            b.send(endDateTime);
            b.send(numberOfTicks);
            b.send(whatToShow);
            b.send(useRth);
            b.send(ignoreSize);
            b.send(miscOptions);

            closeAndSend(b);
        } 
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQHISTORICALTICKS, e.toString());
            close();
        }        
    }
  
    public synchronized void reqTickByTickData(int reqId, Contract contract, String tickType, int numberOfTicks, boolean ignoreSize) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TICK_BY_TICK) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support tick-by-tick data requests.");
          return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TICK_BY_TICK_IGNORE_SIZE) {
            if (numberOfTicks != 0 || ignoreSize) {
                error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                    "  It does not support ignoreSize and numberOfTicks parameters in tick-by-tick data requests.");
                return;
            }
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_TICK_BY_TICK_DATA);
            b.send(reqId);
            b.send(contract.conid());
            b.send(contract.symbol());
            b.send(contract.getSecType());
            b.send(contract.lastTradeDateOrContractMonth());
            b.sendMax(contract.strike());
            b.send(contract.getRight());
            b.send(contract.multiplier());
            b.send(contract.exchange());
            b.send(contract.primaryExch());
            b.send(contract.currency());
            b.send(contract.localSymbol());
            b.send(contract.tradingClass());
            b.send(tickType);
            if (m_serverVersion >= MIN_SERVER_VER_TICK_BY_TICK_IGNORE_SIZE) {
                b.send(numberOfTicks);
                b.send(ignoreSize);
            }

            closeAndSend(b);
        }
        catch(EClientException e) {
            error(reqId, e.error(), e.text());
        }
        catch(Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQTICKBYTICKDATA, e.toString());
            close();
        }
    }

    public synchronized void cancelTickByTickData(int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_TICK_BY_TICK) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support tick-by-tick data cancels.");
          return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_TICK_BY_TICK_DATA);
            b.send(reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_CANCELTICKBYTICKDATA, e.toString());
            close();
        }
    }

    public synchronized void reqCompletedOrders(boolean apiOnly) {
        if (useProtoBuf(REQ_COMPLETED_ORDERS)) {
            reqCompletedOrdersProtoBuf(EClientUtils.createCompletedOrdersRequestProto(apiOnly));
            return;
        }

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_REQ_COMPLETED_ORDERS) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support completed orders requests.");
          return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_COMPLETED_ORDERS);
            b.send(apiOnly);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_REQCOMPLETEDORDERS, e.toString());
            close();
        }
    }
    
    public synchronized void reqCompletedOrdersProtoBuf(CompletedOrdersRequestProto.CompletedOrdersRequest completedOrdersRequest) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        // send req completed orders msg
        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_COMPLETED_ORDERS + PROTOBUF_MSG_ID);

            // send completed orders request
            byte[] byteArray = completedOrdersRequest.toByteArray();
            b.sendByteArray(byteArray);
            closeAndSend(b);
        }
        catch(Exception e) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQCOMPLETEDORDERS, e.toString());
            close();
        }
    }
    
    
    public synchronized void reqWshMetaData(int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_WSHE_CALENDAR) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support WSHE Calendar API.");
          return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_WSH_META_DATA);
            b.send(reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_REQ_WSH_META_DATA, e.toString());
            close();
        }   	
    }
    
    public synchronized void cancelWshMetaData(int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_WSHE_CALENDAR) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support WSHE Calendar API.");
          return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_WSH_META_DATA);
            b.send(reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_CAN_WSH_META_DATA, e.toString());
            close();
        }   	
    }
    
    public synchronized void reqWshEventData(int reqId, WshEventData wshEventData) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_WSHE_CALENDAR) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support WSHE Calendar API.");
          return;
        }

        if (m_serverVersion < MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS) {
            if (!IsEmpty(wshEventData.filter()) || wshEventData.fillWatchlist() || wshEventData.fillPortfolio() || wshEventData.fillCompetitors()) {
                error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS, "  It does not support WSH event data filters.");
                return;
            }
        }

        if (m_serverVersion < MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS_DATE) {
            if (!IsEmpty(wshEventData.startDate()) || !IsEmpty(wshEventData.endDate()) || wshEventData.totalLimit() != Integer.MAX_VALUE) {
                error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS, "  It does not support WSH event data date filters.");
                return;
            }
        }
        
        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_WSH_EVENT_DATA);
            b.send(reqId);
            b.send(wshEventData.conId());

            if (m_serverVersion >= MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS) {
                b.send(wshEventData.filter());
                b.send(wshEventData.fillWatchlist());
                b.send(wshEventData.fillPortfolio());
                b.send(wshEventData.fillCompetitors());
            }

            if (m_serverVersion >= MIN_SERVER_VER_WSH_EVENT_DATA_FILTERS_DATE) {
                b.send(wshEventData.startDate());
                b.send(wshEventData.endDate());
                b.send(wshEventData.totalLimit());
            }
            
            closeAndSend(b);
        }
        catch( EClientException e) {
            error( reqId, e.error(), e.text());
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_REQ_WSH_EVENT_DATA, e.toString());
            close();
        }   	
    }
    
    public synchronized void cancelWshEventData(int reqId) {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_WSHE_CALENDAR) {
          error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS,
                "  It does not support WSHE Calendar API.");
          return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, CANCEL_WSH_EVENT_DATA);
            b.send(reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID,
                   EClientErrors.FAIL_SEND_CAN_WSH_EVENT_DATA, e.toString());
            close();
        }   	
    }
    
    public synchronized void reqUserInfo(int reqId) {

        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if (m_serverVersion < MIN_SERVER_VER_USER_INFO) {
            error(reqId, EClientErrors.UPDATE_TWS, " It does not support user info requests.");
            return;
        }

        try {
            Builder b = prepareBuffer(); 

            sendMsgId(b, REQ_USER_INFO);
            b.send(reqId);

            closeAndSend(b);
        }
        catch( Exception e) {
            error(reqId, EClientErrors.FAIL_SEND_REQ_USER_INFO, e.toString());
            close();
        }
    }    
    
    public synchronized void reqCurrentTimeInMillis() {
        // not connected?
        if( !isConnected()) {
            notConnected();
            return;
        }

        if( m_serverVersion < MIN_SERVER_VER_CURRENT_TIME_IN_MILLIS) {
            error(EClientErrors.NO_VALID_ID, EClientErrors.UPDATE_TWS, "  It does not support current time in millis requests.");
            return;
        }

        try {
            Builder b = prepareBuffer(); 
            sendMsgId(b, REQ_CURRENT_TIME_IN_MILLIS);
            closeAndSend(b);
        }
        catch( Exception e) {
            error( EClientErrors.NO_VALID_ID, EClientErrors.FAIL_SEND_REQCURRTIMEINMILLIS, e.toString());
            close();
        }
    }    
    /**
     * @deprecated This method is never called.
     */
    @Deprecated
    protected synchronized void error( String err) {
        m_eWrapper.error( err);
    }

    protected synchronized void error( int id, int errorCode, String errorMsg) {
        m_eWrapper.error( id, Util.currentTimeMillis(), errorCode, errorMsg, null);
    }

    protected void close() {
        eDisconnect();
        wrapper().connectionClosed();
    }

    protected void error(int id, EClientErrors.CodeMsgPair pair, String tail) {
        error(id, pair.code(), pair.msg() + tail);
    }

    protected abstract Builder prepareBuffer();
    
    protected abstract void closeAndSend(Builder buf) throws IOException;
    
    
    protected void validateInvalidSymbols(String host) throws EClientException {
        if (host != null && !Builder.isAsciiPrintable(host)) {
            throw new EClientException(EClientErrors.INVALID_SYMBOL, host);
        }

        if (m_connectOptions != null && !Builder.isAsciiPrintable(m_connectOptions)) {
            throw new EClientException(EClientErrors.INVALID_SYMBOL, m_connectOptions);
        }

        if (m_optionalCapabilities != null && !Builder.isAsciiPrintable(m_optionalCapabilities)) {
            throw new EClientException(EClientErrors.INVALID_SYMBOL, m_optionalCapabilities);
        }
    }

    private void sendV100APIHeader() throws IOException {
    	try (Builder builder = new Builder(1024)) {
            builder.send("API\0".getBytes(StandardCharsets.UTF_8));

            String out = buildVersionString(MIN_VERSION, MAX_VERSION);

            if (!IsEmpty(m_connectOptions)) {
                out += " " + m_connectOptions;
            }

            int lengthPos = builder.allocateLengthHeader();
            builder.send(out.getBytes(StandardCharsets.UTF_8));
            builder.updateLength(lengthPos);

            sendMsg(new EMessage(builder));
        }
    }

    private String buildVersionString(int minVersion, int maxVersion) {
      return "v" + ((minVersion < maxVersion)
          ? minVersion + ".." + maxVersion : minVersion);
    }

    protected void sendMsg(EMessage msg) throws IOException {
    	m_socketTransport.send(msg);
    }

    private static boolean IsEmpty(String str) {
    	return Util.StringIsEmpty(str);
    }

    protected void notConnected() {
        error(EClientErrors.NO_VALID_ID, EClientErrors.NOT_CONNECTED, "");
    }

    public String connectedHost()        { return m_host; } // Host that was connected
    protected void send( int val) throws IOException {
        send( String.valueOf( val) );
    }
	// Sends String without length prefix (pre-V100 style)
	protected void send( String str) throws IOException {
		// Write string to data buffer
		try (Builder builder = new Builder( 1024 )) {
            builder.send(str);
            sendMsg(new EMessage(builder));
        }
	}

    private void sendMsgId(Builder b, int msgId) throws IOException {
        if (m_serverVersion >= MIN_SERVER_VER_PROTOBUF) {
            b.sendRawInt(msgId);
        } else {
            b.send(msgId);
        }
    }

}
