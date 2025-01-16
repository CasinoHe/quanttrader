#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
#include <string>
#include <memory>
#include <thread>
#include <atomic>

namespace quanttrader {
namespace broker {
class TwsClient;
struct GenericRequest;
struct GenericResponse;
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
    void run_request(std::atomic<int> &tws_version);
    void run_response(std::atomic<int> &tws_version);
    void run_monitor(std::atomic<int> &tws_version);
    bool update_config(std::atomic<int> &tws_version);

    quanttrader::log::LoggerPtr logger_ = nullptr;
    std::shared_ptr<broker::TwsClient> client_ = nullptr;
    std::atomic<bool> stop_flag_ = false;

    // all the threads
    std::shared_ptr<std::thread> tws_thread_ = nullptr;
    std::shared_ptr<std::thread> strategy_thread_ = nullptr;

    // request and response queue
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::GenericRequest>> request_queue_;
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::GenericResponse>> response_queue_;
    std::chrono::milliseconds retry_interval_{5000};
    std::chrono::milliseconds wait_timeout_{10};
    std::chrono::milliseconds update_config_interval_{60000};
};

}
}