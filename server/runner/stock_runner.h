#pragma once

#include "runner_base.h"

namespace quanttrader {
namespace runner {

class StockRunner : public RunnerBase {
public:
    StockRunner() = default;
    ~StockRunner() = default;

    void on_init() override;
    void on_start() override;
    void on_finished() override;
    void on_tick() override;
    void on_bar() override;

private:
};
}
}