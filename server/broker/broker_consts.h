#pragma once

#include <string>

#include "CommonDefs.h"

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
    REQUEST_CONTRACT_DETAILS = 6,
    ERROR_MSG = 999,
    END_REQUEST = 1000,
};

constexpr char HOST_VARIABLE[] = "host";
constexpr char PORT_VARIABLE[] = "port";
constexpr char CLIENTID_VARIABLE[] = "clientid";
constexpr char TWS_PROVIDER_NAME[] = "tws_provider";
constexpr char RETRY_INTERVAL_VARIABLE[] = "retry_interval";
constexpr char WAIT_TIMEOUT_VARIABLE[] = "wait_timeout";
constexpr char UPDATE_CONFIG_INTERVAL_VARIABLE[] = "update_config_interval";
constexpr char STOP_FLAG_VARIABLE[] = "stop_flag";
constexpr char RECORD_LOG_VARIABLE[] = "record_log";
constexpr char SESSION_START_VARIABLE[] = "session_start";
constexpr char SESSION_END_VARIABLE[] = "session_end";
constexpr char SESSION_TIMEZONE_VARIABLE[] = "session_timezone";

}
}