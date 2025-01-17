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

struct GenericRequest {
    RequestType request_type = RequestType::NO_REQUEST;
    TickerId request_id = 0;
    std::shared_ptr<void> request_data = nullptr;
};

struct GenericResponse {
    RequestType response_type = RequestType::NO_REQUEST;
    TickerId request_id;
    std::shared_ptr<void> response_data;
};

struct ReqCurrentTime {
};
struct ResCurrentTime {
    long time;
};

struct ResErrorMsg {
    int id;
    time_t error_time;
    int error_code;
    std::string error_string;
    std::string advanced_order_reject_json;
};

}
}