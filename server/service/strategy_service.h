#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <thread>
#include <atomic>

namespace quanttrader {
namespace broker {
class TwsClient;
}
namespace service {

class StrategyService : public ServiceBase<StrategyService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;
    bool is_service_prepared() const override;

private:
    friend class Singleton<StrategyService>;
    StrategyService(const std::string_view config_path);
    ~StrategyService() = default;

    // start the tws client thread, connect to tws and request data
    void run_tws();

    quanttrader::log::LoggerPtr logger_ = nullptr;
    std::shared_ptr<broker::TwsClient> client_ = nullptr;
    std::shared_ptr<std::thread> tws_thread_ = nullptr;
    std::atomic<bool> stop_flag_ = false;
    int retry_milliseconds_ = 5000;
};

}
}