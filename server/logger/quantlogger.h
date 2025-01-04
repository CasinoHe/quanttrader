#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <thread>

namespace quanttrader
{
namespace logger
{
    using LevelEnum = spdlog::level::level_enum;
    using LoggerPtr = std::shared_ptr<spdlog::logger>;

    enum class LogCategory : int
    {
        file = 1,
        syslog = 2,
        console = 3,
    };

    // reflection to log category

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
            static LoggerPtr get_logger(const std::string &name);

        private:
            void init_thread_pool();

        private:
            static LevelEnum log_level_;
            static std::once_flag thread_pool_flag_;
    };

    extern QuantLoggerMgr g_logger_mgr;

} // namespace logger
} // namespace quanttrader