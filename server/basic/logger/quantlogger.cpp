#include <fmt/core.h>

#include "quantlogger.h"
#include "logger_consts.h"
#include "config/lua_config_loader.h"
#include "spdlog/async.h"

#include <filesystem>
#include <sstream>
#include <chrono>

namespace quanttrader {
namespace log {

// Static member initialization
LevelEnum QuantLogger::default_log_level_ = LevelEnum::info; // Default log level
std::once_flag QuantLogger::init_pool_flag_; // Ensures single thread pool initialization
bool QuantLogger::init_flag_ = false; // Flag to check if logger is initialized
std::mutex QuantLogger::logger_mutex_; // Mutex for thread-safe access
std::unordered_set<std::string> QuantLogger::logger_names_; // Set of logger names to avoid duplicates
LoggerPtr QuantLogger::g_logger_; // Global logger instance
std::unordered_map<std::string, LevelEnum> QuantLogger::log_level_map_ = {
    {"trace", LevelEnum::trace}, {"debug", LevelEnum::debug},
    {"info", LevelEnum::info}, {"warn", LevelEnum::warn},
    {"error", LevelEnum::err}, {"critical", LevelEnum::critical}
};

void QuantLogger::init_thread_pool() {
    std::call_once(QuantLogger::init_pool_flag_, [] { spdlog::init_thread_pool(quanttrader::kSpdLogQueueSize, quanttrader::kSpdLogThreadsCount); });
}

void QuantLogger::init() {
    if (init_flag_) {
        return;
    }

    g_logger_ = get_logger("Global", "global", kLoggerConfigFilePath);
    if (!g_logger_) {
        std::cerr << "Failed to create global logger" << std::endl;
        return;
    }
    spdlog::set_default_logger(g_logger_);
    spdlog::flush_every(std::chrono::seconds(2));
    init_flag_ = true;
}

// Sets the logging level dynamically based on string input
bool QuantLogger::set_default_log_level(std::string level) {
    auto it = log_level_map_.find(level);
    if (it != log_level_map_.end()) {
        default_log_level_ = it->second;
        return true;
    } else {
        std::cerr << "Invalid log level: " << level << std::endl;
        return false;
    }
}

bool QuantLogger::set_logger_level(const std::string &name, const std::string &level) {
    auto it = log_level_map_.find(level);
    if (it != log_level_map_.end()) {
        return set_logger_level(name, it->second);
    } else {
        std::cerr << "Invalid log level: " << level << std::endl;
        return false;
    }
}

bool QuantLogger::set_logger_level(const std::string &name, const LevelEnum &level) {
    std::lock_guard<std::mutex> lock(logger_mutex_);
    if (logger_names_.find(name) == logger_names_.end()) {
        std::cerr << "Logger not found: " << name << std::endl;
        return false;
    }
    auto logger = spdlog::get(name);
    if (logger) {
        logger->set_level(level);
        return true;
    } else {
        std::cerr << "Failed to set log level for logger: " << name << std::endl;
        return false;
    }
}

bool QuantLogger::set_all_logger_level(const LevelEnum &level) {
    std::lock_guard<std::mutex> lock(logger_mutex_);
    for (const auto &name : logger_names_) {
        auto logger = spdlog::get(name);
        if (logger) {
            logger->set_level(level);
        } else {
            std::cerr << "Failed to set log level for logger: " << name << std::endl;
        }
    }
    return true;
}

bool QuantLogger::set_all_logger_level(const std::string &level) {
    auto it = log_level_map_.find(level);
    if (it != log_level_map_.end()) {
        return set_all_logger_level(it->second);
    } else {
        std::cerr << "Invalid log level: " << level << std::endl;
        return false;
    }
}

// Generates the log file path based on the provided name
const std::string QuantLogger::get_log_path(const std::string &logname) {
    if (!std::filesystem::exists(quanttrader::kDefaultLogDir)) {
        std::filesystem::create_directories(quanttrader::kDefaultLogDir);
    }
    return logname.empty()
        ? fmt::format("{}/global.log", quanttrader::kDefaultLogDir)
        : fmt::format("{}/{}.log", quanttrader::kDefaultLogDir, logname);
}

LoggerPtr QuantLogger::get_logger(const std::string &name, const std::string &logname, const std::string &config_path) {
    auto logger = spdlog::get(name);
    if (logger) {
        return logger;
    }

    bool use_sync_config = false;
    LevelEnum level_config = default_log_level_;
    std::string sinks_config = quanttrader::logger::LOGGER_SINK_DAILY_FILE_TYPE;

    if (!config_path.empty()) {
        auto config_loader = quanttrader::luascript::LuaConfigLoader(config_path);
        if (!config_loader.load_config())
        {
            std::cerr << "Failed to load logger config from: " << config_path << std::endl;
            return nullptr;
        }

        std::string level_str = config_loader.get_string_value(logname, quanttrader::logger::LOGGER_LEVEL_FIELD);
        if (!level_str.empty()) {
            for (const auto &pair : log_level_map_) {
                if (pair.first == level_str) {
                    level_config = pair.second;
                    break;
                }
            }
        }

        use_sync_config = config_loader.get_bool_value(logname, quanttrader::logger::LOGGER_USE_SYNC_FIELD);
        sinks_config = config_loader.get_string_value(logname, quanttrader::logger::LOGGER_SINKS_FIELD);
    }

    std::stringstream ss(sinks_config);
    std::string key;
    std::string log_path = get_log_path(logname);
    std::vector<spdlog::sink_ptr> sinks;

    while (std::getline(ss, key, '.')) {
        if (key == quanttrader::logger::LOGGER_SINK_DAILY_FILE_TYPE) {
            auto sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_path, 0, 0);
            sink->set_pattern(quanttrader::kSpdLogPattern);
            sinks.push_back(sink);
        }
        else if (key == quanttrader::logger::LOGGER_SINK_BASIC_FILE_TYPE) {
            auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, false);
            sink->set_pattern(quanttrader::kSpdLogPattern);
            sinks.emplace_back(sink);
        }
        else if (key == quanttrader::logger::LOGGER_SINK_CONSOLE_TYPE) {
            auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            sink->set_pattern(quanttrader::kSpdLogPattern);
            sinks.emplace_back(sink);
        }
        else if (key == quanttrader::logger::LOGGER_SINK_OSTREAM_TYPE) {
            auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout, true);
            sink->set_pattern(quanttrader::kSpdLogPattern);
            sinks.emplace_back(sink);
        }
        else {
            std::cerr << "Create logger" << name << " log file path " << logname << " Unknown sink type: " << key << std::endl;
        }
    }

    if (logname == "global"){
        // set the default log level
        default_log_level_ = level_config;
    }

    if (use_sync_config) {
        auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
        logger->set_level(level_config);
        spdlog::register_logger(logger);
        std::lock_guard<std::mutex> lock(logger_mutex_);
        logger_names_.insert(name);
        return logger;
    } else {
        init_thread_pool();
        auto logger = std::make_shared<spdlog::async_logger>(name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        logger->set_level(level_config);
        spdlog::register_logger(logger);
        std::lock_guard<std::mutex> lock(logger_mutex_);
        logger_names_.insert(name);
        return logger;
    }
}

bool QuantLogger::remove_logger(const std::string &name) {
    std::lock_guard<std::mutex> lock(logger_mutex_);
    auto it = logger_names_.find(name);
    if (it != logger_names_.end()) {
        spdlog::drop(name);
        logger_names_.erase(it);
        return true;
    } else {
        std::cerr << "Logger not found: " << name << std::endl;
        return false;
    }
}

void QuantLogger::print_message(const std::string &&msg) {
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_t t = now / 1000;

    tm timeInfo;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&timeInfo, &t);
#else
    localtime_r(&t, &timeInfo);
#endif
    fmt::print("[{}.{:02d}.{:02d} {:02d}:{:02d}:{:02d}] ", timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, 
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    fmt::print("NOT INIT! {}", msg);
    #ifdef _WIN32
        fmt::print("\r\n");
    #else
        fmt::print("\n");
    #endif
}

LoggerPtr get_common_rotation_logger(const std::string &name, const std::string &logfile) {
    return QuantLogger::get_logger(name, logfile);
}

} // namespace logger
} // namespace quanttrader
