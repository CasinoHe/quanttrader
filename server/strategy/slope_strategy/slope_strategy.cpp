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
    symbol_ = get_param<std::string>("symbol", "AAPL");
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

void SlopeStrategy::on_bar(const std::string& data_name, const data::BarStruct& bar) {
    // This method is called with the most recent bar for backward compatibility
    logger_->debug("Received bar for {}: time={}, close={}", 
                  data_name, bar.time, bar.close);
}

void SlopeStrategy::on_data(const std::map<std::string, std::vector<std::optional<data::BarStruct>>>& data_map) {
    // Call the parent class implementation first to ensure proper setup
    StrategyBase::on_data(data_map);

    // Find our symbol in the data map
    auto it = data_map.find(symbol_);
    if (it == data_map.end()) {
        return; // Our symbol is not in the data map
    }
    
    const auto& bars = it->second;
    
    // Make sure we have enough data
    if (bars.size() < static_cast<size_t>(slow_ma_period_)) {
        logger_->debug("Not enough data yet for calculating indicators: {} < {}", 
                      bars.size(), slow_ma_period_);
        return;
    }

    // Convert to BarSeries for TA-Lib processing
    data::BarSeries series;
    
    // Reserve space for efficiency
    size_t valid_count = 0;
    for (const auto& bar_opt : bars) {
        if (bar_opt.has_value()) valid_count++;
    }
    
    series.start_time.reserve(valid_count);
    series.open.reserve(valid_count);
    series.high.reserve(valid_count);
    series.low.reserve(valid_count);
    series.close.reserve(valid_count);
    series.volume.reserve(valid_count);
    series.count.reserve(valid_count);
    
    // Fill the arrays from the valid bar data
    for (const auto& bar_opt : bars) {
        if (bar_opt.has_value()) {
            const auto& bar = bar_opt.value();
            series.start_time.push_back(bar.time);
            series.open.push_back(bar.open);
            series.high.push_back(bar.high);
            series.low.push_back(bar.low);
            series.close.push_back(bar.close);
            series.volume.push_back(bar.volume);
            series.count.push_back(bar.count);
        }
    }
    
    // Skip if we don't have enough valid data
    if (series.close.size() < static_cast<size_t>(slow_ma_period_)) {
        return;
    }

    // Calculate moving averages using TA-Lib
    int begin_idx = 0;
    int num_elements = 0;
    
    // Vectors to store TA-Lib output
    std::vector<double> slow_ma(series.close.size());
    std::vector<double> fast_ma(series.close.size());
    
    // Use TA-Lib to calculate Simple Moving Averages
    TA_RetCode slow_result = TA_MA(
        0,                          // startIdx
        static_cast<int>(series.close.size() - 1),  // endIdx
        series.close.data(),        // inReal
        slow_ma_period_,           // optInTimePeriod
        TA_MAType_SMA,             // optInMAType
        &begin_idx,                // outBegIdx
        &num_elements,             // outNBElement
        slow_ma.data()             // outReal
    );
    
    TA_RetCode fast_result = TA_MA(
        0,                          // startIdx
        static_cast<int>(series.close.size() - 1),  // endIdx
        series.close.data(),        // inReal
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
                      series.close.back(), 
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