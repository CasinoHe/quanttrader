#include "twsclient.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace quanttrader {
namespace broker {

std::atomic<long> TwsClient::next_request_id_{0};

TwsClient::TwsClient(const std::string_view ip, int port, int clientid) : client_socket_(this, &signal_handler_), mutex_() {
    host_ = ip;
    port_ = port;
    clientid_ = clientid;
    logger_ = qlog::get_common_rotation_logger("TwsClient", "tws_client", false);
    logger_->info("TwsClient instance created.");
}

TwsClient::~TwsClient() {
    disconnect();
    logger_->info("TwsClient instance destroyed.");
}

bool TwsClient::connect() {
    if (client_socket_.isConnected()) {
        logger_->warn("Already connected.");
        return false;
    }

    // Set a signal handler to log connection signals (optional)
    signal_handler_.set_on_signal([this]() {
        logger_->info("Signal received for connection.");
    });

    logger_->info("Start Connecting to TWS at {}:{} with client ID {}.", host_, port_, clientid_);
    if (client_socket_.eConnect(host_.c_str(), port_, clientid_)) {
        logger_->info("Connected to TWS at {}:{} with client ID {}.", host_, port_, clientid_);
        return true;
    } else {
        logger_->error("Failed to connect to TWS at {}:{} with client ID {}.", host_, port_, clientid_);
        return false;
    }
}

void TwsClient::disconnect() {
    if (client_socket_.isConnected()) {
        client_socket_.eDisconnect();
        logger_->info("Disconnected from TWS.");
    }
}

void TwsClient::request_real_time_data(TickerId request_id, const Contract &contract) {
    std::lock_guard<std::mutex> lock(mutex_);
    client_socket_.reqMktData(request_id, contract, "", false, false, {});
    logger_->info("Requested real-time data for request ID: {}", request_id);
}

void TwsClient::cancel_real_time_data(TickerId request_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    client_socket_.cancelMktData(request_id);
    logger_->info("Cancelled real-time data for request ID: {}", request_id);
}

void TwsClient::request_historical_data(TickerId request_id, const Contract &contract, const std::string &end_time,
                                        const std::string &duration, const std::string &bar_size,
                                        const std::string &what_to_show, int use_rth) {
    std::lock_guard<std::mutex> lock(mutex_);
    client_socket_.reqHistoricalData(request_id, contract, end_time, duration, bar_size, what_to_show, use_rth, 1, false, {});
    logger_->info("Requested historical data for request ID: {}", request_id);
}

void TwsClient::cancel_historical_data(TickerId request_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    client_socket_.cancelHistoricalData(request_id);
    logger_->info("Cancelled historical data for request ID: {}", request_id);
}

// EWrapper Callbacks
void TwsClient::tickPrice(TickerId ticker_id, TickType field, double price, const TickAttrib &attrib) {
    logger_->info("Tick Price. TickerId: {}, Field: {}, Price: {:.2f}", ticker_id, static_cast<int>(field), price);
}

void TwsClient::tickSize(TickerId ticker_id, TickType field, Decimal size) {
    logger_->info("Tick Size. TickerId: {}, Field: {}, Size: {}", ticker_id, static_cast<int>(field), size);
}

void TwsClient::historicalData(TickerId req_id, const Bar &bar) {
    logger_->info("Historical Data. ReqId: {}, Date: {}, Open: {}, High: {}, Low: {}, Close: {}, Volume: {}",
                  req_id, bar.time, bar.open, bar.high, bar.low, bar.close, bar.volume);
}

void TwsClient::historicalDataEnd(int req_id, const std::string& start_date, const std::string& end_date) {
    logger_->info("Historical Data End. ReqId: {}, Start Date: {}, End Date: {}", req_id, start_date, end_date);
}

void TwsClient::error(int id, time_t error_time, int error_code, const std::string &error_string, const std::string& advancedOrderRejectJson) {
    logger_->error("Error. Id: {}, Code: {}, Msg: {}", id, error_code, error_string);
}

void TwsClient::connectionClosed() {
    logger_->warn("Connection closed.");
}

} // namespace broker
} // namespace quanttrader
