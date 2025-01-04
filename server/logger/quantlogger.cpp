#include "quantlogger.h"
#include "spdlog/async.h"
#include "common/consts.h"

namespace quanttrader
{
    namespace logger
    {
        // Initialize static members
        std::once_flag QuantLoggerMgr::thread_pool_flag_ {};
        LevelEnum QuantLoggerMgr::log_level_ = LevelEnum::info;

        QuantLoggerMgr g_logger_mgr = QuantLoggerMgr();

        QuantLoggerMgr::QuantLoggerMgr()
        {
            std::call_once(QuantLoggerMgr::thread_pool_flag_, [this] { init_thread_pool(); });
        }

        QuantLoggerMgr::~QuantLoggerMgr()
        {
            spdlog::shutdown();
        }

        LoggerPtr QuantLoggerMgr::get_logger(const std::string &name)
        {
            return spdlog::get(name);
        }

        void QuantLoggerMgr::init_thread_pool()
        {
            spdlog::init_thread_pool(quanttrader::kSpdLogQueueSize, quanttrader::kSpdLogThreadsCount);
        }
    }
} // namespace quanttrader