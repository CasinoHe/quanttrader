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
     * @param config_path Configuration file path
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
     * Processes historical data through the strategies
     * 
     * @return true if backtest completed successfully
     * @return false if backtest failed
     */
    bool run() override {
        logger_->info("Starting backtest");
        stop_flag_.store(false);
        
        if (!prepare()) {
            logger_->error("Failed to prepare cerebro for execution");
            return false;
        }
        
        is_running_ = true;
        
        // Start all strategies
        for (auto& strategy : strategies_) {
            if (!strategy->on_start()) {
                logger_->error("Failed to start strategy: {}", strategy->get_name());
                return false;
            }
        }
        
        // Process data
        int step_count = 0;
        while (!stop_flag_.load()) {
            // Process the next data point
            if (!process_next()) {
                logger_->info("No more data to process, completed {} steps", step_count);
                break;
            }
            
            step_count++;
            
            // Optional small delay to prevent high CPU usage (can be removed for faster backtest)
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        // Stop all strategies
        for (auto& strategy : strategies_) {
            strategy->on_stop();
        }
        
        is_running_ = false;
        logger_->info("Backtest completed with {} steps", step_count);
        return true;
    }
};

} // namespace cerebro
} // namespace quanttrader