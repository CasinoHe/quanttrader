#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
#include <memory>
#include <unordered_map>
#include <tuple>
#include <thread>
#include <atomic>
#include <mutex>

namespace quanttrader {

namespace broker {
struct RequestHeader;
struct ResponseHeader;
}

namespace service {

enum class BackTestState {
    INIT = 0,
    WAIT_STARTING = 1,
    RUNNING = 2,
    WAIT_STOPPING = 3,
    STOPPED = 4,
};

class BackTestService : public ServiceBase<BackTestService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;

    void set_response_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue) { response_queue_ = response_queue; };
    void set_request_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue) { request_queue_ = request_queue; }

private:
    friend class Singleton<BackTestService>;
    BackTestService(const std::string_view config_path);
    ~BackTestService() = default;

    void run_back_test();
    void update_config();

private:
    std::atomic<bool> stop_flag_ {false};
    quanttrader::log::LoggerPtr logger_ {nullptr};
    std::string config_path_ {};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ {nullptr};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue_ = nullptr;
    std::unordered_map<std::string, std::tuple<BackTestState, std::shared_ptr<std::thread>>> back_test_threads_;
};

}
}
