#include "slope_strategy.h"
#include "data/bar_struct.h"
#include <numeric>
#include <algorithm>

namespace quanttrader {
namespace strategy {

SlopeStrategy::SlopeStrategy(StrategyCreateFuncParemType params)
    : StrategyBase(params) {
    
    // Get strategy parameters
    fast_period_ = get_param<int>("fast_period", 20);
    slow_period_ = get_param<int>("slow_period", 50);
    symbol_ = get_param<std::string>("symbol", "");
    
    logger_->info("Created SlopeStrategy with fast_period={}, slow_period={}, symbol={}",
                fast_period_, slow_period_, symbol_);
}

bool SlopeStrategy::initialize() {
    StrategyBase::initialize();
    
    // Ensure slow period is greater than fast period
    if (fast_period_ >= slow_period_) {
        logger_->error("Fast period must be less than slow period");
        return false;
    }
    
    // Ensure symbol is set
    if (symbol_.empty()) {
        logger_->error("Symbol must be specified");
        return false;
    }
    
    logger_->info("Initialized SlopeStrategy");
    return true;
}

void SlopeStrategy::on_bar(const std::string& data_name, const data::BarStruct& bar) {
    // Only process data for our symbol
    if (data_name != symbol_) {
        return;
    }
    
    // Store the closing price
    prices_.push_back(bar.close);
    
    // Keep only enough data for our calculations
    if (prices_.size() > slow_period_) {
        prices_.pop_front();
    }
    
    // Update moving averages
    update_moving_averages();
}

void SlopeStrategy::next() {
    // Wait until we have enough data
    if (prices_.size() < slow_period_) {
        logger_->debug("Not enough data for SlopeStrategy, current size={}", prices_.size());
        return;
    }
    
    // Check for crossover (fast MA crosses above slow MA)
    static double prev_fast_ma = 0.0;
    static double prev_slow_ma = 0.0;
    
    bool crossover_up = prev_fast_ma <= prev_slow_ma && fast_ma_ > slow_ma_;
    bool crossover_down = prev_fast_ma >= prev_slow_ma && fast_ma_ < slow_ma_;
    
    if (crossover_up && !position_open_) {
        // Buy signal
        buy(symbol_, 100);  // Buy 100 shares at market price
        position_open_ = true;
        logger_->info("Buy signal triggered: fast_ma={:.2f}, slow_ma={:.2f}", fast_ma_, slow_ma_);
    }
    else if (crossover_down && position_open_) {
        // Sell signal
        sell(symbol_, 100);  // Sell 100 shares at market price
        position_open_ = false;
        logger_->info("Sell signal triggered: fast_ma={:.2f}, slow_ma={:.2f}", fast_ma_, slow_ma_);
    }
    
    // Store current values for next comparison
    prev_fast_ma = fast_ma_;
    prev_slow_ma = slow_ma_;
}

double SlopeStrategy::calculate_ma(int period) const {
    if (prices_.size() < period) {
        return 0.0;
    }
    
    auto end = prices_.end();
    auto start = end - period;
    
    double sum = std::accumulate(start, end, 0.0);
    return sum / period;
}

void SlopeStrategy::update_moving_averages() {
    // Calculate moving averages if we have enough data
    if (prices_.size() >= fast_period_) {
        fast_ma_ = calculate_ma(fast_period_);
    }
    
    if (prices_.size() >= slow_period_) {
        slow_ma_ = calculate_ma(slow_period_);
    }
}

}
}