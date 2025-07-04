#include "tws_broker_adapter.h"
#include "service/service_consts.h"
#include "config/lua_config_loader.h"
#include "broker/broker_consts.h"
#include "data/common/data_provider.h"
#include "time/time_with_zone.h"
#include <chrono>
#include <utility>
#include <sstream>

namespace quanttrader {
namespace broker {

// Implementation of the TwsBrokerAdapter singleton constructor
TwsBrokerAdapter::TwsBrokerAdapter(const std::string_view configPath) : configPath_(configPath) {
    logger_ = quanttrader::log::get_common_rotation_logger("TwsBroker", "adapter");
    logger_->info("TwsBrokerAdapter instance created with config file: {}", configPath);
}

TwsBrokerAdapter::~TwsBrokerAdapter() {
    // Ensure background thread is properly terminated before destruction
    stop();
}

// Implementation of BrokerProvider interface methods
bool TwsBrokerAdapter::connect() {
    if (!client_) {
        logger_->error("Client is not initialized. Call prepare() first.");
        return false;
    }
    return client_->connect();
}

bool TwsBrokerAdapter::disconnect() {
    if (!client_) {
        return false;
    }
    client_->disconnect();
    return true;
}

bool TwsBrokerAdapter::isConnected() const {
    if (!client_) {
        return false;
    }
    return client_->is_connected();
}

long TwsBrokerAdapter::getNextRequestId() {
    return TwsClient::next_request_id();
}

void TwsBrokerAdapter::requestCurrentTime() {
    auto request = std::make_shared<ReqCurrentTime>();
    request->request_id = 0;  // Time request doesn't use request ID
    pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), std::nullopt);
}

long TwsBrokerAdapter::requestHistoricalData(
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
    const std::string& sessionStart,
    const std::string& sessionEnd) {
    
    auto request = std::make_shared<ReqHistoricalData>();
    request->symbol = symbol;
    request->security_type = secType;
    request->exchange = exchange;
    request->currency = currency;
    request->end_time = endTime;
    request->duration = duration;
    request->bar_size = barSize;
    request->what_to_show = whatToShow;
    request->use_rth = useRTH;
    request->keep_up_to_date = keepUpToDate;
    
    // Define callback for handling historical data
    auto callback = [this](std::shared_ptr<ResponseHeader> response) {
        auto histResponse = std::dynamic_pointer_cast<ResHistoricalData>(response);
        if (!histResponse) {
            logger_->error("Failed to cast response to ResHistoricalData");
            return;
        }
        
        // Convert to BarData and call registered callback
        auto callbackIt = barDataCallbacks_.find(histResponse->request_id);
        if (callbackIt != barDataCallbacks_.end() && callbackIt->second) {
            BarData barData = convertToBarData(*histResponse);
            callbackIt->second(barData);
        } else {
            logger_->warn("Callback not found for request ID: {}", histResponse->request_id);
        }
    };
    
    long requestId = pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), callback);
    if (requestId > 0) {
        requestBarSize_[requestId] = barSize;
        std::string ss = sessionStart.empty() ? sessionStart_ : sessionStart;
        std::string se = sessionEnd.empty() ? sessionEnd_ : sessionEnd;
        requestSession_[requestId] = {ss, se};
    }
    return requestId;
}

long TwsBrokerAdapter::requestRealTimeData(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) {
    
    auto request = std::make_shared<ReqRealtimeMktData>();
    request->symbol = symbol;
    request->security_type = secType;
    request->exchange = exchange;
    request->currency = currency;
    
    // Define callback for handling real-time data
    auto callback = [this](std::shared_ptr<ResponseHeader> response) {
        auto rtResponse = std::dynamic_pointer_cast<ResRealtimeData>(response);
        if (!rtResponse || !tradeCallback_) {
            return;
        }
        
        // Call the trade callback
        tradeCallback_("", rtResponse->price, rtResponse->size);  // Symbol not in response, would need to be mapped
    };
    
    return pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), callback);
}

