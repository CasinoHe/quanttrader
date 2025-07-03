#pragma once

#include "broker_consts.h"
#include <memory>
#include <string>
#include <functional>
#include <optional>
#include "Decimal.h"

namespace quanttrader {
namespace broker {

// Forward declarations
struct BarData {
    uint64_t time;
    uint64_t end_time;
    double open;
    double high;
    double low;
    double close;
    Decimal volume;
    Decimal wap;
    int count;
    bool is_last;
};

// Callback types
using BarDataCallback = std::function<void(const BarData&)>;
using TradeCallback = std::function<void(const std::string& symbol, double price, Decimal size)>;
using OrderStatusCallback = std::function<void(long orderId, const std::string& status, Decimal filled, Decimal remaining)>;
using ErrorCallback = std::function<void(int code, const std::string& message)>;

/**
 * Abstract class representing a broker provider interface.
 * This serves as a common interface for all broker implementations.
 */
class BrokerProvider {
public:
    virtual ~BrokerProvider() = default;

    // Connection methods
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // Data request methods
    virtual long requestHistoricalData(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency,
        const std::string& endTime, 
        const std::string& duration,
        const std::string& barSize,
        const std::string& whatToShow,
        bool useRTH,
        bool keepUpToDate) = 0;
    
    virtual long requestRealTimeData(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency) = 0;
    
    virtual void cancelHistoricalData(long requestId) = 0;
    virtual void cancelRealTimeData(long requestId) = 0;
    
    // Trading methods
    virtual long placeOrder(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency,
        const std::string& action,
        double quantity,
        double price,
        const std::string& orderType) = 0;
    
    virtual void cancelOrder(long orderId) = 0;
    
    // Callback registration methods
    virtual void registerBarDataCallback(long requestId, BarDataCallback callback) = 0;
    virtual void registerTradeCallback(TradeCallback callback) = 0;
    virtual void registerOrderStatusCallback(OrderStatusCallback callback) = 0;
    virtual void registerErrorCallback(ErrorCallback callback) = 0;
    
    // Utility methods
    virtual void requestCurrentTime() = 0;
    virtual long getNextRequestId() = 0;
};

} // namespace broker
} // namespace quanttrader