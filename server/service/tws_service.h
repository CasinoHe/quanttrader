#pragma once

#include "service/service.h"
#include "service/service_consts.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
#include <string>
#include <memory>
#include <thread>
#include <atomic>

namespace quanttrader {
namespace broker {
class TwsClient;
struct RequestHeader;
struct ResponseHeader;
struct ResErrorMsg;
}
namespace service {

class TwsService : public ServiceBase<TwsService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;
    bool is_service_prepared() const override;

    TickerId push_request(std::shared_ptr<broker::RequestHeader> &&request) {
        TickerId request_id = broker::TwsClient::next_request_id();
        request->request_id = request_id;
        bool result = request_queue_->enqueue(request);
        if (!result) {
            logger_->error("Cannot push the request to the queue.");
            return -1;
        } else {
            return request_id;
        }
    }

    void set_response_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue) {
        response_queue_ = response_queue;
    }
    void set_request_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue) {
        request_queue_ = request_queue;
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
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue_ = nullptr;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ = nullptr;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResErrorMsg>>> error_queue_{nullptr};
    std::chrono::milliseconds retry_interval_{kDefaultRetryInterval};
    std::chrono::milliseconds wait_timeout_{kDefaultWaitTimeout};
    std::chrono::milliseconds update_config_interval_{kDefaultUpdateConfigInterval};
};

}
}