long TwsBrokerAdapter::requestContractDetails(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) {

    auto request = std::make_shared<ReqContractDetails>();
    request->symbol = symbol;
    request->security_type = secType;
    request->exchange = exchange;
    request->currency = currency;

    auto callback = [this, symbol, secType, exchange, currency](std::shared_ptr<ResponseHeader> response) {
        auto cdResponse = std::dynamic_pointer_cast<ResContractDetails>(response);
        if (!cdResponse) {
            logger_->error("Failed to cast response to ResContractDetails");
            return;
        }

        if (!cdResponse->is_end && cdResponse->contract_details) {
            // Store the contract details using the TWS ContractDetails structure directly
            std::string key = getContractKey(symbol, secType, exchange, currency);
            contractDetails_[key] = *cdResponse->contract_details;
            
            logger_->info("Stored contract details for key: {} trading_hours: {} liquid_hours: {} time_zone: {}", 
                         key, cdResponse->contract_details->tradingHours, 
                         cdResponse->contract_details->liquidHours, cdResponse->contract_details->timeZoneId);
        }

        auto it = contractDetailCallbacks_.find(cdResponse->request_id);
        if (it != contractDetailCallbacks_.end() && it->second) {
            if (cdResponse->contract_details) {
                // Call the callback with the full contract details
                it->second(*cdResponse->contract_details);
            } else {
                logger_->warn("Contract details pointer is null for request ID: {}", cdResponse->request_id);
            }
            
            if (cdResponse->is_end) {
                contractDetailCallbacks_.erase(it);
                removeCallback(cdResponse->request_id);
                requestIdToContractKey_.erase(cdResponse->request_id);
            }
        }
    };

    long requestId = pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), callback);
    if (requestId > 0) {
        // Store the mapping from request ID to contract key for cleanup
        std::string key = getContractKey(symbol, secType, exchange, currency);
        requestIdToContractKey_[requestId] = key;
    }
    return requestId;
}

void TwsBrokerAdapter::cancelHistoricalData(long requestId) {
    auto request = std::make_shared<ReqCancelHistoricalData>();
    request->request_id = requestId;
    pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), std::nullopt);
    
    // Remove any registered callbacks for this request
    barDataCallbacks_.erase(requestId);
    requestBarSize_.erase(requestId);
    requestSession_.erase(requestId);
    removeCallback(requestId);
    logger_->info("Cancelled historical data for request ID: {}", requestId);
}

void TwsBrokerAdapter::cancelRealTimeData(long requestId) {
    auto request = std::make_shared<ReqCancelRealtimeMktData>();
    request->request_id = requestId;
    pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), std::nullopt);
    
    // Remove any registered callbacks for this request
    removeCallback(requestId);
    logger_->info("Cancelled real-time data for request ID: {}", requestId);
}

// Not implemented in this version, would need additional message types
long TwsBrokerAdapter::placeOrder(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency,
    const std::string& action,
    double quantity,
    double price,
    const std::string& orderType) {
    
    // Placeholder for implementation
    logger_->warn("placeOrder not implemented yet");
    return -1;
}

void TwsBrokerAdapter::cancelOrder(long orderId) {
    // Placeholder for implementation
    logger_->warn("cancelOrder not implemented yet");
}

void TwsBrokerAdapter::registerBarDataCallback(long requestId, BarDataCallback callback) {
    barDataCallbacks_[requestId] = callback;
    logger_->info("Registered bar data callback for request ID: {}", requestId);
}

void TwsBrokerAdapter::registerTradeCallback(TradeCallback callback) {
    tradeCallback_ = callback;
}

void TwsBrokerAdapter::registerOrderStatusCallback(OrderStatusCallback callback) {
    orderStatusCallback_ = callback;
}

void TwsBrokerAdapter::registerErrorCallback(ErrorCallback callback) {
    errorCallback_ = callback;
}

