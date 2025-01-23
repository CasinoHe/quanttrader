#pragma once

#include "runner_base.h"

namespace quanttrader {
namespace runner {

class StockRunner : public Runner {
public:
    StockRunner(RunnerParamsType params): Runner(STOCK_RUNNER_NAME, params) {
    }

    virtual ~StockRunner() {};

    void on_tick() override;
    void on_bar() override;

protected:
    void on_finished() override;
    void on_init() override;
    void on_start() override;
    void run_frame() override;

private:
};

}
}