#include "strategy_base.h"
#include "data/common/data_struct.h"
#include <sstream>
#include <iomanip>
#include <chrono>

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
    current_time_ = 0;
    for (const auto& [data_name, bar_series] : bar_series_map) {
        if (!bar_series.close.empty()) {
            on_bar(data_name, bar_series, day_change, hour_change, minute_change);
            last_prices_[data_name] = bar_series.close.back();
            current_time_ = std::max(current_time_, bar_series.start_time.back());
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

void StrategyBase::set_broker(std::shared_ptr<broker::AbstractBroker> broker) {
    broker_ = broker;
    
    if (broker_) {
        // Register callbacks to receive broker events
        broker_->register_order_status_callback([this](const broker::Order& order) {
            on_order_status(order);
        });
        
        broker_->register_trade_callback([this](const broker::Trade& trade) {
            on_trade_execution(trade);
        });
        
        broker_->register_position_callback([this](const broker::Position& position) {
            on_position_update(position);
        });
        
        broker_->register_account_callback([this](const broker::AccountInfo& account) {
            on_account_update(account);
        });
        
        logger_->info("Broker integration enabled for strategy: {}", strategy_name_);
    }
}

long StrategyBase::buy(const std::string& symbol, double quantity, double price, broker::OrderType type) {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return -1;
    }
    
    if (quantity <= 0) {
        logger_->warn("Buy order rejected: quantity must be positive ({})", quantity);
        return -1;
    }
    
    long order_id = broker_->place_order(symbol, broker::OrderSide::BUY, type, quantity, price);
    
    if (order_id > 0) {
        logger_->info("BUY ORDER PLACED: {} {} {} @ {} (Order ID: {})", 
                     strategy_name_, quantity, symbol, price, order_id);
    }
    
    return order_id;
}

long StrategyBase::sell(const std::string& symbol, double quantity, double price, broker::OrderType type) {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return -1;
    }
    
    if (quantity <= 0) {
        logger_->warn("Sell order rejected: quantity must be positive ({})", quantity);
        return -1;
    }
    
    long order_id = broker_->place_order(symbol, broker::OrderSide::SELL, type, quantity, price);
    
    if (order_id > 0) {
        logger_->info("SELL ORDER PLACED: {} {} {} @ {} (Order ID: {})", 
                     strategy_name_, quantity, symbol, price, order_id);
    }
    
    return order_id;
}

long StrategyBase::buy_limit(const std::string& symbol, double quantity, double price) {
    return buy(symbol, quantity, price, broker::OrderType::LIMIT);
}

long StrategyBase::sell_limit(const std::string& symbol, double quantity, double price) {
    return sell(symbol, quantity, price, broker::OrderType::LIMIT);
}

long StrategyBase::buy_stop(const std::string& symbol, double quantity, double stop_price) {
    return buy(symbol, quantity, stop_price, broker::OrderType::STOP);
}

long StrategyBase::sell_stop(const std::string& symbol, double quantity, double stop_price) {
    return sell(symbol, quantity, stop_price, broker::OrderType::STOP);
}

broker::Position StrategyBase::get_position(const std::string& symbol) const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return broker::Position(symbol, 0.0, 0.0);
    }
    
    return broker_->get_position(symbol);
}

std::map<std::string, broker::Position> StrategyBase::get_all_positions() const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return {};
    }
    
    return broker_->get_all_positions();
}

bool StrategyBase::close_position(const std::string& symbol, double quantity) {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return false;
    }
    
    return broker_->close_position(symbol, quantity);
}

bool StrategyBase::cancel_order(long order_id) {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return false;
    }
    
    return broker_->cancel_order(order_id);
}

std::vector<broker::Order> StrategyBase::get_orders(const std::string& symbol) const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return {};
    }
    
    return broker_->get_orders(symbol);
}

std::vector<broker::Order> StrategyBase::get_open_orders(const std::string& symbol) const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return {};
    }
    
    return broker_->get_open_orders(symbol);
}

broker::AccountInfo StrategyBase::get_account_info() const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return broker::AccountInfo();
    }
    
    return broker_->get_account_info();
}

double StrategyBase::get_cash() const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return 0.0;
    }
    
    return broker_->get_cash();
}

double StrategyBase::get_equity() const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return 0.0;
    }
    
    return broker_->get_equity();
}

double StrategyBase::get_buying_power() const {
    if (!broker_) {
        logger_->error("No broker set for strategy: {}", strategy_name_);
        return 0.0;
    }
    
    return broker_->get_buying_power();
}

void StrategyBase::buy_legacy(const std::string& symbol, int quantity, double price) {
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
    
    logger_->info("LEGACY BUY SIGNAL: {} {} @ {}", symbol, quantity, price_str);

    double trade_price = price;
    if (trade_price <= 0) {
        auto it = last_prices_.find(symbol);
        if (it != last_prices_.end()) {
            trade_price = it->second;
        }
    }
    if (trade_price > 0) {
        for (auto& obs : observers_) {
            obs->record_trade(current_time_, symbol, quantity, trade_price, true);
        }
    }
}

void StrategyBase::sell_legacy(const std::string& symbol, int quantity, double price) {
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
    
    logger_->info("LEGACY SELL SIGNAL: {} {} @ {}", symbol, quantity, price_str);

    double trade_price = price;
    if (trade_price <= 0) {
        auto it = last_prices_.find(symbol);
        if (it != last_prices_.end()) {
            trade_price = it->second;
        }
    }
    if (trade_price > 0) {
        for (auto& obs : observers_) {
            obs->record_trade(current_time_, symbol, quantity, trade_price, false);
        }
    }
}

}
}