/* Copyright (C) 2025 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TWS_API_CLIENT_ECLIENT_UTILS_H
#define TWS_API_CLIENT_ECLIENT_UTILS_H

#include "Contract.h"
#include "Execution.h"
#include "Order.h"
#include "OrderCancel.h"
#include "Order.pb.h"
#include "CancelOrderRequest.pb.h"
#include "GlobalCancelRequest.pb.h"
#include "ExecutionRequest.pb.h"
#include "OrderCancel.pb.h"
#include "PlaceOrderRequest.pb.h"
#include "AllOpenOrdersRequest.pb.h"
#include "AutoOpenOrdersRequest.pb.h"
#include "OpenOrdersRequest.pb.h"
#include "CompletedOrdersRequest.pb.h"
#include "ContractDataRequest.pb.h"
#include "MarketDataRequest.pb.h"
#include "MarketDepthRequest.pb.h"
#include "MarketDataTypeRequest.pb.h"
#include "CancelMarketData.pb.h"
#include "CancelMarketDepth.pb.h"

class EClientUtils {

public:
	static protobuf::ExecutionRequest createExecutionRequestProto(int reqId, const ExecutionFilter& filter);
	static protobuf::PlaceOrderRequest createPlaceOrderRequestProto(OrderId id, const Contract& contract, const Order& order);
	static protobuf::Order createOrderProto(const Order& order);
	static std::list<protobuf::OrderCondition> createConditionsProto(Order order);
	static protobuf::OrderCondition createOrderConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createOperatorConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createContractConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createPriceConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createTimeConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createMarginConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createExecutionConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createVolumeConditionProto(OrderCondition& condition);
	static protobuf::OrderCondition createPercentChangeConditionProto(OrderCondition& condition);
	static protobuf::SoftDollarTier createSoftDollarTierProto(Order order);
	static std::map<std::string, std::string> createStringStringMap(TagValueListSPtr tagValueListSPtr);
	static protobuf::Contract createContractProto(const Contract& contract, const Order& order);
	static protobuf::DeltaNeutralContract* createDeltaNeutralContractProto(const Contract& contract);
	static std::list<protobuf::ComboLeg> createComboLegProtoList(const Contract& contract, const Order& order);
	static protobuf::ComboLeg createComboLegProto(const ComboLeg& comboLeg, double perLegPrice);
	static protobuf::CancelOrderRequest createCancelOrderRequestProto(OrderId id, const OrderCancel& orderCancel);
	static protobuf::GlobalCancelRequest createGlobalCancelRequestProto(const OrderCancel& orderCancel);
	static protobuf::OrderCancel createOrderCancelProto(const OrderCancel& orderCancel);
	static protobuf::AllOpenOrdersRequest createAllOpenOrdersRequestProto();
	static protobuf::AutoOpenOrdersRequest createAutoOpenOrdersRequestProto(bool autoBind);
	static protobuf::OpenOrdersRequest createOpenOrdersRequestProto();
	static protobuf::CompletedOrdersRequest createCompletedOrdersRequestProto(bool apiOnly);
	static protobuf::ContractDataRequest createContractDataRequestProto(int reqId, const Contract& contract);
	static protobuf::MarketDataRequest createMarketDataRequestProto(int reqId, const Contract& contract, const std::string& genericTickList, bool snapshot, bool regulatorySnapshot, const TagValueListSPtr marketDataOptionsList);
	static protobuf::MarketDepthRequest createMarketDepthRequestProto(int reqId, const Contract& contract, int numRows, bool isSmartDepth, const TagValueListSPtr marketDepthOptionsList);
	static protobuf::MarketDataTypeRequest createMarketDataTypeRequestProto(int marketDataType);
	static protobuf::CancelMarketData createCancelMarketDataProto(int reqId);
	static protobuf::CancelMarketDepth createCancelMarketDepthProto(int reqId, bool isSmartDepth);
};

#endif

