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
 * Uses incremental calculation for improved performance.
 */
class SlopeStrategy : public StrategyBase {
public:
    SlopeStrategy(StrategyCreateFuncParemType params);
    ~SlopeStrategy() override = default;

    // Strategy initialization
    bool initialize() override;
    
    // Event handlers 
    void on_bar(const std::string& data_name, const data::BarSeries& bar_series, bool day_change, bool hour_change, bool minute_change) override;
    
protected:
    // Implementation of the next method required by StrategyBase
    void next() override;

private:
    // Strategy parameters
    int slow_ma_period_ = 20;
    int fast_ma_period_ = 5;
    
    // Internal state
    bool in_position_ = false;
    std::vector<double> slow_ma_values_;
    std::vector<double> fast_ma_values_;
    
    // Data storage for moving averages
    std::deque<double> prices_;
    double fast_ma_ = 0.0;
    double slow_ma_ = 0.0;
    bool position_open_ = false;
    
    // Cache for performance optimization
    // Keeps track of the last processed size to avoid redundant calculations
    size_t last_processed_size_ = 0;
    
    // Cached copy of all close prices to avoid copying data for each calculation
    std::vector<double> cached_close_prices_;
    
    // Helper methods
    double calculate_ma(int period) const;
    void update_moving_averages();
    
    /**
     * @brief Efficient calculation of moving averages using TA-Lib with caching
     * 
     * This method implements a caching strategy that:
     * 1. Stores previously calculated values to avoid redundant calculations
     * 2. Only calculates new values for new data points
     * 3. Uses the optimal TA-Lib lookback period for efficiency
     * 
     * @param bar_series The input bar series data
     */
    void calculate_moving_averages_incremental(const data::BarSeries& bar_series);
};

}
}