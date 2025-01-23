#pragma once

constexpr char UPDATE_CONFIG_INTERVAL_VARIABLE[] = "update_config_interval";
constexpr char HOST_VARIABLE[] = "host";
constexpr char PORT_VARIABLE[] = "port";
constexpr char CLIENTID_VARIABLE[] = "clientid";
constexpr char RETRY_INTERVAL_VARIABLE[] = "retry_interval";
constexpr char WAIT_TIMEOUT_VARIABLE[] = "wait_timeout";
constexpr char RECORD_LOG_VARIABLE[] = "record_log";
constexpr char VERSION_VARIABLE[] = "version";
constexpr char STOP_FLAG_VARIABLE[] = "stop_flag";
constexpr char STOP_BACK_TEST_VARIABLE[] = "stop_runner";
constexpr char START_BACK_TEST_VARIABLE[] = "new_runner";
constexpr char SPLIT_BACK_TEST_VARIABLE[] = ",";

// strategy variables
constexpr char STRATEGY_NAME_VARIABLE[] = "strategy_name";
constexpr char RUNNER_NAME_VARIABLE[] = "runner_name";

constexpr int kDefaultUpdateConfigInterval = 60000;  // interval to update the configuration file in milliseconds
constexpr int kDefaultRetryInterval = 5000;          // retry interval to connect to TWS in milliseconds
constexpr int kDefaultWaitTimeout = 10;              // wait timeout in milliseconds in each thread in broker service
constexpr int kDefaultWaitBackTestTimeout = 1000;    // wait timeout in milliseconds in back test service