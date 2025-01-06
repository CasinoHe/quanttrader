#include <fmt/core.h>

#include "quantlogger.h"
#include "spdlog/async.h"

#include <filesystem>

namespace quanttrader {
namespace logger {

// Static member initialization
LevelEnum QuantLoggerMgr::log_level_ = LevelEnum::info; // Default log level
std::once_flag QuantLoggerMgr::thread_pool_flag_; // Ensures single thread pool initialization
std::mutex QuantLoggerMgr::logger_mutex_; // Mutex for thread-safe access
std::unordered_map<std::string, LevelEnum> QuantLoggerMgr::log_level_map_ = {
    {"trace", LevelEnum::trace}, {"debug", LevelEnum::debug},
    {"info", LevelEnum::info}, {"warn", LevelEnum::warn},
    {"error", LevelEnum::err}, {"critical", LevelEnum::critical}
};

// Global logger and manager
QuantLoggerMgr g_logger_mgr = QuantLoggerMgr();
LoggerPtr g_logger = g_logger_mgr.get_file_and_console_logger("quanttrader", "global");

// Constructor: Initializes thread pool for async logging
QuantLoggerMgr::QuantLoggerMgr() {
    std::lock_guard<std::mutex> lock(QuantLoggerMgr::logger_mutex_);
    std::call_once(QuantLoggerMgr::thread_pool_flag_, [this] { init_thread_pool(); });
}

// Destructor: Ensures proper cleanup of loggers
QuantLoggerMgr::~QuantLoggerMgr() {
    spdlog::shutdown();
}

// Initializes the thread pool with predefined constants
void QuantLoggerMgr::init_thread_pool() {
    spdlog::init_thread_pool(quanttrader::kSpdLogQueueSize, quanttrader::kSpdLogThreadsCount);
}

// Sets the logging level dynamically based on string input
bool QuantLoggerMgr::set_log_level(std::string level) {
    auto it = log_level_map_.find(level);
    if (it != log_level_map_.end()) {
        log_level_ = it->second;
        return true;
    } else {
        std::cerr << "Invalid log level: " << level << std::endl;
        return false;
    }
}

// Generates the log file path based on the provided name
const std::string QuantLoggerMgr::get_log_path(const std::string &logname) {
    if (!std::filesystem::exists(quanttrader::kDefaultLogDir)) {
        std::filesystem::create_directories(quanttrader::kDefaultLogDir);
    }
    return logname.empty()
        ? fmt::format("{}/running.log", quanttrader::kDefaultLogDir)
        : fmt::format("{}/{}.log", quanttrader::kDefaultLogDir, logname);
}

// Retrieves or creates a file logger based on specified parameters
LoggerPtr QuantLoggerMgr::get_file_logger(const std::string &name, const std::string &logfile, bool with_stdout,
                                          std::shared_ptr<std::map<std::string, size_t>> max_size_map,
                                          std::shared_ptr<std::map<std::string, int>> rotation_hour_map) {
    const std::string &path = get_log_path(logfile);

    if (max_size_map && max_size_map->find("max_size") != max_size_map->end())
    {
        size_t max_size = max_size_map->at("max_size");
        uint16_t max_files = static_cast<uint16_t>(get_map_value<std::string, size_t>(max_size_map, "max_files", quanttrader::kSpdLogMaxRotatingFiles));
        std::cout << fmt::format("Create rotating file logger: {} path: {} max_size: {} max_files: {}", name, path, max_size, max_files) << std::endl;
        return get_logger<spdlog::sinks::rotating_file_sink_mt>(name, with_stdout, path, max_size, max_files);
    }
    else if (rotation_hour_map && rotation_hour_map->find("rotation_hour") != rotation_hour_map->end())
    {
        int rotation_hour = rotation_hour_map->at("rotation_hour");
        int rotation_min = get_map_value<std::string, int>(rotation_hour_map, "rotation_min", 0);
        uint16_t max_files = static_cast<uint16_t>(get_map_value<std::string, int>(rotation_hour_map, "max_files", quanttrader::kSpdLogMaxRotatingFiles));
        std::cout << fmt::format("Create daily file logger: {} path: {} rotation_hour: {} rotation_min: {} max_files: {}", name, path, rotation_hour, rotation_min, max_files) << std::endl;
        return get_logger<spdlog::sinks::daily_file_sink_mt>(name, with_stdout, path, rotation_hour, rotation_min, false, max_files);
    }
    else
    {
        std::cout << "Create common file logger" << name << " path: " << path << std::endl;
        return get_logger<spdlog::sinks::basic_file_sink_mt>(name, with_stdout, path);
    }
}

// Retrieves or creates a console-only logger
LoggerPtr QuantLoggerMgr::get_console_logger(const std::string &name) {
    return get_logger<spdlog::sinks::stdout_color_sink_mt>(name, false);
}

// Retrieves or creates a logger that outputs to both file and console
LoggerPtr QuantLoggerMgr::get_file_and_console_logger(const std::string &name, const std::string &logfile,
                                                      std::shared_ptr<std::map<std::string, size_t>> max_size_map,
                                                      std::shared_ptr<std::map<std::string, int>> rotation_hour_map) {
    return get_file_logger(name, logfile, true, max_size_map, rotation_hour_map);
}

} // namespace logger
} // namespace quanttrader
