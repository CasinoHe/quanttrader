#pragma once

#include "CommonDefs.h"
#include <memory>
#include <string>
#include <variant>
#include <functional>

namespace quanttrader {
namespace broker {

struct RequestHeader;

// -----------------------------  Request -------------------------------------------
enum class RequestType {
    NO_REQUEST = 0,
    REQUEST_CURRENT_TIME = 1,
    REQUEST_HISTORICAL_DATA = 2,
    REQUEST_REALTIME_MKT_DATA = 3,
    CANCEL_REQUEST_HISTORICAL_DATA = 4,
    CANCEL_REAL_TIME_MKT_DATA = 5,
    ERROR_MSG = 999,
    END_REQUEST = 1000,
};

struct RequestHeader {
    TickerId request_id = 0;
    RequestType request_type = RequestType::NO_REQUEST;

    virtual ~RequestHeader() = default;
};

struct ResponseHeader {
    TickerId request_id;
    RequestType response_type = RequestType::NO_REQUEST;

    virtual ~ResponseHeader() = default;
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
    std::string end_time;
    bool keep_up_to_date = false;
};

struct ReqCancelHistoricalData: RequestHeader {
    ReqCancelHistoricalData() {
        request_type = RequestType::CANCEL_REQUEST_HISTORICAL_DATA;
    }
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

struct ReqCancelRealtimeMktData: RequestHeader {
    ReqCancelRealtimeMktData() {
        request_type = RequestType::CANCEL_REAL_TIME_MKT_DATA;
    }
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

struct ResHistoricalData: ResponseHeader {
    ResHistoricalData() {
        response_type = RequestType::REQUEST_HISTORICAL_DATA;
    }

    std::variant<int, std::string> date;
    double high;
    double low;
    double open;
    double close;
    Decimal wap;
    Decimal volume;
    int count;
    bool is_end;
    std::string start_date;
    std::string end_date;
};

struct ResRealtimeData: ResponseHeader {
    ResRealtimeData() {
        response_type = RequestType::REQUEST_REALTIME_MKT_DATA;
    }

    double price;
    Decimal size;
};

// ----------------------------------------------------------------------------------

}
}