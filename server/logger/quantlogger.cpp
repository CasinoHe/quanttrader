
#include <fmt/core.h>

#include "quantlogger.h"
#include "spdlog/async.h"

namespace quanttrader
{
    namespace logger
    {
        // Initialize static members
        std::once_flag QuantLoggerMgr::thread_pool_flag_ {};
        LevelEnum QuantLoggerMgr::log_level_ = LevelEnum::info;
        std::mutex QuantLoggerMgr::logger_mutex_ {};

        QuantLoggerMgr g_logger_mgr = QuantLoggerMgr();

        QuantLoggerMgr::QuantLoggerMgr()
        {
            std::lock_guard<std::mutex> lock(QuantLoggerMgr::logger_mutex_);

            std::call_once(QuantLoggerMgr::thread_pool_flag_, [this] { init_thread_pool(); });
        }

        QuantLoggerMgr::~QuantLoggerMgr()
        {
            spdlog::shutdown();
        }

        void QuantLoggerMgr::init_thread_pool()
        {
            spdlog::init_thread_pool(quanttrader::kSpdLogQueueSize, quanttrader::kSpdLogThreadsCount);
        }

        const std::string QuantLoggerMgr::get_log_path(const std::string &logname)
        {
            if (logname.empty())
            {
                return fmt::format("{}/running.log", quanttrader::kDefaultLogDir);
            }
            else
            {
                return fmt::format("{}/{}.log", quanttrader::kDefaultLogDir, logname);
            }
        }

        LoggerPtr QuantLoggerMgr::get_file_logger(const std::string &name, const std::string &logfile,
                                      std::map<std::string, size_t> &max_size_map,
                                      std::map<std::string, int> &rotation_hour_map)
        {
            const std::string &path = get_log_path(logfile);

            if (max_size_map.find("max_size") != max_size_map.end())
            {
                size_t max_size = max_size_map["max_size"];

                uint16_t max_files = kSpdLogMaxRotatingFiles;
                if (max_size_map.find("max_files") == max_size_map.end())
                {
                    max_files = static_cast<uint16_t>(max_size_map["max_files"]);
                }

                std::cout << fmt::format("Create rotating file logger: {} path: {} max_size: {} max_files: {}", name, path, max_size, max_files) << std::endl;
                return get_logger<spdlog::sinks::rotating_file_sink_mt>(LogCategory::file, name, path, max_size, max_files);
            }
            else if (rotation_hour_map.find("rotation_hour") != rotation_hour_map.end() && rotation_hour_map.find("rotation_min") != rotation_hour_map.end())
            {
                int rotation_hour = rotation_hour_map["rotation_hour"];
                int rotation_min = rotation_hour_map["rotation_min"];

                uint16_t max_files = kSpdLogMaxRotatingFiles;
                if (max_size_map.find("max_files") == max_size_map.end())
                {
                    max_files = static_cast<uint16_t>(max_size_map["max_files"]);
                }

                std::cout << fmt::format("Create daily file logger: {} path: {} rotation_hour: {} rotation_min: {} max_files: {}", name, path, rotation_hour, rotation_min, max_files) << std::endl;
                return get_logger<spdlog::sinks::daily_file_sink_mt>(LogCategory::file, name, path, rotation_hour, rotation_min, false, max_files);
            }
            else
            {
                std::cout << "Create common file logger" << name << " path: " << path << std::endl;
                return get_logger<spdlog::sinks::basic_file_sink_mt>(LogCategory::file, name, path);
            }
        }

        LoggerPtr QuantLoggerMgr::get_console_logger(const std::string &name)
        {
            return get_logger<spdlog::sinks::stdout_color_sink_mt>(LogCategory::console, name);
        }
    }
} // namespace quanttrader