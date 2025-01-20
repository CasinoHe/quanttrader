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

    std::string symbol = "AAPL";
    std::string security_type = "STK";
    std::string currency = "USD";
    std::string exchange = "SMART";
    std::string duration = "1 D";
    std::string bar_size = "30 S";
    std::string what_to_show = "TRADES";
    bool use_rth = true;
    bool format_date = false;
    std::string end_time = "20220101 00:00:00";
    bool keep_up_to_date = false;
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