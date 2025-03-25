#pragma once

#include "strategy_base.h"

namespace quanttrader {
namespace strategy {

class SlopeStrategy : public StrategyBase {
public:
    SlopeStrategy(StrategyCreateFuncParemType params) : StrategyBase(params) {
    }

    void on_tick() override {
    }

    void on_bar() override {
    }

    void on_trade() override {
    }
private:

};

}
}