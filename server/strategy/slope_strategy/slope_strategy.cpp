#include "slope_strategy.h"
#include "data/common/data_struct.h"
#include "ta_libc.h" // TA-Lib header
#include <numeric>
#include <algorithm>

namespace quanttrader {
namespace strategy {

SlopeStrategy::SlopeStrategy(StrategyCreateFuncParemType params)
    : StrategyBase(params) {
    // Extract strategy parameters
    slow_ma_period_ = get_param<int>("slow_ma_period", 20);
    fast_ma_period_ = get_param<int>("fast_ma_period", 5);
    
    logger_->info("Created SlopeStrategy for symbol: {}, with fast MA: {}, slow MA: {}", 
                 symbol_, fast_ma_period_, slow_ma_period_);
}

bool SlopeStrategy::initialize() {
    bool result = StrategyBase::initialize();
    if (result) {
        logger_->info("Initialized SlopeStrategy for symbol: {}", symbol_);
    }
    return result;
}

void SlopeStrategy::on_bar(const std::string& data_name, const data::BarSeries& bar_series) {
    // Extract the symbol from data_name by finding the part after the last underscore
    std::string bar_symbol = data_name;
    size_t last_underscore_pos = data_name.find_last_of('_');
    if (last_underscore_pos != std::string::npos) {
        bar_symbol = data_name.substr(last_underscore_pos + 1);
    }
    
    // Check if this is the data feed we're interested in by comparing with symbol_
    if (bar_symbol != symbol_) {
        logger_->debug("Ignoring bar data for symbol: {} (expected: {})", bar_symbol, symbol_);
        return;
    }
    
    // Make sure we have enough data
    if (bar_series.close.size() < static_cast<size_t>(slow_ma_period_)) {
        logger_->debug("Not enough data yet for calculating indicators: {} < {}", 
                      bar_series.close.size(), slow_ma_period_);
        return;
    }
    
    // Calculate moving averages using TA-Lib directly with the provided BarSeries
    int begin_idx = 0;
    int num_elements = 0;
    
    // Vectors to store TA-Lib output
    std::vector<double> slow_ma(bar_series.close.size());
    std::vector<double> fast_ma(bar_series.close.size());
    
    // Use TA-Lib to calculate Simple Moving Averages
    TA_RetCode slow_result = TA_MA(
        0,                          // startIdx
        static_cast<int>(bar_series.close.size() - 1),  // endIdx
        bar_series.close.data(),    // inReal - direct access to the close price array!
        slow_ma_period_,            // optInTimePeriod
        TA_MAType_SMA,             // optInMAType
        &begin_idx,                // outBegIdx
        &num_elements,             // outNBElement
        slow_ma.data()             // outReal
    );
    
    TA_RetCode fast_result = TA_MA(
        0,                          // startIdx
        static_cast<int>(bar_series.close.size() - 1),  // endIdx
        bar_series.close.data(),    // inReal - direct access to the close price array!
        fast_ma_period_,           // optInTimePeriod
        TA_MAType_SMA,             // optInMAType
        &begin_idx,                // outBegIdx
        &num_elements,             // outNBElement
        fast_ma.data()             // outReal
    );
    
    if (slow_result != TA_SUCCESS || fast_result != TA_SUCCESS) {
        logger_->error("Error calculating moving averages with TA-Lib");
        return;
    }
    
    // Store the latest MA values for use in the next() method
    slow_ma_values_ = std::move(slow_ma);
    fast_ma_values_ = std::move(fast_ma);
    
    // Log the current values
    int last_idx = static_cast<int>(slow_ma_values_.size()) - 1;
    if (last_idx >= 0) {
        logger_->debug("Latest values - Close: {}, Fast MA: {}, Slow MA: {}", 
                      bar_series.close.back(), 
                      fast_ma_values_[last_idx], 
                      slow_ma_values_[last_idx]);
    }
}

void SlopeStrategy::next() {
    // Skip if we don't have enough data
    if (slow_ma_values_.empty() || fast_ma_values_.empty()) {
        return;
    }
    
    // Get the latest values
    int last_idx = static_cast<int>(slow_ma_values_.size()) - 1;
    double current_fast_ma = fast_ma_values_[last_idx];
    double current_slow_ma = slow_ma_values_[last_idx];
    
    // Simple crossover strategy
    if (current_fast_ma > current_slow_ma && !in_position_) {
        // Fast MA crossed above slow MA - BUY signal
        buy(symbol_, 100);
        in_position_ = true;
        logger_->info("BUY SIGNAL: Fast MA {} crossed above Slow MA {}", 
                     current_fast_ma, current_slow_ma);
    }
    else if (current_fast_ma < current_slow_ma && in_position_) {
        // Fast MA crossed below slow MA - SELL signal
        sell(symbol_, 100);
        in_position_ = false;
        logger_->info("SELL SIGNAL: Fast MA {} crossed below Slow MA {}", 
                     current_fast_ma, current_slow_ma);
    }
}

}  // namespace strategy
}  // namespace quanttrader