void TwsBrokerAdapter::registerContractDetailsCallback(long requestId, std::function<void(const ContractDetails&)> callback) {
    contractDetailCallbacks_[requestId] = std::move(callback);
}

// Adapter-specific methods
bool TwsBrokerAdapter::prepare() {
    // Initialize client and queues
    requestQueue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<RequestHeader>>>();
    responseQueue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>>();

    // load config
    updateConfig();

    // Create the TWS client
    client_ = std::make_shared<TwsClient>(host_, port_, clientId_, static_cast<int>(waitTimeout_.count()));
    
    // Set up the client's response queue
    client_->set_response_queue(responseQueue_);
    
    logger_->info("TwsBrokerAdapter prepared with host {}, port {}, clientId {}", host_, port_, clientId_);
    return true;
}

void TwsBrokerAdapter::run() {
    if (!isPrepared()) {
        logger_->error("Adapter is not prepared. Please call prepare() first.");
        return;
    }
    
    // Start the TWS thread without joining it
    // This allows the thread to run in the background
    clientThread_ = std::make_shared<std::thread>(&TwsBrokerAdapter::runTws, this);
    
    // Don't join the thread here, that would block the calling thread
    // The thread will continue to run in the background
    logger_->info("TWS client thread started in background");
}

void TwsBrokerAdapter::stop() {
    stopFlag_.store(true);
    logger_->info("Stop flag set");
    
    // Join the client thread if it exists and is joinable
    if (clientThread_ && clientThread_->joinable()) {
        logger_->info("Waiting for TWS client thread to finish...");
        clientThread_->join();
        logger_->info("TWS client thread joined successfully");
    }
}

bool TwsBrokerAdapter::isPrepared() const {
    if (!client_) {
        return false;
    }
    
    if (!requestQueue_) {
        return false;
    }
    
    if (!responseQueue_) {
        return false;
    }
    
    return true;
}

void TwsBrokerAdapter::setResponseQueue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>> responseQueue) {
    responseQueue_ = responseQueue;
}

void TwsBrokerAdapter::setRequestQueue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<RequestHeader>>> requestQueue) {
    requestQueue_ = requestQueue;
}

long TwsBrokerAdapter::pushRequest(std::shared_ptr<RequestHeader> request, std::optional<ResponseCallBackType> callback) {
    long requestId = TwsClient::next_request_id();
    request->request_id = requestId;
    
    // Register callback if provided
    if (callback.has_value()) {
        responseCallbacks_[requestId] = callback.value();
    }
    
    bool result = requestQueue_->enqueue(request);
    if (!result) {
        logger_->error("Cannot push the request to the queue.");
        return -1;
    } else {
        return requestId;
    }
}

bool TwsBrokerAdapter::removeCallback(long requestId) {
    auto iter = responseCallbacks_.find(requestId);
    if (iter != responseCallbacks_.end()) {
        responseCallbacks_.erase(iter);
        return true;
    }
    return false;
}

