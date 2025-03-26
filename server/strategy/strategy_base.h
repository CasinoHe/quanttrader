#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <any>
#include <functional>
#include <memory>
#include <optional>
#include "logger/quantlogger.h"

namespace quanttrader {
namespace data {
struct BarStruct;
}
namespace strategy {

class StrategyBase;

using StrategyCreateFuncParemType = std::unordered_map<std::string, std::any>;
using StrategyCreateFuncType = std::function<std::shared_ptr<StrategyBase>(StrategyCreateFuncParemType &)>;

/**
 * @brief Base class for all trading strategies
 * 
 * This class provides the common functionality for all strategy implementations.
 * Derived classes should implement the next() method to define strategy logic.
 */
class StrategyBase {
public:
    StrategyBase(StrategyCreateFuncParemType params);
    virtual ~StrategyBase() = default;

    // Core event handlers
    virtual void on_tick(const std::string& data_name, const std::any& tick_data) {}
    virtual void on_bar(const std::string& data_name, const data::BarStruct& bar) {}
    virtual void on_trade(const std::string& symbol, double price, int quantity, bool is_buy) {}
    virtual void on_order(const std::string& order_id, const std::string& status) {}
    virtual void on_data(const std::map<std::string, std::optional<data::BarStruct>>& data_map);
    
    // Lifecycle methods
    virtual bool initialize();
    virtual bool on_start();
    virtual bool on_stop();
    
    // Access methods
    std::string get_name() const { return strategy_name_; }
    
    // Trading signals
    virtual void buy(const std::string& symbol, int quantity, double price = 0.0);
    virtual void sell(const std::string& symbol, int quantity, double price = 0.0);
    
protected:
    // Common logging
    quanttrader::log::LoggerPtr logger_;
    
    // Strategy parameters
    std::string strategy_name_ = "BaseStrategy";
    StrategyCreateFuncParemType params_;
    
    // Derived strategies should implement this method
    virtual void next() = 0;
    
    // Helper methods for accessing parameters
    template<typename T>
    T get_param(const std::string& name, const T& default_value) const {
        auto it = params_.find(name);
        if (it != params_.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast&) {
                logger_->warn("Parameter type mismatch for {}", name);
            }
        }
        return default_value;
    }
};

}
}