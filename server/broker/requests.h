#pragma once

#include "CommonDefs.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace broker {

// -----------------------------  Request -------------------------------------------
enum class RequestType {
    NO_REQUEST = 0,
    REQUEST_CURRENT_TIME = 1,
    REQUEST_HISTORICAL_DATA = 2,
    REQUEST_REALTIME_MKT_DATA = 3,
    ERROR_MSG = 999,
    END_REQUEST = 1000,
};

struct RequestHeader {
    TickerId request_id = 0;
    RequestType request_type = RequestType::NO_REQUEST;
};

struct ResponseHeader {
    TickerId request_id;
    RequestType response_type = RequestType::NO_REQUEST;
};

struct ReqCurrentTime : RequestHeader {
    ReqCurrentTime() {
        request_type = RequestType::REQUEST_CURRENT_TIME;
    }
};

struct ReqHistoricalData: RequestHeader {
    ReqHistoricalData() {
        request_type = RequestType::REQUEST_HISTORICAL_DATA;
    }

    std::string symbol;
    std::string security_type;
    std::string currency;
    std::string exchange;
    std::string duration;
    std::string bar_size;
    std::string what_to_show;
    bool use_rth;
    bool format_date = false;
    std::string end_time;
    bool keep_up_to_date = false;
};

struct ReqRealtimeMktData: RequestHeader {
    ReqRealtimeMktData() {
        request_type = RequestType::REQUEST_REALTIME_MKT_DATA;
    }

    std::string symbol;
    std::string security_type;
    std::string currency;
    std::string exchange;
};

// ----------------------------------------------------------------------------------


// -----------------------------  Response -------------------------------------------
struct ResCurrentTime : ResponseHeader {
    long time;
    ResCurrentTime() {
        response_type = RequestType::REQUEST_CURRENT_TIME;
    }
};

struct ResErrorMsg : ResponseHeader {
    time_t error_time;
    int error_code;
    std::string error_string;
    std::string advanced_order_reject_json;

    ResErrorMsg() {
        response_type = RequestType::ERROR_MSG;
    }
};

// ----------------------------------------------------------------------------------

}
}