#include "cerebro.h"
#include "data/data_provider.h"
#include "broker/broker_provider_factory.h"
#include "strategy/strategy_factory.h"
#include <iostream>

/**
 * @file cerebro_example.cpp
 * @brief Example implementation showing how to use the Cerebro system
 * 
 * This file demonstrates typical usage patterns for the Cerebro system,
 * including both backtesting and live trading setups.
 */

namespace quanttrader {
namespace examples {

/**
 * @brief Example of setting up and running a backtest using Cerebro
 * 
 * @param strategyName Name of the strategy to use
 * @param dataPrefix Data prefix for the data provider
 * @param initialCash Starting cash amount for the backtest
 * @return true if the backtest was successful
 * @return false if the backtest failed
 */
bool run_backtest_example(
    const std::string& strategyName,
    const std::string& dataPrefix,
    double initialCash = 100000.0) {
    
    using namespace cerebro;
    
    // Create parameters for the backtest
    auto params = CerebroUtil::create_default_params(BACKTEST_CEREBRO_TYPE);
    (*params)[INITIAL_CASH_KEY] = initialCash;
    (*params)[CEREBRO_NAME_KEY] = "example_backtest";
    
    // Add strategy-specific parameters if needed
    (*params)["strategy_name"] = strategyName;
    (*params)["data_prefix"] = dataPrefix;
    
    // Create a backtest cerebro instance
    auto cerebro = CerebroUtil::create_backtest("example_backtest", params);
    if (!cerebro) {
        std::cerr << "Failed to create backtest cerebro" << std::endl;
        return false;
    }
    
    // Create and add strategy
    auto strategy = strategy::StrategyFactory::create_strategy(strategyName, *params);
    if (!strategy) {
        std::cerr << "Failed to create strategy: " << strategyName << std::endl;
        return false;
    }
    cerebro->add_strategy(strategy);
    
    // Create and add data provider
    auto dataProvider = std::make_shared<data::DataProvider>(dataPrefix, params);
    if (!dataProvider) {
        std::cerr << "Failed to create data provider for: " << dataPrefix << std::endl;
        return false;
    }
    cerebro->add_data(dataProvider);
    
    // Create and set broker (using a simulated broker for backtesting)
    auto broker = broker::BrokerProviderFactory::instance().createProvider("simulated", "");
    if (!broker) {
        std::cerr << "Failed to create simulated broker" << std::endl;
        return false;
    }
    cerebro->set_broker(broker);
    
    // Run the backtest
    std::cout << "Starting backtest..." << std::endl;
    bool result = cerebro->run();
    
    // Cleanup - the CerebroManager will handle this automatically, but we can do it explicitly
    CerebroUtil::destroy("example_backtest");
    
    return result;
}

/**
 * @brief Example of setting up and running live trading using Cerebro
 * 
 * @param strategyName Name of the strategy to use
 * @param dataPrefix Data prefix for the data provider
 * @param brokerConfigPath Path to the broker configuration file
 * @param initialCash Starting cash amount
 * @return true if live trading started successfully
 * @return false if live trading failed to start
 */
bool run_live_trading_example(
    const std::string& strategyName,
    const std::string& dataPrefix,
    const std::string& brokerConfigPath,
    double initialCash = 100000.0) {
    
    using namespace cerebro;
    
    // Create parameters for live trading
    auto params = CerebroUtil::create_default_params(LIVE_CEREBRO_TYPE);
    (*params)[INITIAL_CASH_KEY] = initialCash;
    (*params)[CEREBRO_NAME_KEY] = "example_live";
    
    // Add strategy-specific parameters if needed
    (*params)["strategy_name"] = strategyName;
    (*params)["data_prefix"] = dataPrefix;
    
    // Create a live trading cerebro instance
    auto cerebro = CerebroUtil::create_live("example_live", params);
    if (!cerebro) {
        std::cerr << "Failed to create live trading cerebro" << std::endl;
        return false;
    }
    
    // Create and add strategy
    auto strategy = strategy::StrategyFactory::create_strategy(strategyName, *params);
    if (!strategy) {
        std::cerr << "Failed to create strategy: " << strategyName << std::endl;
        return false;
    }
    cerebro->add_strategy(strategy);
    
    // Create and add data provider
    auto dataProvider = std::make_shared<data::DataProvider>(dataPrefix, params);
    if (!dataProvider) {
        std::cerr << "Failed to create data provider for: " << dataPrefix << std::endl;
        return false;
    }
    cerebro->add_data(dataProvider);
    
    // Create and set broker (using a real broker for live trading)
    auto broker = broker::BrokerProviderFactory::instance().createProvider("tws", brokerConfigPath);
    if (!broker) {
        std::cerr << "Failed to create TWS broker" << std::endl;
        return false;
    }
    cerebro->set_broker(broker);
    
    // Run the live trading (this will start in a separate thread)
    std::cout << "Starting live trading..." << std::endl;
    bool result = cerebro->run();
    
    if (result) {
        std::cout << "Live trading started successfully. Press Enter to stop." << std::endl;
        std::cin.get();
        
        // Stop the live trading
        cerebro->stop();
        
        // Cleanup
        CerebroUtil::destroy("example_live");
    }
    
    return result;
}

} // namespace examples
} // namespace quanttrader