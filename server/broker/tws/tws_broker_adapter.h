#pragma once

#include "broker/broker_provider.h"
#include "twsclient.h"
#include "logger/quantlogger.h"
#include "common/singleton.h"
#include "broker/requests.h"
#include "broker/response.h"
#include "concurrentqueue/blockingconcurrentqueue.h"

#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <optional>
#include <chrono>
#include <utility>

namespace quanttrader {
namespace broker {

// Type for response callbacks
using ResponseCallBackType = std::function<void(std::shared_ptr<ResponseHeader>)>;

/**
 * TWS Broker Adapter class that connects the TWS client with the broker provider interface.
 * This adapter implements the BrokerProvider interface using the TWS API client.
 */
class TwsBrokerAdapter : public BrokerProvider, public Singleton<TwsBrokerAdapter> {
public:
    // Implementation of BrokerProvider interface methods
    bool connect() override;
    bool disconnect() override;
    bool isConnected() const override;
    
    long requestHistoricalData(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency,
        const std::string& endTime,
        const std::string& duration,
        const std::string& barSize,
        const std::string& whatToShow,
        bool useRTH,
        bool keepUpToDate,
        const std::string& sessionStart = "",
        const std::string& sessionEnd = "") override;
    
    long requestRealTimeData(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency) override;
    
    void cancelHistoricalData(long requestId) override;
    void cancelRealTimeData(long requestId) override;
    
    long placeOrder(
        const std::string& symbol,
        const std::string& secType,
        const std::string& exchange,
        const std::string& currency,
        const std::string& action,
        double quantity,
        double price,
        const std::string& orderType) override;
    
    void cancelOrder(long orderId) override;
    
    void registerBarDataCallback(long requestId, BarDataCallback callback) override;
    void registerTradeCallback(TradeCallback callback) override;
    void registerOrderStatusCallback(OrderStatusCallback callback) override;
    void registerErrorCallback(ErrorCallback callback) override;
    
    void requestCurrentTime() override;
    long getNextRequestId() override;

    // Adapter-specific methods
    bool prepare();
    void run();
    void stop();
    bool isPrepared() const;

    void setResponseQueue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>> responseQueue);
    void setRequestQueue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<RequestHeader>>> requestQueue);

    // Method to push requests to the queue
    long pushRequest(std::shared_ptr<RequestHeader> request, std::optional<ResponseCallBackType> callback);
    bool removeCallback(long requestId);

private:
    friend class Singleton<TwsBrokerAdapter>;
    TwsBrokerAdapter(const std::string_view configPath);
    ~TwsBrokerAdapter();

    // TWS client handling methods
    void runTws();
    void runRequest(std::atomic<int> &twsVersion);
    void runResponse(std::atomic<int> &twsVersion);
    void runMonitor(std::atomic<int> &twsVersion);
    void distributeResponse(std::atomic<int> &twsVersion);
    bool updateConfig();
    void keepAlive();
    void initAfterConnected();

    // Helper methods for converting between provider interface and TWS API
    BarData convertToBarData(const ResHistoricalData& resData);

private:
    quanttrader::log::LoggerPtr logger_ = nullptr;
    std::shared_ptr<TwsClient> client_ = nullptr;
    std::atomic<bool> stopFlag_ = false;
    std::unordered_map<TickerId, ResponseCallBackType> responseCallbacks_;
    std::unordered_map<TickerId, BarDataCallback> barDataCallbacks_;
    TradeCallback tradeCallback_ = nullptr;
    OrderStatusCallback orderStatusCallback_ = nullptr;
    ErrorCallback errorCallback_ = nullptr;

    // Thread management
    std::shared_ptr<std::thread> clientThread_ = nullptr;

    // Request and response queues
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<RequestHeader>>> requestQueue_ = nullptr;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>> responseQueue_ = nullptr;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResErrorMsg>>> errorQueue_ = nullptr;

    // Configuration for timing
    std::string host_{"127.0.0.1"};
    int port_{0};
    int clientId_{0};
    std::chrono::milliseconds retryInterval_{1000};    // Default 1 second
    std::chrono::milliseconds waitTimeout_{10};        // Default 10 milliseconds
    std::chrono::milliseconds updateConfigInterval_{30000};  // Default 30 seconds

    // session time configuration
    std::string sessionStart_{"09:30:00"};
    std::string sessionEnd_{"16:00:00"};

    // map request id to bar size string for end time calculation
    std::unordered_map<TickerId, std::string> requestBarSize_;

    // map request id to session start and end time
    std::unordered_map<TickerId, std::pair<std::string, std::string>> requestSession_;

    // Configuration path
    std::string configPath_;
};

} // namespace broker
} // namespace quanttrader