// Helper methods
BarData TwsBrokerAdapter::convertToBarData(const ResHistoricalData& resData) {
    BarData barData;
    barData.end_time = 0;
    if (resData.is_end) {
        logger_->info("Received end of historical data for request ID: {}", resData.request_id);
        barData.is_last = true;
        return barData;
    }
    
    // Handle the variant type for date
    if (std::holds_alternative<std::string>(resData.date)) {
        std::string dateStr = std::get<std::string>(resData.date);
        bool onlyDate = dateStr.size() == 8;
        std::string parseStr = dateStr;
        if (onlyDate) {
            std::string sessStart = sessionStart_;
            auto sit = requestSession_.find(resData.request_id);
            if (sit != requestSession_.end()) {
                sessStart = sit->second.first;
            }
            parseStr += " " + sessStart;
        }

        auto timeWithZone = quanttrader::time::TimeWithZone::from_ibapi_string(parseStr, "America/New_York");
        if (timeWithZone.has_value()) {
            barData.time = timeWithZone.value().get_nano_epoch();

            if (onlyDate) {
                std::string sessEnd = sessionEnd_;
                auto sit = requestSession_.find(resData.request_id);
                if (sit != requestSession_.end()) {
                    sessEnd = sit->second.second;
                }
                std::string endStr = std::get<std::string>(resData.date) + " " + sessEnd;
                auto endWithZone = quanttrader::time::TimeWithZone::from_ibapi_string(endStr, "America/New_York");
                if (endWithZone.has_value()) {
                    barData.end_time = endWithZone.value().get_nano_epoch();
                }
            } else {
                auto iter = requestBarSize_.find(resData.request_id);
                if (iter != requestBarSize_.end()) {
                    auto barInfo = quanttrader::data::provider::DataProvider::get_bar_type_from_string(iter->second);
                    uint64_t sec = barInfo.second;
                    switch (barInfo.first) {
                        case data::BarType::Minute: sec *= 60; break;
                        case data::BarType::Hour: sec *= 3600; break;
                        case data::BarType::Day: sec *= 86400; break;
                        case data::BarType::Week: sec *= 604800; break;
                        case data::BarType::Month: sec *= 2592000; break; // approx
                        default: break; // seconds already handled
                    }
                    if (sec > 0) {
                        barData.end_time = barData.time + sec * 1000000000ULL;
                    }
                }
            }
        } else {
            logger_->error("Failed to parse date string: {} open:{} high:{} low:{} close:{}", dateStr, resData.open, resData.high, resData.low, resData.close);
            barData.time = 0;
        }
    } else if (std::holds_alternative<int>(resData.date)) {
        barData.time = std::get<int>(resData.date);
        auto iter = requestBarSize_.find(resData.request_id);
        if (iter != requestBarSize_.end()) {
            auto barInfo = quanttrader::data::provider::DataProvider::get_bar_type_from_string(iter->second);
            uint64_t sec = barInfo.second;
            switch (barInfo.first) {
                case data::BarType::Minute: sec *= 60; break;
                case data::BarType::Hour: sec *= 3600; break;
                case data::BarType::Day: sec *= 86400; break;
                case data::BarType::Week: sec *= 604800; break;
                case data::BarType::Month: sec *= 2592000; break;
                default: break;
            }
            if (sec > 0) {
                barData.end_time = barData.time + sec * 1000000000ULL;
            }
        }
    }
    
    barData.open = resData.open;
    barData.high = resData.high;
    barData.low = resData.low;
    barData.close = resData.close;
    barData.volume = resData.volume;
    barData.wap = resData.wap;
    barData.count = resData.count;
    barData.is_last = resData.is_end;
    
    return barData;
}

