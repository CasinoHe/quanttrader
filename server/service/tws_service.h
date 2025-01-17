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

class TwsService : public ServiceBase<TwsService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;
    bool is_service_prepared() const override;

    bool push_request(std::shared_ptr<broker::GenericRequest> &request) {
        return request_queue_->enqueue(request);
    }
    void set_response_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::GenericResponse>>> response_queue) {
        response_queue_ = response_queue;
    }

private:
    friend class Singleton<TwsService>;
    TwsService(const std::string_view config_path);
    ~TwsService() = default;

    // start the tws client thread, connect to tws and request data
    void run_tws();
    void run_request(std::atomic<int> &tws_version);
    void run_response(std::atomic<int> &tws_version);
    void run_monitor(std::atomic<int> &tws_version);
    bool update_config(std::atomic<int> &tws_version);

    void keep_alive();

    // init data
    void init_after_connected();

private:
    quanttrader::log::LoggerPtr logger_ = nullptr;
    std::shared_ptr<broker::TwsClient> client_ = nullptr;
    std::atomic<bool> stop_flag_ = false;

    // all the threads
    std::shared_ptr<std::thread> client_thread_ = nullptr;

    // request and response queue
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::GenericRequest>>> request_queue_ = nullptr;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::GenericResponse>>> response_queue_ = nullptr;
    // std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResErrorMsg>>> error_queue_{nullptr};
    std::chrono::milliseconds retry_interval_{5000};
    std::chrono::milliseconds wait_timeout_{10};
    std::chrono::milliseconds update_config_interval_{60000};
};

}
}