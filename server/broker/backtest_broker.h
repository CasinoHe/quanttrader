#pragma once

#include "abstract_broker.h"
#include <chrono>

namespace quanttrader {
namespace broker {

/**
 * @brief A simple backtesting broker implementation
 * 
 * This broker simulates trading operations for backtesting purposes.
 * It doesn't connect to any real broker but maintains all the necessary
 * state for position and order management.
 */
class BacktestBroker : public AbstractBroker {
public:
    explicit BacktestBroker(double starting_cash = 100000.0);
    ~BacktestBroker() override = default;

    // Enhanced order processing for backtesting
    void process_market_data(uint64_t timestamp, const std::map<std::string, double>& prices);
    
    // Backtesting specific settings
    void set_commission(double commission_per_trade) { commission_per_trade_ = commission_per_trade; }
    void set_slippage(double slippage_percent) { slippage_percent_ = slippage_percent; }
    void set_margin_requirements(double initial_margin, double maintenance_margin) {
        initial_margin_percent_ = initial_margin;
        maintenance_margin_percent_ = maintenance_margin;
    }
    
    // Override order placement to handle backtesting logic
    long place_order(const std::string& symbol, OrderSide side, OrderType type, 
                    double quantity, double price = 0.0, double stop_price = 0.0) override;
    long place_order(const std::string& symbol, OrderSide side, OrderType type, 
                    double quantity, double price, double stop_price, uint64_t timestamp) override;

private:
    // Backtesting parameters
    double commission_per_trade_ = 0.0;
    double slippage_percent_ = 0.0;
    double initial_margin_percent_ = 0.0;
    double maintenance_margin_percent_ = 0.0;
    
    // Internal methods for backtesting
    void process_pending_orders(uint64_t timestamp, const std::map<std::string, double>& prices);
    bool should_fill_order(const Order& order, double current_price) const;
    double calculate_fill_price(const Order& order, double current_price) const;
    double calculate_commission(const Order& order) const;
    void check_margin_requirements();
    
    uint64_t current_timestamp_ = 0;
};

} // namespace broker
} // namespace quanttrader
