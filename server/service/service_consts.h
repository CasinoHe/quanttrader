#pragma once

#include <string>

namespace quanttrader {
namespace service {

// Common service name constants
constexpr inline const char* STOCK_TRADE_SERVICE_NAME = "StockTradeService";

constexpr char VERSION_VARIABLE[] = "version";
constexpr char STOP_BACK_TEST_VARIABLE[] = "stop_runner";
constexpr char START_BACK_TEST_VARIABLE[] = "new_runner";
constexpr char SPLIT_BACK_TEST_VARIABLE[] = ",";

// strategy variables
constexpr char STRATEGY_NAME_VARIABLE[] = "strategy_name";

constexpr int kDefaultUpdateConfigInterval = 60000;  // interval to update the configuration file in milliseconds
constexpr int kDefaultRetryInterval = 5000;          // retry interval to connect to TWS in milliseconds
constexpr int kDefaultWaitTimeout = 10;              // wait timeout in milliseconds in each thread in broker service
constexpr int kDefaultWaitBackTestTimeout = 1000;    // wait timeout in milliseconds in back test service

}
}