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
    
    // Extract symbol from parameters if available
    auto symbol_it = params_.find("symbol");
    if (symbol_it != params_.end()) {
        try {
            symbol_ = std::any_cast<std::string>(symbol_it->second);
        } catch (const std::bad_any_cast&) {
            symbol_ = "";
            logger_->warn("Symbol parameter has incorrect type");
        }
    }

    // Check if the date output debug switch is set
    auto log_data_it = params_.find("log_data");
    if (log_data_it != params_.end()) {
        try {
            log_data_ = std::any_cast<bool>(log_data_it->second);
        } catch (const std::bad_any_cast&) {
            log_data_ = false; // Default to false if type is incorrect
            logger_->warn("log_data parameter has incorrect type, defaulting to false");
        }
    } else {
        log_data_ = false; // Default to false if not set
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

    
void StrategyBase::on_data_series(const std::map<std::string, data::BarSeries>& bar_series_map, bool day_change, bool hour_change, bool minute_change) {
    // Call on_bar with TA-Lib compatible data for each feed
    if (log_data_) {
        logger_->info("Processing data series for strategy: {}", strategy_name_);
    }
    for (const auto& [data_name, bar_series] : bar_series_map) {
        if (!bar_series.close.empty()) {
            on_bar(data_name, bar_series, day_change, hour_change, minute_change);
        }

        if (log_data_) {
            // Log the last bar of the series for debugging
            logger_->info("Data series {} last bar: {}", data_name, bar_series.last_one_to_string());
        }
    }
    if (log_data_) {
        logger_->info("Finished processing data series for strategy: {}", strategy_name_);
    }
    
    // Call the next() method which derived strategies should implement
    next();
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