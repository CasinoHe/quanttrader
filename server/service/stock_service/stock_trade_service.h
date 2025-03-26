#pragma once

#include "service/service.h"
#include "logger/quantlogger.h"

#include <memory>
#include <thread>
#include <vector>

namespace quanttrader {

namespace broker {
class BrokerProvider;
}
namespace data {
namespace provider {
class DataProvider;
}
}

namespace cerebro {
class CerebroBase;
}

namespace strategy {
class StrategyBase;
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

    bool prepare_data_series();
    bool prepare_cerebro();
    bool prepare_strategyes();

private:
    quanttrader::log::LoggerPtr logger_ {nullptr};
    std::shared_ptr<broker::BrokerProvider> broker_provider_ {nullptr};  // broker provider, defined by configuration file
    std::unordered_map<std::string, std::shared_ptr<cerebro::CerebroBase>> cerebro_map_ {};  // cerebro, defined by configuration file
    std::vector<std::shared_ptr<data::provider::DataProvider>> data_providers_ {};
    std::unordered_map<std::string, std::shared_ptr<strategy::StrategyBase>> strategy_map_ {};
};

}
}
