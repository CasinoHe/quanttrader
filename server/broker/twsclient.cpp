#include "twsclient.h"
#include "EReader.h"
#include "EReaderOSSignal.h"
#include "requests.h"
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

namespace quanttrader {
namespace broker {

std::atomic<long> TwsClient::next_request_id_{1};

TwsClient::TwsClient(const std::string_view ip, int port, int clientid, int wait_timeout) : signal_handler_(wait_timeout), client_socket_(this, &signal_handler_) {
    host_ = ip;
    port_ = port;
    clientid_ = clientid;
    logger_ = qlog::get_common_rotation_logger("TwsClient", "client", false);
    logger_->info("TwsClient instance created.");
}

TwsClient::~TwsClient() {
    disconnect();

    if (reader_ptr_) {
        reader_ptr_.reset();
    }
    logger_->info("TwsClient instance destroyed.");
}

void TwsClient::update_connect_config(const std::string_view ip, int port, int clientid) {
    host_ = ip;
    port_ = port;
    clientid_ = clientid;
}

bool TwsClient::connect() {
    if (client_socket_.isConnected()) {
        logger_->warn("Already connected.");
        return false;
    }

    logger_->info("Start Connecting to TWS at {}:{} with client ID {}.", host_, port_, clientid_);
    if (client_socket_.eConnect(host_.c_str(), port_, clientid_)) {
        logger_->info("Connected to TWS at {}:{} with client ID {}.", host_, port_, clientid_);
        reader_ptr_ = std::make_unique<EReader>(&client_socket_, &signal_handler_);
        reader_ptr_->start();
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

void TwsClient::process_messages() {
    signal_handler_.waitForSignal();
    errno = 0;
    reader_ptr_->processMsgs();
}

void TwsClient::request_current_time() {
    client_socket_.reqCurrentTime();
    logger_->debug("Requested current time.");
}

void TwsClient::request_real_time_data(TickerId request_id, const Contract &contract) {
    client_socket_.reqMktData(request_id, contract, "", false, false, {});
    logger_->debug("Requested real-time data for request ID: {}", request_id);
}

void TwsClient::cancel_real_time_data(TickerId request_id) {
    client_socket_.cancelMktData(request_id);
    logger_->debug("Cancelled real-time data for request ID: {}", request_id);
}

void TwsClient::request_historical_data(TickerId request_id, const Contract &contract, const std::string &end_time,
                                        const std::string &duration, const std::string &bar_size,
                                        const std::string &what_to_show, int use_rth, bool keep_up_to_date) {
    client_socket_.reqHistoricalData(request_id, contract, end_time, duration, bar_size, what_to_show, use_rth, 1, keep_up_to_date, {});
    logger_->info("Requested historical data for request ID: {}", request_id);
}

void TwsClient::cancel_historical_data(TickerId request_id) {
    client_socket_.cancelHistoricalData(request_id);
    logger_->info("Cancelled historical data for request ID: {}", request_id);
}

// Callbacks from TWS API
void TwsClient::currentTime(long time) {
    auto response = std::make_shared<ResCurrentTime>();
    response->request_id = 0;
    response->time = time;
    response_queue_->enqueue(std::dynamic_pointer_cast<ResponseHeader>(response));
    logger_->debug("Current time: {}", time);
}

// EWrapper Callbacks
void TwsClient::tickPrice(TickerId ticker_id, TickType field, double price, const TickAttrib &attrib) {
    logger_->info("Tick Price. TickerId: {}, Field: {}, Price: {:.2f}", ticker_id, static_cast<int>(field), price);
}

void TwsClient::tickSize(TickerId ticker_id, TickType field, Decimal size) {
    logger_->info("Tick Size. TickerId: {}, Field: {}, Size: {}", ticker_id, static_cast<int>(field), size);
}

void TwsClient::historicalData(TickerId req_id, const Bar &bar) {
    auto response = std::make_shared<ResHistoricalData>();
    response->request_id = req_id;
    response->date = bar.time;
    response->open = bar.open;
    response->high = bar.high;
    response->low = bar.low;
    response->close = bar.close;
    response->wap = bar.wap;
    response->volume = bar.volume;
    response->count = bar.count;
    response->is_end = false;
    response_queue_->enqueue(std::dynamic_pointer_cast<ResponseHeader>(response));

    logger_->info("Historical Data. ReqId: {}, Date: {}, Open: {}, High: {}, Low: {}, Close: {}, Volume: {} Wap: {} Count: {}",
                  req_id, bar.time, bar.open, bar.high, bar.low, bar.close, bar.volume, bar.wap, bar.count);
}

void TwsClient::historicalDataEnd(int req_id, const std::string& start_date, const std::string& end_date) {
    auto response = std::make_shared<ResHistoricalData>();
    response->request_id = req_id;
    response->is_end = true;
    response->start_date = start_date;
    response->end_date = end_date;
    response_queue_->enqueue(std::dynamic_pointer_cast<ResponseHeader>(response));
    logger_->info("Historical Data End. ReqId: {}, Start Date: {}, End Date: {}", req_id, start_date, end_date);
}

void TwsClient::error(int id, time_t error_time, int error_code, const std::string &error_string, const std::string& advancedOrderRejectJson) {
    auto response = std::make_shared<ResErrorMsg>();
    response->request_id = id;
    response->error_time = error_time;
    response->error_code = error_code;
    response->error_string = error_string;
    response->advanced_order_reject_json = advancedOrderRejectJson;
    response_queue_->enqueue(std::dynamic_pointer_cast<ResponseHeader>(response));

    logger_->error("Error. Id: {}, Code: {}, Msg: {}", id, error_code, error_string);
}

void TwsClient::connectionClosed() {
    logger_->warn("Connection closed.");
}

} // namespace broker
} // namespace quanttrader
