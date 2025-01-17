#pragma once

#include "CommonDefs.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace broker {

enum class RequestType {
    NO_REQUEST = 0,
    REQUEST_CURRENT_TIME = 1,
};

struct GenericRequest {
    RequestType request_type = RequestType::NO_REQUEST;
    TickerId request_id = 0;
    std::shared_ptr<void> request_data = nullptr;

    GenericRequest() = default;
    GenericRequest(GenericRequest &&data) { request_type = data.request_type; request_id = data.request_id; request_data = std::move(data.request_data); };
};

struct GenericResponse {
    RequestType response_type = RequestType::NO_REQUEST;
    TickerId request_id;
    std::shared_ptr<void> response_data;

    GenericResponse() = default;
    GenericResponse(GenericResponse &&data) { response_type = data.response_type; request_id = data.request_id; response_data = std::move(data.response_data); };
};

struct ReqCurrentTime {
};
struct ResCurrentTime {
    long time;
};

}
}