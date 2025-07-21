#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include "logger/quantlogger.h"

namespace quanttrader {
namespace broker {

// Forward declarations
class BrokerProvider;

// Order states
enum class OrderStatus {
    PENDING,
    FILLED,
    PARTIALLY_FILLED,
    CANCELLED,
    REJECTED
};

// Order types
enum class OrderType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT
};

// Order side
enum class OrderSide {
    BUY,
    SELL
};

// Order structure
struct Order {
    long order_id;
    std::string symbol;
    OrderSide side;
    OrderType type;
    double quantity;
    double price;
    double stop_price;
    OrderStatus status;
    double filled_quantity;
    double remaining_quantity;
    uint64_t timestamp;
    std::string error_message;
    
    // Default constructor for std::unordered_map
    Order() : order_id(0), side(OrderSide::BUY), type(OrderType::MARKET), quantity(0.0), price(0.0), 
              stop_price(0.0), status(OrderStatus::PENDING), filled_quantity(0.0), 
              remaining_quantity(0.0), timestamp(0) {}
    
    Order(long id, const std::string& sym, OrderSide s, OrderType t, double qty, double p = 0.0, double sp = 0.0)
        : order_id(id), symbol(sym), side(s), type(t), quantity(qty), price(p), stop_price(sp),
          status(OrderStatus::PENDING), filled_quantity(0.0), remaining_quantity(qty), timestamp(0) {}
};

// Position structure
struct Position {
    std::string symbol;
    double quantity;        // Positive for long, negative for short
    double avg_price;       // Average entry price
    double unrealized_pnl;  // Unrealized P&L
    double realized_pnl;    // Realized P&L
    
    Position(const std::string& sym = "", double qty = 0.0, double price = 0.0)
        : symbol(sym), quantity(qty), avg_price(price), unrealized_pnl(0.0), realized_pnl(0.0) {}
        
    bool is_long() const { return quantity > 0; }
    bool is_short() const { return quantity < 0; }
    bool is_flat() const { return quantity == 0; }
};

// Trade execution structure
struct Trade {
    long order_id;
    std::string symbol;
    OrderSide side;
    double quantity;
    double price;
    uint64_t timestamp;
    double commission;
    
    Trade(long id, const std::string& sym, OrderSide s, double qty, double p, uint64_t ts, double comm = 0.0)
        : order_id(id), symbol(sym), side(s), quantity(qty), price(p), timestamp(ts), commission(comm) {}
};

// Account information
struct AccountInfo {
    double cash;
    double equity;
    double buying_power;
    double maintenance_margin;
    double initial_margin;
    double unrealized_pnl;
    double realized_pnl;
    
    AccountInfo(double initial_cash = 100000.0)
        : cash(initial_cash), equity(initial_cash), buying_power(initial_cash),
          maintenance_margin(0.0), initial_margin(0.0), unrealized_pnl(0.0), realized_pnl(0.0) {}
};

// Callback types
using BrokerOrderStatusCallback = std::function<void(const Order&)>;
using BrokerTradeCallback = std::function<void(const Trade&)>;
using PositionCallback = std::function<void(const Position&)>;
using AccountCallback = std::function<void(const AccountInfo&)>;

/**
 * Abstract broker class that manages trading operations, positions, and account information.
 * This class serves as the central hub for all trading activities and maintains the state
 * of cash, orders, and holdings.
 */
class AbstractBroker {
public:
    explicit AbstractBroker(double starting_cash = 100000.0);
    virtual ~AbstractBroker() = default;

    // Order management
    virtual long place_order(const std::string& symbol, OrderSide side, OrderType type, 
                           double quantity, double price = 0.0, double stop_price = 0.0);
    virtual bool cancel_order(long order_id);
    virtual bool modify_order(long order_id, double new_quantity, double new_price = 0.0);
    virtual std::optional<Order> get_order(long order_id) const;
    virtual std::vector<Order> get_orders(const std::string& symbol = "") const;
    virtual std::vector<Order> get_open_orders(const std::string& symbol = "") const;

    // Position management
    virtual Position get_position(const std::string& symbol) const;
    virtual std::map<std::string, Position> get_all_positions() const;
    virtual bool close_position(const std::string& symbol, double quantity = 0.0); // 0 means close all
    
    // Account information
    virtual AccountInfo get_account_info() const { return account_info_; }
    virtual double get_cash() const { return account_info_.cash; }
    virtual double get_equity() const { return account_info_.equity; }
    virtual double get_buying_power() const { return account_info_.buying_power; }
    
    // Trade history
    virtual std::vector<Trade> get_trades(const std::string& symbol = "") const;
    virtual std::vector<Trade> get_trades_by_date(uint64_t start_time, uint64_t end_time, const std::string& symbol = "") const;
    
    // Market data and pricing
    virtual void update_market_prices(const std::map<std::string, double>& prices);
    virtual double get_last_price(const std::string& symbol) const;
    
    // Callback registration
    virtual void register_order_status_callback(BrokerOrderStatusCallback callback) { order_status_callback_ = callback; }
    virtual void register_trade_callback(BrokerTradeCallback callback) { trade_callback_ = callback; }
    virtual void register_position_callback(PositionCallback callback) { position_callback_ = callback; }
    virtual void register_account_callback(AccountCallback callback) { account_callback_ = callback; }
    
    // Broker provider integration
    virtual void set_broker_provider(std::shared_ptr<BrokerProvider> provider) { broker_provider_ = provider; }
    virtual std::shared_ptr<BrokerProvider> get_broker_provider() const { return broker_provider_; }
    
    // Utility methods
    virtual long get_next_order_id() { return next_order_id_++; }
    virtual void reset_account(double starting_cash);
    
protected:
    // Internal order processing
    virtual void process_order_fill(long order_id, double fill_quantity, double fill_price, uint64_t timestamp);
    virtual void process_order_cancel(long order_id);
    virtual void process_order_reject(long order_id, const std::string& reason);
    
    // Internal position updates
    virtual void update_position(const std::string& symbol, OrderSide side, double quantity, double price);
    virtual void update_account_info();
    
    // Internal state
    AccountInfo account_info_;
    std::unordered_map<long, Order> orders_;
    std::map<std::string, Position> positions_;
    std::vector<Trade> trades_;
    std::map<std::string, double> last_prices_;
    
    long next_order_id_ = 1;
    
    // Broker provider for actual order execution
    std::shared_ptr<BrokerProvider> broker_provider_;
    
    // Callbacks
    BrokerOrderStatusCallback order_status_callback_;
    BrokerTradeCallback trade_callback_;
    PositionCallback position_callback_;
    AccountCallback account_callback_;
    
    // Logging
    quanttrader::log::LoggerPtr logger_;
    
private:
    // Helper methods
    std::string order_side_to_string(OrderSide side) const;
    std::string order_type_to_string(OrderType type) const;
    std::string order_status_to_string(OrderStatus status) const;
};

} // namespace broker
} // namespace quanttrader
