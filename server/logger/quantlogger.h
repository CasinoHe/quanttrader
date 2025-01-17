#pragma once

#include "common/singleton.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
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

namespace quanttrader {
namespace log {

// Alias for log level and logger pointer for better readability
using LevelEnum = spdlog::level::level_enum;
using LoggerPtr = std::shared_ptr<spdlog::logger>;

class QuantLoggerMgr : public Singleton<QuantLoggerMgr> {
private:
    friend class Singleton<QuantLoggerMgr>;
    // Constructor: Initializes thread pool on first creation
    QuantLoggerMgr();
    // Destructor: Ensures all loggers are shutdown properly
    ~QuantLoggerMgr();

public:
    // Sets the logging level dynamically based on input string
    bool set_log_level(std::string level);

    // Returns the log file path based on the provided name
    const std::string get_log_path(const std::string &logname);

    // Helper to safely retrieve map values with a default fallback
    template <typename KeyType, typename ValueType>
    ValueType get_map_value(const std::shared_ptr<std::map<KeyType, ValueType>> map, const KeyType& key, const ValueType& default_val)
    {
        return (map && map->find(key) != map->end()) ? map->at(key) : default_val;
    }

    // Retrieves or creates a logger with specified sink type
    template<typename Sink, typename... SinkArgs>
    LoggerPtr get_logger(const std::string &name, bool with_stdout, SinkArgs &&... sink_args)
    {
        auto logger = spdlog::get(name);
        if (logger)
        {
            return logger;
        }

        std::lock_guard<std::mutex> lock(QuantLoggerMgr::logger_mutex_);

        try
        {
            return create_logger<Sink>(name, with_stdout, std::forward<SinkArgs>(sink_args)...);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return nullptr;
        }
    }

    // Logger creation methods for specific use cases
    LoggerPtr get_file_logger(const std::string &name, const std::string &logfile, bool with_stdout = false,
                              std::shared_ptr<std::map<std::string, size_t>> max_size_map = nullptr,
                              std::shared_ptr<std::map<std::string, int>> rotation_hour_map = nullptr);

    LoggerPtr get_console_logger(const std::string &name);
    LoggerPtr get_file_and_console_logger(const std::string &name, const std::string &logfile,
                                          std::shared_ptr<std::map<std::string, size_t>> max_size_map = nullptr,
                                          std::shared_ptr<std::map<std::string, int>> rotation_hour_map = nullptr);

private:
    // Initializes the thread pool used for asynchronous logging
    void init_thread_pool();

    // Encapsulated logic for creating a logger with specified sink
    template<typename Sink, typename... SinkArgs>
    LoggerPtr create_logger(const std::string &name, bool with_stdout, SinkArgs &&... sink_args)
    {
        auto formattor = std::make_unique<spdlog::pattern_formatter>(quanttrader::kSpdLogPattern);
        auto sink = std::make_shared<Sink>(std::forward<SinkArgs>(sink_args)...);
        sink->set_formatter(formattor->clone());
        sink->set_level(log_level_);
        auto logger = spdlog::create_async_nb<Sink>(name, std::forward<SinkArgs>(sink_args)...);
        logger->sinks().clear();
        logger->sinks().push_back(sink);

        if (with_stdout)
        {
            auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            stdout_sink->set_formatter(formattor->clone());
            stdout_sink->set_level(log_level_);
            logger->sinks().push_back(stdout_sink);
        }
        return logger;
    }

    // Static members shared across all instances
    static std::unordered_map<std::string, LevelEnum> log_level_map_; // Map for string-to-level conversion
    static LevelEnum log_level_; // Current log level
    static std::once_flag thread_pool_flag_; // Ensures thread pool is initialized only once
    static std::mutex logger_mutex_; // Mutex for thread-safe logger operations
};

// Global logger manager and default logger instance
extern std::shared_ptr<QuantLoggerMgr> g_logger_mgr_ptr;
extern LoggerPtr g_logger;

    // ---- Automatically script interface generation begin ----

    // -- Function:
    // ---- Name: 
    //      get_logger
    // ---- Params: 
    //      const std::string &name

    // ---- Automatically script interface generation end ----

extern LoggerPtr get_common_rotation_logger(const std::string &name, const std::string &logfile, bool with_stdout = false);
extern void Trace(const std::string &msg);
extern void Debug(const std::string &msg);
extern void Info(const std::string &msg);
extern void Warn(const std::string &msg);
extern void Error(const std::string &msg);
extern void Critical(const std::string &msg);

} // namespace logger
} // namespace quanttrader
