#pragma once

namespace quanttrader {
namespace logger {

constexpr const char *LOGGER_USE_SYNC_FIELD = "use_sync";
constexpr const char *LOGGER_LEVEL_FIELD = "level";
constexpr const char *LOGGER_SINKS_FIELD = "sinks";

constexpr const char *LOGGER_SINK_DAILY_FILE_TYPE = "daily_file_sink";
constexpr const char *LOGGER_SINK_BASIC_FILE_TYPE = "basic_file_sink";
constexpr const char *LOGGER_SINK_CONSOLE_TYPE = "console_sink";
constexpr const char *LOGGER_SINK_OSTREAM_TYPE = "ostream_sink";

}
}