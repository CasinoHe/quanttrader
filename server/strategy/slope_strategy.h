#pragma once

#include "strategy_base.h"
#include "data/common/data_struct.h"
#include <deque>
#include <vector>
#include <string>

namespace quanttrader {
namespace strategy {

/**
 * @brief Simple moving average crossover strategy
 * 
 * This strategy buys when a fast moving average crosses above a slow moving average,
 * and sells when the fast moving average crosses below the slow moving average.
 */
class SlopeStrategy : public StrategyBase {
public:
    SlopeStrategy(StrategyCreateFuncParemType params);
    ~SlopeStrategy() override = default;

    // Strategy initialization
    bool initialize() override;
    
    // Event handlers 
    void on_bar(const std::string& data_name, const data::BarStruct& bar) override;
    
protected:
    // Implementation of the next method required by StrategyBase
    void next() override;

private:
    // Strategy parameters
    int fast_period_ = 20;
    int slow_period_ = 50;
    std::string symbol_;
    
    // Data storage for moving averages
    std::deque<double> prices_;
    double fast_ma_ = 0.0;
    double slow_ma_ = 0.0;
    bool position_open_ = false;
    
    // Helper methods
    double calculate_ma(int period) const;
    void update_moving_averages();
};

}
}