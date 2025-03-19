#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/pattern_formatter.h"

#ifdef __LINUX__
#include "spdlog/sinks/syslog_sink.h" // Include syslog sink for Linux systems
#endif

#include "common/consts.h" // Include custom constants used in the logger

#include <thread>
#include <mutex>
#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>
#include <unordered_set>

namespace quanttrader {
namespace log {

// Alias for log level and logger pointer for better readability
using LevelEnum = spdlog::level::level_enum;
using LoggerPtr = std::shared_ptr<spdlog::logger>;

class QuantLogger {
public:
    // Initialize function, should be called before use logger
    static void init();

    // Sets the logging level dynamically based on input string
    static bool set_default_log_level(std::string level);
    static bool set_logger_level(const std::string &name, const std::string &level);
    static bool set_logger_level(const std::string &name, const LevelEnum &level);
    static bool set_all_logger_level(const LevelEnum &level);
    static bool set_all_logger_level(const std::string &level);

    static LoggerPtr get_logger(const std::string &name, const std::string &logname, const std::string &config_path = "");
    static bool remove_logger(const std::string &name);

    static LoggerPtr get_glogger() { return g_logger_; }

    static void print_message(const std::string &&msg);
    static bool has_init() { return init_flag_; }

private:
    // Initializes the thread pool used for asynchronous logging
    static void init_thread_pool();

    // Returns the log file path based on the provided name
    static const std::string get_log_path(const std::string &logname);

    // Static members shared across all instances
    static std::unordered_map<std::string, LevelEnum> log_level_map_; // Map for string-to-level conversion
    static LevelEnum default_log_level_; // Current log level
    static std::once_flag init_pool_flag_; // Ensures thread pool is initialized only once
    static std::mutex logger_mutex_; // Mutex for thread-safe logger operations
    static std::unordered_set<std::string> logger_names_; // Set of logger names to avoid duplicates
    static LoggerPtr g_logger_; // Global logger instance
    static bool init_flag_; // Flag to check if logger is initialized
};

extern LoggerPtr get_common_rotation_logger(const std::string &name, const std::string &logfile);

template <typename... Args>
void Trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->trace(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->debug(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->info(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->warn(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->error(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    if (!QuantLogger::has_init()) {
        QuantLogger::print_message(fmt::format(fmt, std::forward<Args>(args)...));
        return;
    }
    QuantLogger::g_logger_->critical(fmt, std::forward<Args>(args)...);
}

} // namespace logger
} // namespace quanttrader
