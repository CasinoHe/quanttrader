#pragma once

#include "service.h"
#include "logger/quantlogger.h"
// #include "concurrentqueue/blockingconcurrentqueue.h"

#include <memory>
#include <thread>

namespace quanttrader {

namespace broker {
class BrokerProvider;
}

namespace cerebro {
class Cerebro;
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
    // std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::RequestHeader>>> request_queue_ {nullptr};
    // std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>> response_queue_ {nullptr};
    std::shared_ptr<broker::BrokerProvider> broker_provider_ {nullptr};  // broker provider, defined by configuration file
    std::shared_ptr<cerebro::Cerebro> cerebro_ {nullptr};  // cerebro, defined by configuration file
};

}
}
