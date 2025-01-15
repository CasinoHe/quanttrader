#pragma once

#include "service.h"
#include "logger/quantlogger.h"
#include <string>
#include <memory>

namespace quanttrader {
namespace broker {
class TwsClient;
}
namespace service {

class StrategyService : public ServiceBase<StrategyService> {
public:
    void run() override;
    bool prepare() override;
    bool is_service_prepared() const override;

private:
    friend class Singleton<StrategyService>;
    StrategyService(const std::string_view config_path);
    ~StrategyService() = default;

    quanttrader::log::LoggerPtr logger_ = nullptr;
    std::shared_ptr<broker::TwsClient> client_ = nullptr;
};

}
}