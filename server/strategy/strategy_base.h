#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <any>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include "logger/quantlogger.h"
#include "data/common/data_struct.h"
#include "observer/observer_base.h"
#include "broker/abstract_broker.h"

namespace quanttrader {
namespace data {
struct BarStruct;
struct BarSeries;
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
    virtual void on_bar(const std::string& data_name, const data::BarSeries& bar_series, bool day_change, bool hour_change, bool minute_change) {}
    virtual void on_trade(const std::string& symbol, double price, int quantity, bool is_buy) {}
    virtual void on_order(const std::string& order_id, const std::string& status) {}
    
    // Broker event handlers
    virtual void on_order_status(const broker::Order& order) {}
    virtual void on_trade_execution(const broker::Trade& trade) {}
    virtual void on_position_update(const broker::Position& position) {}
    virtual void on_account_update(const broker::AccountInfo& account) {}
    
    /**
     * @brief Called when new data is available (optimized version)
     * 
     * This method receives cached TA-Lib compatible BarSeries directly from CerebroBase,
     * avoiding the need to recreate BarSeries objects on each call.
     * 
     * @param bar_series_map Map of data provider names to cached BarSeries (readonly)
     */
    virtual void on_data_series(const std::map<std::string, data::BarSeries>& bar_series_map, bool day_change, bool hour_change, bool minute_change);
    
    // Lifecycle methods
    virtual bool initialize();
    virtual bool on_start();
    virtual bool on_stop();

    void add_observer(std::shared_ptr<observer::ObserverBase> obs) { if (obs) observers_.push_back(obs); }
    
    // Broker integration
    void set_broker(std::shared_ptr<broker::AbstractBroker> broker);
    std::shared_ptr<broker::AbstractBroker> get_broker() const { return broker_; }
    
    // Access methods
    std::string get_name() const { return strategy_name_; }
    
    // Enhanced trading signals with broker integration
    virtual long buy(const std::string& symbol, double quantity, double price = 0.0, broker::OrderType type = broker::OrderType::MARKET);
    virtual long sell(const std::string& symbol, double quantity, double price = 0.0, broker::OrderType type = broker::OrderType::MARKET);
    virtual long buy_limit(const std::string& symbol, double quantity, double price);
    virtual long sell_limit(const std::string& symbol, double quantity, double price);
    virtual long buy_stop(const std::string& symbol, double quantity, double stop_price);
    virtual long sell_stop(const std::string& symbol, double quantity, double stop_price);
    
    // Position management
    virtual broker::Position get_position(const std::string& symbol) const;
    virtual std::map<std::string, broker::Position> get_all_positions() const;
    virtual bool close_position(const std::string& symbol, double quantity = 0.0);
    
    // Order management
    virtual bool cancel_order(long order_id);
    virtual std::vector<broker::Order> get_orders(const std::string& symbol = "") const;
    virtual std::vector<broker::Order> get_open_orders(const std::string& symbol = "") const;
    
    // Account information
    virtual broker::AccountInfo get_account_info() const;
    virtual double get_cash() const;
    virtual double get_equity() const;
    virtual double get_buying_power() const;
    
    // Legacy trading methods (for backward compatibility)
    virtual void buy_legacy(const std::string& symbol, int quantity, double price = 0.0);
    virtual void sell_legacy(const std::string& symbol, int quantity, double price = 0.0);
    
protected:
    // Common logging
    quanttrader::log::LoggerPtr logger_;
    
    // Strategy parameters
    std::string strategy_name_ = "BaseStrategy";
    std::string symbol_ = "";  // Added symbol as a base class member
    StrategyCreateFuncParemType params_;

    // key data debug switch
    bool log_data_ = false; // Added key data debug switch

    std::vector<std::shared_ptr<observer::ObserverBase>> observers_;
    uint64_t current_time_ = 0;
    std::unordered_map<std::string, double> last_prices_;
    
    // Broker integration
    std::shared_ptr<broker::AbstractBroker> broker_;

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