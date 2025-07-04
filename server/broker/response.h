#pragma once

#include "broker_consts.h"
#include <memory>
#include <string>
#include <variant>
#include "Decimal.h"

namespace quanttrader {
namespace broker {

// Common header structure for responses
struct ResponseHeader {
    TickerId request_id;
    MessageType response_type = MessageType::NO_REQUEST;
    virtual ~ResponseHeader() = default;
};

// Response structures
struct ResCurrentTime : ResponseHeader {
    ResCurrentTime() {
        response_type = MessageType::REQUEST_CURRENT_TIME;
    }
    
    long time;
};

struct ResHistoricalData : ResponseHeader {
    ResHistoricalData() {
        response_type = MessageType::REQUEST_HISTORICAL_DATA;
    }
    
    std::variant<std::string, int> date;
    double open;
    double high;
    double low;
    double close;
    Decimal volume;
    Decimal wap;
    int count;
    bool is_end = false;
    std::string start_date;
    std::string end_date;
};

struct ResRealtimeData : ResponseHeader {
    ResRealtimeData() {
        response_type = MessageType::REQUEST_REALTIME_MKT_DATA;
    }
    
    double price;
    Decimal size;
    int field;
};

struct ResContractDetails : ResponseHeader {
    ResContractDetails() {
        response_type = MessageType::REQUEST_CONTRACT_DETAILS;
    }

    std::string trading_hours;
    std::string time_zone;
    bool is_end = false;
};

struct ResErrorMsg : ResponseHeader {
    ResErrorMsg() {
        response_type = MessageType::ERROR_MSG;
    }
    
    time_t error_time;
    int error_code;
    std::string error_string;
    std::string advanced_order_reject_json;
};

}
}