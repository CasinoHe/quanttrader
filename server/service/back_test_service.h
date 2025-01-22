#pragma once

#include "service/service.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
#include "service/service_consts.h"
#include <memory>
#include <unordered_map>
#include <tuple>
#include <thread>
#include <atomic>
#include <mutex>
#include <any>

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

struct BackTestServiceStruct {
    std::string config_key;
    int version;
    BackTestState state;
    BackTestState expected_state;
    std::string strategy_name;
    std::shared_ptr<std::unordered_map<std::string, std::any>> strategy_data;
    std::shared_ptr<std::thread> process;
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

    void run_back_test(std::shared_ptr<BackTestServiceStruct> back_test);
    void update_config();
    std::vector<std::string> handle_need_stop_process();
    void handle_need_start_process(const std::vector<std::string> &exclude_processes);
    void handle_need_restarting_process();
    void update_process_data(const std::string_view key);

private:
    std::chrono::milliseconds update_config_interval_{kDefaultUpdateConfigInterval};
    std::chrono::milliseconds wait_timeout_{kDefaultWaitBackTestTimeout};
    std::atomic<bool> stop_flag_ {false};
    quanttrader::log::LoggerPtr logger_ {nullptr};
    std::string config_path_ {};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ {nullptr};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue_ = nullptr;
    std::unordered_map<std::string, std::shared_ptr<BackTestServiceStruct>> back_test_process_;
    std::mutex back_test_process_mutex_;
};

}
}
