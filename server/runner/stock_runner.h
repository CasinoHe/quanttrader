#pragma once

#include "runner_base.h"
#include <memory>
#include <vector>

namespace quanttrader {
namespace data {
class DataProvider;
}

namespace strategy {
class Strategy;
}

namespace runner {

class StockRunner : public Runner {
public:
    StockRunner(RunnerParamsType params): Runner(STOCK_RUNNER_NAME, params) {
    }

    virtual ~StockRunner() {};

    void on_tick() override;
    void on_bar() override;
    void on_trade() override;

protected:
    void on_finished() override;
    bool on_init() override;
    void on_start() override;
    void on_history() override;
    void run_frame() override;

private:
    std::vector<std::shared_ptr<quanttrader::data::DataProvider>> data_providers_;
    std::shared_ptr<quanttrader::strategy::Strategy> strategy_;
};

}
}