#pragma once

#include "cerebro/cerebro_base.h"
#include <chrono>
#include <atomic>
#include <thread>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Implementation of CerebroBase for backtesting
 * 
 * This class handles the execution of strategies in a backtesting environment,
 * coordinating data feeds and broker simulation.
 */
class BacktestCerebro : public CerebroBase {
public:
    /**
     * @brief Construct a new Backtest CerebroBase object
     * 
     * @param name Name of this cerebro instance
     */
    BacktestCerebro(const std::string_view name)
        : CerebroBase(name) {
        
        logger_->info("Created backtest cerebro: {}", name_);
    }

    ~BacktestCerebro() {
        logger_->info("Destroying backtest cerebro: {}", name_);
    }

    /**
     * @brief Run the backtest
     * 
     * Uses the enhanced CerebroBase run method
     * 
     * @return true if backtest completed successfully
     * @return false if backtest failed
     */
    bool run() override {
        // Use the enhanced run method from CerebroBase
        return CerebroBase::run();
    }
};

} // namespace cerebro
} // namespace quanttrader