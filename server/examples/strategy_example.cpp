#include "cerebro/cerebro_util.h"
#include "strategy/strategy_util.h"
#include "strategy/strategy_prepare.h"
#include "data/csv/csv_data_provider.h"
#include <iostream>
#include <memory>
#include <string>

using namespace quanttrader;

/**
 * @brief Example showing how to use the refactored strategy and cerebro system
 * 
 * This example demonstrates:
 * 1. Creating a backtest cerebro instance
 * 2. Adding a CSV data provider for historical data
 * 3. Creating and configuring a simple moving average crossover strategy
 * 4. Running the backtest
 */
int main() {
    // Initialize the strategy system
    if (!strategy::prepare_stategies()) {
        std::cerr << "Failed to initialize strategy system" << std::endl;
        return 1;
    }
    
    // Create a backtest cerebro
    auto cerebro = cerebro::CerebroUtil::create_backtest("MA_Crossover_Test");
    if (!cerebro) {
        std::cerr << "Failed to create cerebro instance" << std::endl;
        return 1;
    }
    
    // Create a CSV data provider for SPY data
    auto spy_data = std::make_shared<data::provider::CSVDataProvider>(
        "SPY",
        "data/spy_daily.csv",
        data::BarType::TIME_BAR,
        data::TimeFrame::DAY,
        1
    );
    
    // Add the data provider to cerebro
    if (!cerebro->add_data("SPY", spy_data)) {
        std::cerr << "Failed to add data provider" << std::endl;
        return 1;
    }
    
    // Create and add a simple moving average crossover strategy
    if (!cerebro::CerebroUtil::add_ma_crossover_strategy(
        cerebro, "MA_Crossover", "SPY", 20, 50)) {
        std::cerr << "Failed to add strategy" << std::endl;
        return 1;
    }
    
    // Set the replay mode to continuous (runs through all data automatically)
    cerebro->set_replay_mode(data::provider::DataProvider::ReplayMode::CONTINUOUS);
    
    // Run the backtest
    if (!cerebro->run()) {
        std::cerr << "Failed to run backtest" << std::endl;
        return 1;
    }
    
    std::cout << "Backtest completed successfully" << std::endl;
    return 0;
}