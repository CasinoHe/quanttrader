#pragma once

#include "CommonDefs.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace broker {

enum class RequestType {
    NO_REQUEST = 0,
    REQUEST_CURRENT_TIME = 1,
    ERROR_MSG = 2,
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

}
}