// TWS client handling methods
void TwsBrokerAdapter::runTws() {
    auto connectFunc = [this]() {
        while (true) {
            if (stopFlag_.load()) {
                logger_->info("Stop flag is set, exit the TWS client thread.");
                break;
            }
            // Try connecting to TWS infinitely
            if (!client_->connect()) {
                logger_->error("Connect to TWS failed. Retry in {} milliseconds.", retryInterval_.count());
                std::this_thread::sleep_for(retryInterval_);
            } else {
                break;
            }
        }
    };

    // Initialize version tracking for thread management
    std::atomic<int> twsVersion = 0;
    int lastTwsVersion = twsVersion.load();

    // Threads for handling different aspects of TWS communication
    std::shared_ptr<std::thread> processRequestThread = nullptr;
    std::shared_ptr<std::thread> processResponseThread = nullptr;
    std::shared_ptr<std::thread> configMonitorThread = nullptr;
    std::shared_ptr<std::thread> distributeResponseThread = nullptr;

    auto waitAll = [&]() {
        if (processRequestThread && processRequestThread->joinable()) {
            processRequestThread->join();
        }
        if (processResponseThread && processResponseThread->joinable()) {
            processResponseThread->join();
        }
        if (configMonitorThread && configMonitorThread->joinable()) {
            configMonitorThread->join();
        }
        if (distributeResponseThread && distributeResponseThread->joinable()) {
            distributeResponseThread->join();
        }
    };

    while (true) {
        if (!client_->is_connected()) {
            // Try connect to TWS
            connectFunc();

            if (client_->is_connected()) {
                twsVersion.fetch_add(1);
            } else {
                if (stopFlag_.load()) {
                    logger_->info("Stop flag is set, exit the TWS client thread.");
                    break;
                } else {
                    std::this_thread::sleep_for(retryInterval_);
                }
            }
        } else {
            if (stopFlag_.load()) {
                logger_->info("Stop flag is set, exit the TWS client thread.");
                break;
            }

            if (twsVersion.load() != lastTwsVersion) {
                lastTwsVersion = twsVersion.load();
                // Wait for all threads to finish
                waitAll();

                // Initialize data before starting threads
                initAfterConnected();

                // Start the threads
                processRequestThread = std::make_shared<std::thread>(&TwsBrokerAdapter::runRequest, this, std::ref(twsVersion));
                processResponseThread = std::make_shared<std::thread>(&TwsBrokerAdapter::runResponse, this, std::ref(twsVersion));
                configMonitorThread = std::make_shared<std::thread>(&TwsBrokerAdapter::runMonitor, this, std::ref(twsVersion));
                distributeResponseThread = std::make_shared<std::thread>(&TwsBrokerAdapter::distributeResponse, this, std::ref(twsVersion));
            }

            // Avoid 100% CPU usage
            std::this_thread::sleep_for(retryInterval_);

            // Keep alive
            keepAlive();
        }
    }

    waitAll();
}

void TwsBrokerAdapter::runRequest(std::atomic<int> &twsVersion) {
    int lastVersion = twsVersion.load();

    while (true) {
        if (twsVersion.load() != lastVersion) {
            // Exit the thread
            break;
        }

        if (stopFlag_.load()) {
            logger_->info("Stop flag is set, stop the request thread.");
            break;
        }

        std::shared_ptr<RequestHeader> requestPtr = nullptr;
        requestQueue_->wait_dequeue_timed(requestPtr, waitTimeout_);

        if (requestPtr) {
            logger_->debug("Received request: {}", static_cast<int>(requestPtr->request_type));
            if (requestPtr->request_type == MessageType::REQUEST_CURRENT_TIME) {
                // Request current time
                client_->request_current_time();
            } else if (requestPtr->request_type == MessageType::REQUEST_HISTORICAL_DATA) {
                auto request = std::dynamic_pointer_cast<ReqHistoricalData>(requestPtr);
                if (request) {
                    Contract contract;
                    contract.symbol = request->symbol;
                    contract.currency = request->currency;
                    contract.exchange = request->exchange;
                    contract.secType = request->security_type;

                    client_->request_historical_data(request->request_id, contract, request->end_time, request->duration,
                                                    request->bar_size, request->what_to_show, request->use_rth,
                                                    request->keep_up_to_date);
                } else {
                    logger_->error("Cannot cast the request to ReqHistoricalData.");
                }
            } else if (requestPtr->request_type == MessageType::REQUEST_REALTIME_MKT_DATA) {
                auto request = std::dynamic_pointer_cast<ReqRealtimeMktData>(requestPtr);
                if (request) {
                    Contract contract;
                    contract.symbol = request->symbol;
                    contract.currency = request->currency;
                    contract.exchange = request->exchange;
                    contract.secType = request->security_type;

                    client_->request_real_time_data(request->request_id, contract);
                }
            } else if (requestPtr->request_type == MessageType::CANCEL_REQUEST_HISTORICAL_DATA) {
                auto request = std::dynamic_pointer_cast<ReqCancelHistoricalData>(requestPtr);
                if (request) {
                    client_->cancel_historical_data(request->request_id);
                }
            } else if (requestPtr->request_type == MessageType::CANCEL_REAL_TIME_MKT_DATA) {
                auto request = std::dynamic_pointer_cast<ReqCancelRealtimeMktData>(requestPtr);
                if (request) {
                    client_->cancel_real_time_data(request->request_id);
                }
            } else if (requestPtr->request_type == MessageType::REQUEST_CONTRACT_DETAILS) {
                auto request = std::dynamic_pointer_cast<ReqContractDetails>(requestPtr);
                if (request) {
                    Contract contract;
                    contract.symbol = request->symbol;
                    contract.currency = request->currency;
                    contract.exchange = request->exchange;
                    contract.secType = request->security_type;

                    client_->request_contract_details(request->request_id, contract);
                }
            } else {
                logger_->warn("Cannot find the request type: {}", static_cast<int>(requestPtr->request_type));
            }
        }
    }
}

