# Enhanced Trading System Architecture

## Overview

This document describes the enhanced trading system architecture that provides a hierarchical structure with proper separation of concerns between data management, trading logic, and broker operations.

## Architecture Components

### 1. AbstractBroker Class (`broker/abstract_broker.h`)

The `AbstractBroker` class serves as the central hub for all trading operations and maintains the complete state of:

- **Account Information**: Cash, equity, buying power, margins
- **Orders**: All order states (pending, filled, cancelled, rejected)
- **Positions**: Current holdings with average prices and P&L
- **Trades**: Complete trade history with execution details

#### Key Features:
- **Order Management**: Place, cancel, modify orders with proper validation
- **Position Tracking**: Automatic position updates on trade execution
- **Risk Management**: Account monitoring and margin requirements
- **Event Callbacks**: Real-time notifications for order status, trades, positions
- **Market Data Integration**: Price updates for unrealized P&L calculations

### 2. BacktestBroker Class (`broker/backtest_broker.h`)

A concrete implementation of `AbstractBroker` designed for backtesting:

- **Realistic Order Simulation**: Market, limit, stop, and stop-limit orders
- **Commission and Slippage**: Configurable trading costs
- **Margin Requirements**: Initial and maintenance margin calculations
- **Market Data Processing**: Automatic order execution based on price movements

### 3. Enhanced Strategy Base (`strategy/strategy_base.h`)

The strategy base class has been enhanced with broker integration:

#### New Trading Methods:
```cpp
// Enhanced order placement with proper typing
long buy(const std::string& symbol, double quantity, double price = 0.0, 
         broker::OrderType type = broker::OrderType::MARKET);
long sell(const std::string& symbol, double quantity, double price = 0.0, 
          broker::OrderType type = broker::OrderType::MARKET);

// Specialized order types
long buy_limit(const std::string& symbol, double quantity, double price);
long sell_limit(const std::string& symbol, double quantity, double price);
long buy_stop(const std::string& symbol, double quantity, double stop_price);
long sell_stop(const std::string& symbol, double quantity, double stop_price);
```

#### Position Management:
```cpp
broker::Position get_position(const std::string& symbol) const;
std::map<std::string, broker::Position> get_all_positions() const;
bool close_position(const std::string& symbol, double quantity = 0.0);
```

#### Account Access:
```cpp
double get_cash() const;
double get_equity() const;
double get_buying_power() const;
```

#### Event Handlers:
```cpp
virtual void on_order_status(const broker::Order& order);
virtual void on_trade_execution(const broker::Trade& trade);
virtual void on_position_update(const broker::Position& position);
virtual void on_account_update(const broker::AccountInfo& account);
```

### 4. Enhanced Cerebro (`cerebro/cerebro_base.h`)

The cerebro class now provides complete broker integration:

- **Broker Configuration**: Easy setup of different broker types with parameters
- **Strategy Integration**: Automatic broker assignment to strategies
- **Performance Monitoring**: Enhanced reporting with broker data
- **Data Synchronization**: Market data updates to broker and strategies
- **Unified Interface**: Single class handles both simple and complex trading scenarios

### 5. Performance Observer Updates (`observer/performance_observer.h`)

The performance observer can now work with broker data:

- **Broker Integration**: Direct access to broker account information
- **Real-time Updates**: Automatic updates from broker state
- **Enhanced Reporting**: Detailed performance metrics with position data

## Usage Example

```cpp
// Create enhanced cerebro
auto cerebro = std::make_shared<cerebro::CerebroBase>("MyBacktest", 100000.0);

// Configure backtest broker
cerebro->set_broker_type("backtest", 100000.0);
cerebro->configure_backtest_broker(
    1.0,    // $1 commission per trade
    0.01,   // 0.01% slippage
    0.25,   // 25% initial margin
    0.15    // 15% maintenance margin
);

// Create strategy
strategy::StrategyCreateFuncParemType params;
params["symbol"] = std::string("AAPL");
params["position_size"] = 100.0;
params["allow_short"] = true;

auto strategy = std::make_shared<strategy::BrokerAwareStrategy>(params);
cerebro->add_strategy(strategy);

// Add data and run
cerebro->add_data("AAPL", data_provider);
cerebro->run();
```

## Long and Short Position Support

The system now properly supports both long and short positions:

### Long Positions:
```cpp
// Buy shares to establish long position
long order_id = buy("AAPL", 100, 150.0);  // Buy 100 shares at $150

// Position tracking
auto position = get_position("AAPL");
if (position.is_long()) {
    // Handle long position logic
}
```

### Short Positions:
```cpp
// Sell shares to establish short position
long order_id = sell("AAPL", 100, 150.0);  // Sell 100 shares at $150

// Position tracking
auto position = get_position("AAPL");
if (position.is_short()) {
    // Handle short position logic
}
```

### Position Management:
```cpp
// Close entire position
close_position("AAPL");

// Close partial position
close_position("AAPL", 50);  // Close 50 shares

// Check position status
auto position = get_position("AAPL");
if (position.is_flat()) {
    // No position
} else if (position.is_long()) {
    // Long position
} else if (position.is_short()) {
    // Short position
}
```

## Order Types and States

### Order Types:
- **Market Orders**: Execute immediately at market price
- **Limit Orders**: Execute only at specified price or better
- **Stop Orders**: Trigger market order when stop price is reached
- **Stop Limit Orders**: Trigger limit order when stop price is reached

### Order States:
- **Pending**: Order placed but not yet filled
- **Filled**: Order completely executed
- **Partially Filled**: Order partially executed
- **Cancelled**: Order cancelled before execution
- **Rejected**: Order rejected due to insufficient funds or other reasons

## Benefits of the New Architecture

1. **Separation of Concerns**: Clear separation between data, strategy, and broker logic
2. **Realistic Trading**: Proper order management with realistic execution
3. **Risk Management**: Built-in account monitoring and margin requirements
4. **Flexibility**: Easy to switch between backtesting and live trading
5. **Extensibility**: Easy to add new broker types and order types
6. **Performance Monitoring**: Comprehensive performance tracking
7. **Event-Driven**: Real-time notifications for all trading events

## Migration from Legacy System

The new system maintains backward compatibility with legacy strategy methods:

```cpp
// Legacy methods still work
void buy_legacy(const std::string& symbol, int quantity, double price = 0.0);
void sell_legacy(const std::string& symbol, int quantity, double price = 0.0);
```

However, new strategies should use the enhanced broker-integrated methods for full functionality.

## Future Enhancements

- **Live Trading**: Integration with real broker APIs
- **Advanced Order Types**: OCO, trailing stops, etc.
- **Portfolio Management**: Multi-strategy portfolio optimization
- **Risk Analytics**: Advanced risk metrics and reporting
- **Machine Learning**: AI-powered trade execution optimization
