# Introduction

## Overall Goals

1. Designed for personal or family use.
2. Efficient and convenient operation, with mobile app support for operations.
3. Capable of providing early warnings, forecasting trading scenarios, and generating pre-trading reports in advance.
4. Real-time trading, with real-time monitoring of trading situations and dynamic adjustment of trading strategies.

## Phase 1 Goal: Develop an efficient stock backtesting system

1. **Core System**: Implemented in C++, with annotated interfaces to facilitate rapid support for scripting language APIs such as Python and Lua.
2. **Technical Analysis**: Utilize the TA-Lib library to support common technical indicators, ensuring the accuracy of indicator calculations.
3. **Data Support**: Support real-time data acquisition from the IB API and OHLCV CSV data.
4. **AI-Driven Development**: Incorporate AI in code writing, from code annotations to code generation, to minimize manual coding efforts.
5. **Real-Time Trading**: Implement real-time trading using the IB API.

---

## Short-Term Plan

1. Logging support module: Use **spdlog** to enable multi-threaded logging output.
2. Data reading module: Support real-time data acquisition from the IB API.
3. Simple strategy backtesting module: Perform rapid backtesting with data retrieved from the IB API.

## Strategy Plugins

Strategies are compiled independently of the trading system. Each strategy
produces a shared library that can be placed in the directory configured by
`strategy_lib_path`. The sample project under `server/strategy/slope_strategy`
can be built separately using CMake and the resulting library copied to the
server before running.