void TwsBrokerAdapter::runResponse(std::atomic<int> &twsVersion) {
    int lastVersion = twsVersion.load();

    while (true) {
        if (twsVersion.load() != lastVersion) {
            // Exit the thread
            break;
        }

        if (stopFlag_.load()) {
            logger_->info("Stop flag is set, stop the response thread.");
            break;
        }

        // Process messages from TWS client
        client_->process_messages();
    }
}

void TwsBrokerAdapter::distributeResponse(std::atomic<int> &twsVersion) {
    int lastVersion = twsVersion.load();

    while (true) {
        if (twsVersion.load() != lastVersion) {
            // Exit the thread
            break;
        }

        if (stopFlag_.load()) {
            logger_->info("Stop flag is set, stop the response thread.");
            break;
        }

        std::shared_ptr<ResponseHeader> responsePtr = nullptr;
        responseQueue_->wait_dequeue_timed(responsePtr, waitTimeout_);
        if (!responsePtr) {
            continue;
        }

        logger_->debug("Received response: {}", static_cast<int>(responsePtr->response_type));
        if (responsePtr->response_type == MessageType::ERROR_MSG) {
            auto errorResponse = std::dynamic_pointer_cast<ResErrorMsg>(responsePtr);
            if (errorResponse && errorCallback_) {
                errorCallback_(errorResponse->error_code, errorResponse->error_string);
            }
            logger_->error("Received error message: {}", errorResponse ? errorResponse->error_string : "Unknown error");
            continue;
        } else if (responsePtr->response_type == MessageType::REQUEST_CURRENT_TIME) {
            // Handle current time response
            continue;
        } else {
            TickerId requestId = responsePtr->request_id;
            auto iter = responseCallbacks_.find(requestId);
            if (iter != responseCallbacks_.end()) {
                iter->second(responsePtr);
            } else {
                logger_->warn("Cannot find the callback for request id: {}", requestId);
            }
        }
    }
}

void TwsBrokerAdapter::runMonitor(std::atomic<int> &twsVersion) {
    int lastVersion = twsVersion.load();
    auto now = std::chrono::system_clock::now();

    while (true) {
        if (twsVersion.load() != lastVersion) {
            // Exit the thread
            break;
        }

        if (stopFlag_.load()) {
            logger_->info("Stop flag is set, stop the monitor thread.");
            break;
        }

        std::this_thread::sleep_for(waitTimeout_);

        // Check for configuration updates periodically
        auto current = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current - now) > updateConfigInterval_) {
            now = current;
            // Update configuration if needed
            updateConfig();
        }
    }
}

void TwsBrokerAdapter::keepAlive() {
    static auto now = std::chrono::system_clock::now();

    auto aliveInterval = std::chrono::milliseconds(30000);
    auto current = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(current - now) > aliveInterval) {
        now = current;
        auto request = std::make_shared<ReqCurrentTime>();
        request->request_id = 0;  // Current time doesn't check up the request id
        pushRequest(std::dynamic_pointer_cast<RequestHeader>(request), std::nullopt);
    }
}

void TwsBrokerAdapter::initAfterConnected() {
    // Initialize error queue
    errorQueue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResErrorMsg>>>();
    client_->set_response_queue(responseQueue_);
    client_->set_error_queue(errorQueue_);
}

