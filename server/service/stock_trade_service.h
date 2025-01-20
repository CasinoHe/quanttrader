#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include "concurrentqueue/blockingconcurrentqueue.h"

#include <memory>
#include <thread>

namespace quanttrader {

namespace broker {
class TwsClient;
struct RequestHeader;
struct ResponseHeader;
}

namespace service {

class StockTradeService : public ServiceBase<StockTradeService> {
public:
    bool prepare() override;
    void run() override;
    void stop() override;
    bool is_service_prepared() const override;

private:
    friend class Singleton<StockTradeService>;
    StockTradeService(const std::string_view config_path);
    ~StockTradeService() = default;

private:
    quanttrader::log::LoggerPtr logger_ {nullptr};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ {nullptr};
    std::shared_ptr<void> broker_service_ {nullptr};  // broker service, defined by configuration file
    std::shared_ptr<void> back_test_service_ {nullptr};  // back test service, defined by configuration file
};

}
}
