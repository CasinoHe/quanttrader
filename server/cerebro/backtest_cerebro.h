#pragma once

#include "cerebro_base.h"
#include <chrono>
#include <atomic>
#include <thread>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Implementation of Cerebro for backtesting
 * 
 * This class handles the execution of strategies in a backtesting environment,
 * coordinating data feeds and broker simulation.
 */
class BacktestCerebro : public Cerebro {
public:
    /**
     * @brief Construct a new Backtest Cerebro object
     * 
     * @param name Name of this cerebro instance
     * @param params Configuration parameters
     */
    BacktestCerebro(const std::string_view name, CerebroParamsType params)
        : Cerebro(name, params), stop_flag_(false) {
        
        logger_->info("Created backtest cerebro: {}", name_);
    }

    ~BacktestCerebro() {
        stop();
        logger_->info("Destroying backtest cerebro: {}", name_);
    }

    /**
     * @brief Initialize the backtest engine
     * 
     * Sets up the data feeds, strategies, and broker for backtesting
     * 
     * @return true if initialization succeeds
     * @return false if initialization fails
     */
    bool initialize() override {
        logger_->info("Initializing backtest cerebro: {}", name_);
        
        if (strategies_.empty()) {
            logger_->error("No strategies added to cerebro");
            return false;
        }
        
        if (data_providers_.empty()) {
            logger_->error("No data providers added to cerebro");
            return false;
        }
        
        if (!broker_) {
            logger_->error("No broker set for cerebro");
            return false;
        }
        
        // Prepare data providers
        for (auto& data_provider : data_providers_) {
            if (!data_provider->prepare_data()) {
                logger_->error("Failed to prepare data provider: {}", data_provider->get_data_prefix());
                return false;
            }
        }
        
        logger_->info("Backtest cerebro initialized successfully");
        return true;
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
        if (!initialize()) {
            logger_->error("Failed to initialize backtest");
            return false;
        }
        
        logger_->info("Starting backtest");
        stop_flag_.store(false);
        
        // Start data feeds
        for (auto& data_provider : data_providers_) {
            data_provider->start_request_data();
        }
        
        // Main backtest loop
        while (!stop_flag_.load()) {
            // Check if all data is ready
            bool all_data_ready = true;
            for (auto& data_provider : data_providers_) {
                if (!data_provider->is_data_ready()) {
                    all_data_ready = false;
                    break;
                }
            }
            
            if (!all_data_ready) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            
            // Get next data points from all providers
            for (auto& data_provider : data_providers_) {
                auto bar = data_provider->next();
                // TODO: Process this bar with all strategies
            }
            
            // Process strategies
            for (auto& strategy : strategies_) {
                strategy->on_bar();
            }
            
            // Check if we've reached the end of data
            bool end_of_data = true;
            for (auto& data_provider : data_providers_) {
                if (!data_provider->is_data_finished()) {
                    end_of_data = false;
                    break;
                }
            }
            
            if (end_of_data) {
                logger_->info("Reached end of backtest data");
                break;
            }
            
            // Add a small delay to prevent high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        logger_->info("Backtest completed");
        return true;
    }

    /**
     * @brief Stop the backtest
     */
    void stop() override {
        stop_flag_.store(true);
        logger_->info("Stopping backtest");
    }

    /**
     * @brief Add a strategy to the backtest
     * 
     * @param strategy The strategy to add
     * @return true if added successfully
     * @return false if failed to add
     */
    bool add_strategy(std::shared_ptr<strategy::Strategy> strategy) override {
        if (!strategy) {
            logger_->error("Cannot add null strategy");
            return false;
        }
        
        strategies_.push_back(strategy);
        logger_->info("Added strategy to backtest");
        return true;
    }

    /**
     * @brief Add a data provider to the backtest
     * 
     * @param data_provider The data provider to add
     * @return true if added successfully
     * @return false if failed to add
     */
    bool add_data(std::shared_ptr<data::DataProvider> data_provider) override {
        if (!data_provider) {
            logger_->error("Cannot add null data provider");
            return false;
        }
        
        data_providers_.push_back(data_provider);
        logger_->info("Added data provider: {}", data_provider->get_data_prefix());
        return true;
    }

    /**
     * @brief Set the broker for the backtest
     * 
     * @param broker The broker to use
     * @return true if set successfully
     * @return false if failed to set
     */
    bool set_broker(std::shared_ptr<broker::BrokerProvider> broker) override {
        if (!broker) {
            logger_->error("Cannot set null broker");
            return false;
        }
        
        broker_ = broker;
        logger_->info("Set broker for backtest");
        return true;
    }

private:
    std::atomic<bool> stop_flag_;
};

} // namespace cerebro
} // namespace quanttrader