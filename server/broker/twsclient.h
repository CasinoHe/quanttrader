#pragma once

#include "EClientSocket.h"
#include "EWrapper.h"
#include "Contract.h"
#include "Decimal.h"
#include "logger/quantlogger.h"

#include <mutex>
#include <string>
#include <map>
#include <functional>
#include <atomic>

namespace quanttrader {
namespace broker {

namespace qlog = quanttrader::log;

class TwsClient : public EWrapper {
public:
    TwsClient();
    ~TwsClient();

    bool connect(const std::string &host, int port, int client_id);
    void disconnect();

    void request_real_time_data(TickerId request_id, const Contract &contract);
    void cancel_real_time_data(TickerId request_id);

    void request_historical_data(TickerId request_id, const Contract &contract, const std::string &end_time,
                                  const std::string &duration, const std::string &bar_size,
                                  const std::string &what_to_show, int use_rth);
    void cancel_historical_data(TickerId request_id);

    // EWrapper callbacks
    void tickPrice(TickerId ticker_id, TickType field, double price, const TickAttrib &attrib) override;
    void tickSize(TickerId ticker_id, TickType field, Decimal size) override;
    void historicalData(TickerId req_id, const Bar &bar) override;
    void error(int id, time_t error_time, int error_code, const std::string &error_string, const std::string& advancedOrderRejectJson) override;
    void connectionClosed() override;

    static TickerId next_request_id() { return next_request_id_.fetch_add(1, std::memory_order_relaxed); }

private:
    EClientSocket client_socket_;
    std::mutex mutex_;
    qlog::LoggerPtr logger_ = qlog::get_common_rotation_logger("TwsClient", "tws_client");

    std::string ip_;
    int port_;
    int client_id;

    static std::atomic<TickerId> next_request_id_;
};

}
}