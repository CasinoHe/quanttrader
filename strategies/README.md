# QuantTrader Strategy Development Framework

This directory contains the strategy development framework for QuantTrader, designed to be used as a standalone submodule for strategy development.

## Overview

This framework allows developers to create trading strategies as dynamic libraries (.so files) that can be loaded at runtime by the QuantTrader trading system.

### Prerequisites
- CMake 3.22+
- C++23 compatible compiler
- QuantTrader server dependencies (fmt, spdlog, TA-Lib, etc.)

### Building Strategies

```bash
mkdir build
cd build
cmake ..
make
```

This will build all strategies and place the resulting `.so` files in the `build/plugins/` directory.

### Building Individual Strategies

```bash
# Build only slope strategy
mkdir build && cd build
cmake .. -DBUILD_SLOPE_STRATEGY_ONLY=ON
make
```

## Creating a New Strategy

1. Create a new directory under `strategies/`
2. Implement your strategy class inheriting from `StrategyBase` or `BrokerAwareStrategy`
3. Create a registration function that adds your strategy to the factory
4. Add a CMakeLists.txt file
5. Add your strategy to the main CMakeLists.txt

### Example Strategy Structure

```cpp
// my_strategy.h
#pragma once
#include "strategy/broker_aware_strategy.h"

namespace quanttrader {
namespace strategy {

class MyStrategy : public BrokerAwareStrategy {
public:
    MyStrategy(const std::string& name, 
               std::shared_ptr<std::unordered_map<std::string, std::any>> params);
    
    void on_bar(const std::shared_ptr<data::BasicBarData>& bar) override;
    void on_tick(const std::shared_ptr<data::BasicTickData>& tick) override;
};

}}

// my_strategy_register.cpp
#include "strategy/strategy_factory.h"
#include "my_strategy.h"

extern "C" {
    void register_strategy() {
        quanttrader::strategy::StrategyFactory::register_strategy(
            "MyStrategy", 
            [](const std::string& name, auto params) {
                return std::make_shared<quanttrader::strategy::MyStrategy>(name, params);
            }
        );
    }
}
```

## Strategy Interface

All strategies must inherit from either:
- `StrategyBase`: Basic strategy interface
- `BrokerAwareStrategy`: Strategy with broker integration

### Key Methods to Override

- `on_bar()`: Called when new bar data arrives
- `on_tick()`: Called when new tick data arrives  
- `on_trade()`: Called when trade is executed
- `on_order()`: Called when order status changes

## Configuration

Strategies are configured through Lua configuration files. The strategy name and parameters are passed to the constructor.

Example configuration:
```lua
strategy_names = "MyStrategy"

MyStrategy = {
    symbol = "AAPL",
    timeframe = "1min",
    custom_param = 42
}
```

## Testing

Build with testing enabled:
```bash
cmake .. -DBUILD_TESTING=ON
make test
```

## Deployment

1. Build your strategies
2. Copy the resulting `.so` files to the QuantTrader `strategies/` directory
3. Configure the strategy in your trading configuration
4. Start the QuantTrader system