bool TwsBrokerAdapter::updateConfig() {
    auto loader = quanttrader::luascript::LuaConfigLoader(configPath_);
    loader.load_config();

    host_ = loader.get_string_value(TWS_PROVIDER_NAME, HOST_VARIABLE);
    port_ = loader.get_int_value(TWS_PROVIDER_NAME, PORT_VARIABLE);
    clientId_ = loader.get_int_value(TWS_PROVIDER_NAME, CLIENTID_VARIABLE);

    retryInterval_ = std::chrono::milliseconds(loader.get_int_value(TWS_PROVIDER_NAME, RETRY_INTERVAL_VARIABLE));
    waitTimeout_ = std::chrono::milliseconds(loader.get_int_value(TWS_PROVIDER_NAME, WAIT_TIMEOUT_VARIABLE));
    updateConfigInterval_ = std::chrono::milliseconds(loader.get_int_value(TWS_PROVIDER_NAME, UPDATE_CONFIG_INTERVAL_VARIABLE));

    sessionStart_ = loader.get_string_value(TWS_PROVIDER_NAME, SESSION_START_VARIABLE);
    sessionEnd_ = loader.get_string_value(TWS_PROVIDER_NAME, SESSION_END_VARIABLE);

    bool stop_flag = loader.get_bool_value(TWS_PROVIDER_NAME, STOP_FLAG_VARIABLE);
    bool record_log = loader.get_bool_value(TWS_PROVIDER_NAME, RECORD_LOG_VARIABLE);
    stopFlag_.store(stop_flag);

    if (record_log) {
        logger_->info("Updated configuration: host {}, port {}, clientId {}, retryInterval {} ms, waitTimeout {} ms, updateConfigInterval {} ms stop flag {}",
                    host_, port_, clientId_, retryInterval_.count(), waitTimeout_.count(), updateConfigInterval_.count(), stop_flag);
    }

    return true;
}

void TwsBrokerAdapter::clearContractDetails() {
    contractDetails_.clear();
    logger_->info("Cleared all stored contract details");
}

bool TwsBrokerAdapter::removeContractDetails(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) {
    
    std::string key = getContractKey(symbol, secType, exchange, currency);
    
    auto it = contractDetails_.find(key);
    if (it != contractDetails_.end()) {
        contractDetails_.erase(it);
        logger_->info("Removed contract details for key: {}", key);
        return true;
    }
    return false;
}

std::optional<std::string> TwsBrokerAdapter::getTradingHours(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) const {
    
    std::string key = getContractKey(symbol, secType, exchange, currency);
    
    auto it = contractDetails_.find(key);
    if (it != contractDetails_.end()) {
        return it->second.tradingHours;
    }
    return std::nullopt;
}

std::optional<std::string> TwsBrokerAdapter::getLiquidHours(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) const {
    
    std::string key = getContractKey(symbol, secType, exchange, currency);
    
    auto it = contractDetails_.find(key);
    if (it != contractDetails_.end()) {
        return it->second.liquidHours;
    }
    return std::nullopt;
}

std::optional<std::string> TwsBrokerAdapter::getTimeZone(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) const {
    
    std::string key = getContractKey(symbol, secType, exchange, currency);
    
    auto it = contractDetails_.find(key);
    if (it != contractDetails_.end()) {
        return it->second.timeZoneId;
    }
    return std::nullopt;
}

std::optional<ContractDetails> TwsBrokerAdapter::getContractInfo(
    const std::string& symbol,
    const std::string& secType,
    const std::string& exchange,
    const std::string& currency) const {
    
    std::string key = getContractKey(symbol, secType, exchange, currency);
    
    auto it = contractDetails_.find(key);
    if (it != contractDetails_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::string TwsBrokerAdapter::getContractKey(const std::string& symbol, const std::string& secType, 
                                           const std::string& exchange, const std::string& currency) const {
    return symbol + "_" + secType + "_" + exchange + "_" + currency;
}
} // namespace broker
} // namespace quanttrader