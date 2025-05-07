#include "strategy_base.h"
#include "data/common/data_struct.h"
#include <sstream>
#include <iomanip>

namespace quanttrader {
namespace strategy {

StrategyBase::StrategyBase(StrategyCreateFuncParemType params) : params_(params) {
    // Extract strategy name from parameters if available
    auto it = params_.find("strategy_name");
    if (it != params_.end()) {
        try {
            strategy_name_ = std::any_cast<std::string>(it->second);
        } catch (const std::bad_any_cast&) {
            strategy_name_ = "UnnamedStrategy";
        }
    }
    
    // Initialize logger
    logger_ = quanttrader::log::get_common_rotation_logger(strategy_name_, "strategy");
    logger_->info("Created strategy: {}", strategy_name_);
}

bool StrategyBase::initialize() {
    logger_->info("Initializing strategy: {}", strategy_name_);
    return true;
}

bool StrategyBase::on_start() {
    logger_->info("Starting strategy: {}", strategy_name_);
    return true;
}

bool StrategyBase::on_stop() {
    logger_->info("Stopping strategy: {}", strategy_name_);
    return true;
}

void StrategyBase::on_data(const std::map<std::string, std::vector<std::optional<data::BarStruct>>>& data_map) {
    // Process each data feed and trigger appropriate callbacks
    // For the most recent bar in each feed
    for (const auto& [data_name, bars] : data_map) {
        if (!bars.empty() && bars.back().has_value()) {
            // Pass the most recent bar to on_bar for backward compatibility
            on_bar(data_name, bars.back().value());
        }
    }
    
    // Convert the vector of BarStruct to BarSeries for TA-Lib compatibility
    std::map<std::string, data::BarSeries> bar_series_map;
    
    for (const auto& [data_name, bars] : data_map) {
        if (bars.empty()) {
            continue;
        }
        
        // Create a BarSeries instance for this data feed
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
        
        // Add to the map if we have valid data
        if (!series.close.empty()) {
            bar_series_map[data_name] = std::move(series);
        }
    }
    
    // Call the new on_bar_series method with TA-Lib compatible data
    if (!bar_series_map.empty()) {
        on_bar_series(bar_series_map);
    }
    
    // Call the next() method which derived strategies should implement
    next();
}

void StrategyBase::on_bar_series(const std::map<std::string, data::BarSeries>& bar_series_map) {
    // Default implementation is empty, derived classes should override this
    // method to implement TA-Lib based indicators and strategies
}

void StrategyBase::buy(const std::string& symbol, int quantity, double price) {
    if (quantity <= 0) {
        logger_->warn("Buy order rejected: quantity must be positive ({})", quantity);
        return;
    }
    
    std::string price_str;
    if (price > 0) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << price;
        price_str = oss.str();
    } else {
        price_str = "market";
    }
    
    logger_->info("BUY SIGNAL: {} {} @ {}", symbol, quantity, price_str);
    
    // In a real implementation, this would send the order to the broker
    // broker_->place_order(symbol, quantity, price, true);
}

void StrategyBase::sell(const std::string& symbol, int quantity, double price) {
    if (quantity <= 0) {
        logger_->warn("Sell order rejected: quantity must be positive ({})", quantity);
        return;
    }
    
    std::string price_str;
    if (price > 0) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << price;
        price_str = oss.str();
    } else {
        price_str = "market";
    }
    
    logger_->info("SELL SIGNAL: {} {} @ {}", symbol, quantity, price_str);
    
    // In a real implementation, this would send the order to the broker
    // broker_->place_order(symbol, quantity, price, false);
}

}
}