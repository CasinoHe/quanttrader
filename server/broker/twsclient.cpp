#include "twsclient.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace quanttrader {
namespace broker {

std::atomic<long> TwsClient::next_request_id_{0};

TwsClient::TwsClient() : client_socket_(this) {
    logger_->info("TwsClient instance created.");
}

TwsClient::~TwsClient() {
    disconnect();
    logger_->info("TwsClient instance destroyed.");
}

bool TwsClient::connect(const std::string &host, int port, int client_id) {
    if (client_socket_.isConnected()) {
        logger_->warn("Already connected.");
        return false;
    }

    if (client_socket_.eConnect(host.c_str(), port, client_id)) {
        logger_->info("Connected to TWS at {}:{} with client ID {}.", host, port, client_id);
        return true;
    } else {
        logger_->error("Failed to connect to TWS at {}:{} with client ID {}.", host, port, client_id);
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

void TwsClient::error(int id, time_t error_time, int error_code, const std::string &error_string, const std::string& advancedOrderRejectJson) {
    logger_->error("Error. Id: {}, Code: {}, Msg: {}", id, error_code, error_string);
}

void TwsClient::connectionClosed() {
    logger_->warn("Connection closed.");
}

} // namespace broker
} // namespace quanttrader
