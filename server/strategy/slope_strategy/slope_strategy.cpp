#include "slope_strategy.h"
#include "data/common/data_struct.h"
#include "ta_libc.h" // TA-Lib header
#include <numeric>
#include <algorithm>

namespace quanttrader {
namespace strategy {

SlopeStrategy::SlopeStrategy(StrategyCreateFuncParemType params)
    : StrategyBase(params), last_processed_size_(0) {
    // Extract strategy parameters
    slow_ma_period_ = get_param<int>("slow_ma_period", 20);
    fast_ma_period_ = get_param<int>("fast_ma_period", 5);
    
    // Initialize cache variables
    cached_close_prices_.clear();
    slow_ma_values_.clear();
    fast_ma_values_.clear();
    
    logger_->info("Created SlopeStrategy for symbol: {}, with fast MA: {}, slow MA: {}", 
                 symbol_, fast_ma_period_, slow_ma_period_);
}

bool SlopeStrategy::initialize() {
    bool result = StrategyBase::initialize();
    if (result) {
        // Clear any existing cached data
        last_processed_size_ = 0;
        cached_close_prices_.clear();
        slow_ma_values_.clear();
        fast_ma_values_.clear();
        
        logger_->info("Initialized SlopeStrategy for symbol: {}", symbol_);
    }
    return result;
}

void SlopeStrategy::calculate_moving_averages_incremental(const data::BarSeries& bar_series) {
    // Get the current size of data
    size_t current_size = bar_series.close.size();
    
    // If this is the first calculation or we've somehow lost our cache
    if (cached_close_prices_.empty()) {
        // Initialize our cache with the current data
        cached_close_prices_ = std::vector<double>(bar_series.close.begin(), bar_series.close.end());
        last_processed_size_ = current_size;
        
        // Calculate moving averages for all data points
        int begin_idx_slow = 0;
        int begin_idx_fast = 0;
        int num_elements_slow = 0;
        int num_elements_fast = 0;
        
        // Allocate vectors for the results
        // We need to initialize with enough space for all values including the lookback period
        slow_ma_values_.resize(current_size, 0.0);
        fast_ma_values_.resize(current_size, 0.0);
        
        // Calculate slow MA
        std::vector<double> temp_slow_ma(current_size);
        TA_RetCode slow_result = TA_MA(
            0,                           // startIdx
            static_cast<int>(current_size - 1),  // endIdx
            bar_series.close.data(),     // inReal
            slow_ma_period_,             // optInTimePeriod
            TA_MAType_SMA,              // optInMAType
            &begin_idx_slow,            // outBegIdx
            &num_elements_slow,         // outNBElement
            temp_slow_ma.data()         // outReal
        );
        
        // Calculate fast MA
        std::vector<double> temp_fast_ma(current_size);
        TA_RetCode fast_result = TA_MA(
            0,                           // startIdx
            static_cast<int>(current_size - 1),  // endIdx
            bar_series.close.data(),     // inReal
            fast_ma_period_,            // optInTimePeriod
            TA_MAType_SMA,              // optInMAType
            &begin_idx_fast,            // outBegIdx
            &num_elements_fast,         // outNBElement
            temp_fast_ma.data()         // outReal
        );
        
        if (slow_result != TA_SUCCESS || fast_result != TA_SUCCESS) {
            logger_->error("Error calculating initial moving averages with TA-Lib");
            return;
        }
        
        // Copy values from temporary arrays to our member vectors
        // Account for the begin_idx offset that TA-Lib returns
        for (int i = 0; i < num_elements_slow; ++i) {
            slow_ma_values_[begin_idx_slow + i] = temp_slow_ma[i];
        }
        
        for (int i = 0; i < num_elements_fast; ++i) {
            fast_ma_values_[begin_idx_fast + i] = temp_fast_ma[i];
        }
        
        logger_->debug("Initial calculation of {} MA values complete (slow starts at {}, fast starts at {})", 
                     current_size, begin_idx_slow, begin_idx_fast);
    }
    // If we have new data points to process
    else if (current_size > last_processed_size_) {
        // How many new data points do we have?
        size_t new_points = current_size - last_processed_size_;
        logger_->debug("Processing {} new data points incrementally", new_points);
        
        // Add new close prices to our cache
        cached_close_prices_.insert(
            cached_close_prices_.end(), 
            bar_series.close.begin() + last_processed_size_, 
            bar_series.close.end()
        );
        
        // We'll use the efficient lookback method
        // For calculating new MA values, we need to include enough history
        // Use the maximum of (MA period) data points before the new ones
        int lookback = std::max(slow_ma_period_, fast_ma_period_);
        
        // Calculate start index for efficient calculation
        size_t input_start_idx = (last_processed_size_ > static_cast<size_t>(lookback)) ? 
                              (last_processed_size_ - lookback) : 0;
        
        // Number of input elements for the calculation
        size_t input_length = current_size - input_start_idx;
        
        // Resize our MA vectors to accommodate new data
        slow_ma_values_.resize(current_size, 0.0);
        fast_ma_values_.resize(current_size, 0.0);
        
        // Temporary vectors to hold calculation results
        std::vector<double> temp_slow_ma(input_length);
        std::vector<double> temp_fast_ma(input_length);
        
        int begin_idx_slow = 0;
        int begin_idx_fast = 0;
        int num_elements_slow = 0;
        int num_elements_fast = 0;
        
        // Use TA-Lib to calculate the new MA values
        TA_RetCode slow_result = TA_MA(
            0,                                    // startIdx
            static_cast<int>(input_length - 1),   // endIdx
            cached_close_prices_.data() + input_start_idx, // inReal - start from the needed history
            slow_ma_period_,                      // optInTimePeriod
            TA_MAType_SMA,                       // optInMAType
            &begin_idx_slow,                     // outBegIdx
            &num_elements_slow,                  // outNBElement
            temp_slow_ma.data()                  // outReal
        );
        
        TA_RetCode fast_result = TA_MA(
            0,                                    // startIdx
            static_cast<int>(input_length - 1),   // endIdx
            cached_close_prices_.data() + input_start_idx, // inReal - start from the needed history
            fast_ma_period_,                     // optInTimePeriod
            TA_MAType_SMA,                       // optInMAType
            &begin_idx_fast,                     // outBegIdx
            &num_elements_fast,                  // outNBElement
            temp_fast_ma.data()                  // outReal
        );
        
        if (slow_result != TA_SUCCESS || fast_result != TA_SUCCESS) {
            logger_->error("Error calculating incremental moving averages with TA-Lib");
            return;
        }
        
        // Update our MA arrays with the new values
        // We only need to update values from last_processed_size_ onwards
        for (int i = 0; i < num_elements_slow; ++i) {
            size_t global_idx = input_start_idx + begin_idx_slow + i;
            if (global_idx >= last_processed_size_ && global_idx < current_size) {
                slow_ma_values_[global_idx] = temp_slow_ma[i];
            }
        }
        
        for (int i = 0; i < num_elements_fast; ++i) {
            size_t global_idx = input_start_idx + begin_idx_fast + i;
            if (global_idx >= last_processed_size_ && global_idx < current_size) {
                fast_ma_values_[global_idx] = temp_fast_ma[i];
            }
        }
        
        // Update our last processed size
        last_processed_size_ = current_size;
        logger_->debug("Incremental calculation complete, now have {} MA values", current_size);
    }
    // If the size hasn't changed, no need to recalculate
    else if (current_size == last_processed_size_) {
        logger_->debug("No new data points to process");
    }
    // If the size somehow decreased, reset everything
    else {
        logger_->warn("Data size decreased from {} to {}, resetting cache", 
                    last_processed_size_, current_size);
        cached_close_prices_.clear();
        calculate_moving_averages_incremental(bar_series);
    }
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
    
    // Performance optimization - check if we need to recalculate at all
    if (bar_series.close.size() > last_processed_size_) {
        logger_->debug("Processing {} new data points", bar_series.close.size() - last_processed_size_);
        
        // Use the incremental calculation method
        calculate_moving_averages_incremental(bar_series);
        
        // Log the current values
        int last_idx = static_cast<int>(slow_ma_values_.size()) - 1;
        if (last_idx >= 0) {
            logger_->debug("Latest values - Close: {}, Fast MA: {}, Slow MA: {}", 
                          bar_series.close.back(), 
                          fast_ma_values_[last_idx], 
                          slow_ma_values_[last_idx]);
        }
    } else {
        logger_->debug("No new data points since last calculation, skipping calculation");
    }
}

void SlopeStrategy::next() {
    // Skip if we don't have enough data
    if (slow_ma_values_.empty() || fast_ma_values_.empty() || 
        slow_ma_values_.size() < 2 || fast_ma_values_.size() < 2) {
        return;
    }
    
    // Get the latest values
    int last_idx = static_cast<int>(slow_ma_values_.size()) - 1;
    double current_fast_ma = fast_ma_values_[last_idx];
    double current_slow_ma = slow_ma_values_[last_idx];
    
    // Get previous values for crossover detection
    double prev_fast_ma = fast_ma_values_[last_idx - 1];
    double prev_slow_ma = slow_ma_values_[last_idx - 1];
    
    // More precise crossover detection using both current and previous values
    bool crossover_up = (prev_fast_ma <= prev_slow_ma) && (current_fast_ma > current_slow_ma);
    bool crossover_down = (prev_fast_ma >= prev_slow_ma) && (current_fast_ma < current_slow_ma);
    
    // Simple crossover strategy with improved detection
    if (crossover_up && !in_position_) {
        // Fast MA crossed above slow MA - BUY signal
        buy(symbol_, 100);
        in_position_ = true;
        logger_->info("BUY SIGNAL: Fast MA {} crossed above Slow MA {}", 
                     current_fast_ma, current_slow_ma);
    }
    else if (crossover_down && in_position_) {
        // Fast MA crossed below slow MA - SELL signal
        sell(symbol_, 100);
        in_position_ = false;
        logger_->info("SELL SIGNAL: Fast MA {} crossed below Slow MA {}", 
                     current_fast_ma, current_slow_ma);
    }
}

}  // namespace strategy
}  // namespace quanttrader