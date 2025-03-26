#include "strategy_base.h"
#include "data/bar_struct.h"
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

void StrategyBase::on_data(const std::map<std::string, std::optional<data::BarStruct>>& data_map) {
    // Process each data feed and trigger appropriate callbacks
    for (const auto& [data_name, bar_opt] : data_map) {
        if (bar_opt.has_value()) {
            on_bar(data_name, bar_opt.value());
        }
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