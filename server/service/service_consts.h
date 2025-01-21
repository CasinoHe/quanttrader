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

constexpr int kDefaultUpdateConfigInterval = 60000;  // interval to update the configuration file in milliseconds
constexpr int kDefaultRetryInterval = 5000;          // retry interval to connect to TWS in milliseconds
constexpr int kDefaultWaitTimeout = 10;              // wait timeout in milliseconds in each thread in broker service
constexpr int kDefaultWaitBackTestTimeout = 1000;    // wait timeout in milliseconds in back test service