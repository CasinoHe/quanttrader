#pragma once

#include "cerebro_base.h"
#include <chrono>
#include <atomic>
#include <thread>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Implementation of Cerebro for live trading
 * 
 * This class handles the execution of strategies in a live trading environment,
 * coordinating real-time data feeds and broker interactions.
 */
class LiveCerebro : public Cerebro {
public:
    /**
     * @brief Construct a new Live Cerebro object
     * 
     * @param name Name of this cerebro instance
     * @param params Configuration parameters
     */
    LiveCerebro(const std::string_view name, const std::string& config_path)
        : Cerebro(name, config_path), stop_flag_(false), trading_thread_(nullptr) {
        
        logger_->info("Created live trading cerebro: {}", name_);
    }

    ~LiveCerebro() {
        stop();
        logger_->info("Destroying live trading cerebro: {}", name_);
    }

    /**
     * @brief Initialize the live trading engine
     * 
     * Sets up the data feeds, strategies, and broker connections
     * 
     * @return true if initialization succeeds
     * @return false if initialization fails
     */
    bool initialize() override {
        logger_->info("Initializing live trading cerebro: {}", name_);
        
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
        
        // Connect to broker
        if (!broker_->connect()) {
            logger_->error("Failed to connect to broker");
            return false;
        }
        
        // Prepare data providers for real-time data
        for (auto& data_provider : data_providers_) {
            if (!data_provider->prepare_data()) {
                logger_->error("Failed to prepare data provider: {}", data_provider->get_data_prefix());
                return false;
            }
        }
        
        logger_->info("Live trading cerebro initialized successfully");
        return true;
    }

    /**
     * @brief Run the live trading session
     * 
     * Processes real-time data through the strategies
     * 
     * @return true if trading session started successfully
     * @return false if trading session failed to start
     */
    bool run() override {
        if (!initialize()) {
            logger_->error("Failed to initialize live trading");
            return false;
        }
        
        logger_->info("Starting live trading");
        stop_flag_.store(false);
        
        // Start trading in a separate thread
        trading_thread_ = std::make_shared<std::thread>([this]() {
            this->trading_loop();
        });
        
        return true;
    }

    /**
     * @brief Stop the live trading session
     */
    void stop() override {
        if (!trading_thread_) {
            return;
        }
        
        stop_flag_.store(true);
        logger_->info("Stopping live trading");
        
        if (trading_thread_->joinable()) {
            trading_thread_->join();
        }
        
        trading_thread_.reset();
        
        // Disconnect from broker
        if (broker_) {
            broker_->disconnect();
        }
        
        logger_->info("Live trading stopped");
    }

    /**
     * @brief Add a strategy to the live trading engine
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
        logger_->info("Added strategy to live trading");
        return true;
    }

    /**
     * @brief Add a data provider to the live trading engine
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
     * @brief Set the broker for the live trading engine
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
        logger_->info("Set broker for live trading");
        return true;
    }

private:
    /**
     * @brief Main trading loop for live execution
     */
    void trading_loop() {
        // Start real-time data feeds
        for (auto& data_provider : data_providers_) {
            data_provider->start_request_data();
        }
        
        // Register callbacks for real-time data and trading events
        // TODO: Implement proper callback registration
        
        // Main trading loop
        while (!stop_flag_.load()) {
            // Process any new data or trading events
            for (auto& strategy : strategies_) {
                strategy->on_tick();
                strategy->on_bar();
                strategy->on_trade();
            }
            
            // Sleep to prevent high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::atomic<bool> stop_flag_;
    std::shared_ptr<std::thread> trading_thread_;
};

} // namespace cerebro
} // namespace quanttrader