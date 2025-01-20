#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
#include "tws_service.h"

namespace quanttrader {
namespace service {

class BackTestService : public ServiceBase<BackTestService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;

private:
    friend class Singleton<BackTestService>;
    BackTestService(const std::string_view config_path);
    ~BackTestService() = default;

    quanttrader::log::LoggerPtr logger_ {nullptr};
    std::string config_path_;
    std::shared_ptr<TwsService> tws_service_ {nullptr};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ {nullptr};
};

}
}
