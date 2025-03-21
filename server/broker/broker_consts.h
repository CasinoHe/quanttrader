#pragma once
#include "CommonDefs.h"
#include <string>

namespace quanttrader {
namespace broker {

// Message type enum for both requests and responses
enum class MessageType {
    NO_REQUEST = 0,
    REQUEST_CURRENT_TIME = 1,
    REQUEST_HISTORICAL_DATA = 2,
    REQUEST_REALTIME_MKT_DATA = 3,
    CANCEL_REQUEST_HISTORICAL_DATA = 4,
    CANCEL_REAL_TIME_MKT_DATA = 5,
    ERROR_MSG = 999,
    END_REQUEST = 1000,
};

}
}