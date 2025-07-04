#pragma once

#include "broker_consts.h"
#include <memory>
#include <string>
#include <functional>

namespace quanttrader {
namespace broker {

// Common header structure for requests
struct RequestHeader {
    TickerId request_id = 0;
    MessageType request_type = MessageType::NO_REQUEST;
    virtual ~RequestHeader() = default;
};

// Request structures
struct ReqCurrentTime : RequestHeader {
    ReqCurrentTime() {
        request_type = MessageType::REQUEST_CURRENT_TIME;
    }
};

struct ReqHistoricalData: RequestHeader {
    ReqHistoricalData() {
        request_type = MessageType::REQUEST_HISTORICAL_DATA;
    }

    std::string symbol;
    std::string security_type;
    std::string currency;
    std::string exchange;
    std::string duration;
    std::string bar_size;
    std::string what_to_show;
    bool use_rth;
    std::string end_time;
    bool keep_up_to_date = false;
};

struct ReqCancelHistoricalData: RequestHeader {
    ReqCancelHistoricalData() {
        request_type = MessageType::CANCEL_REQUEST_HISTORICAL_DATA;
    }
};

struct ReqRealtimeMktData: RequestHeader {
    ReqRealtimeMktData() {
        request_type = MessageType::REQUEST_REALTIME_MKT_DATA;
    }

    std::string symbol;
    std::string security_type;
    std::string currency;
    std::string exchange;
};

struct ReqCancelRealtimeMktData: RequestHeader {
    ReqCancelRealtimeMktData() {
        request_type = MessageType::CANCEL_REAL_TIME_MKT_DATA;
    }
};

struct ReqContractDetails: RequestHeader {
    ReqContractDetails() {
        request_type = MessageType::REQUEST_CONTRACT_DETAILS;
    }

    std::string symbol;
    std::string security_type;
    std::string exchange;
    std::string currency;
};

}
}