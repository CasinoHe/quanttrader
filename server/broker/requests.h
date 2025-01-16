#pragma once

#include "CommonDefs.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace broker {

struct GenericRequest {
    int request_type = 0;
    TickerId request_id = 0;
    std::shared_ptr<void> request_data = nullptr;

    GenericRequest() = default;
    GenericRequest(GenericRequest &&data) { request_type = data.request_type; request_id = data.request_id; request_data = std::move(data.request_data); };
};

struct GenericResponse {
    int response_type = 0;
    TickerId request_id;
    std::shared_ptr<void> response_data;

    GenericResponse() = default;
    GenericResponse(GenericResponse &&data) { response_type = data.response_type; request_id = data.request_id; response_data = std::move(data.response_data); };
};

}
}