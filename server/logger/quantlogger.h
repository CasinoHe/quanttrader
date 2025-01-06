#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/pattern_formatter.h"

#ifdef __LINUX__
#include "spdlog/sinks/syslog_sink.h"
#endif

#include "common/consts.h"

#include <unordered_map>
#include <thread>
#include <mutex>
#include <iostream>
#include <map>
#include <memory>

namespace quanttrader
{
namespace logger
{
    using LevelEnum = spdlog::level::level_enum;
    using LoggerPtr = std::shared_ptr<spdlog::logger>;

    class QuantLoggerMgr
    {
        public:
            QuantLoggerMgr();
            ~QuantLoggerMgr();
        private:
            QuantLoggerMgr(const QuantLoggerMgr &other) = delete;
            QuantLoggerMgr(QuantLoggerMgr &&other) = delete;
            QuantLoggerMgr &operator=(const QuantLoggerMgr &other) = delete;

        public:
            inline void set_log_level(const LevelEnum level) { log_level_ = level; }
            const std::string get_log_path(const std::string &logname);

            /* AIGC Function explaination:
            */
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
                    auto formattor = std::make_unique<spdlog::pattern_formatter>(quanttrader::kSpdLogPattern);
                    logger = spdlog::create_async_nb<Sink>(name, std::forward<SinkArgs>(sink_args)...);
                    auto sink = std::make_shared<Sink>(std::forward<SinkArgs>(sink_args)...);
                    sink->set_formatter(formattor->clone());
                    sink->set_level(log_level_);
                    logger->sinks().push_back(sink);

                    if (with_stdout)
                    {
                        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                        stdout_sink->set_formatter(formattor->clone());
                        stdout_sink->set_level(log_level_);
                        logger->sinks().push_back(stdout_sink);
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    return nullptr;
                }
            }

            LoggerPtr get_file_logger(const std::string &name, const std::string &logfile, bool with_stdout = false,
                                      std::shared_ptr<std::map<std::string, size_t>> max_size_map = nullptr,
                                      std::shared_ptr<std::map<std::string, int>> rotation_hour_map = nullptr);

            LoggerPtr get_console_logger(const std::string &name);
            LoggerPtr get_file_and_console_logger(const std::string &name, const std::string &logfile,
                                                  std::shared_ptr<std::map<std::string, size_t>> max_size_map = nullptr,
                                                  std::shared_ptr<std::map<std::string, int>> rotation_hour_map = nullptr);

        private:
            void init_thread_pool();

        private:
            static LevelEnum log_level_;
            static std::once_flag thread_pool_flag_;
            static std::mutex logger_mutex_;
    };

    extern QuantLoggerMgr g_logger_mgr;
    extern LoggerPtr g_logger;

    // ---- Automatically script interface generation begin ----

    // -- Function:
    // ---- Name: 
    //      get_logger
    // ---- Params: 
    //      const std::string &name

    // ---- Automatically script interface generation end ----

} // namespace logger
} // namespace quanttrader