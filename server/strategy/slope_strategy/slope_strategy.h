#pragma once

#include "strategy/strategy_base.h"
#include "data/common/data_struct.h"
#include <deque>
#include <vector>
#include <string>
#include <map>

namespace quanttrader {
namespace strategy {

/**
 * @brief Example strategy using TA-Lib for technical indicators
 * 
 * This strategy demonstrates how to use the BarSeries data structure with TA-Lib.
 * It calculates a simple moving average using TA-Lib and generates buy/sell signals.
 */
class SlopeStrategy : public StrategyBase {
public:
    SlopeStrategy(StrategyCreateFuncParemType params);
    ~SlopeStrategy() override = default;

    // Strategy initialization
    bool initialize() override;
    
    // Event handlers 
    void on_bar(const std::string& data_name, const data::BarSeries& bar_series) override;
    
protected:
    // Implementation of the next method required by StrategyBase
    void next() override;

private:
    // Strategy parameters
    int slow_ma_period_ = 20;
    int fast_ma_period_ = 5;
    std::string symbol_;
    
    // Internal state
    bool in_position_ = false;
    std::vector<double> slow_ma_values_;
    std::vector<double> fast_ma_values_;
    
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