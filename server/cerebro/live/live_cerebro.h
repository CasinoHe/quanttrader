#pragma once

#include "cerebro_base.h"
#include <chrono>
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
     * @param config_path Configuration file path
     */
    LiveCerebro(const std::string_view name, const std::string& config_path)
        : Cerebro(name, config_path), trading_thread_(nullptr) {
        
        logger_->info("Created live trading cerebro: {}", name_);
    }

    ~LiveCerebro() {
        stop();
        logger_->info("Destroying live trading cerebro: {}", name_);
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
        Cerebro::stop(); // Call base class stop first
        
        if (!trading_thread_) {
            return;
        }
        
        if (trading_thread_->joinable()) {
            trading_thread_->join();
        }
        
        trading_thread_.reset();
        logger_->info("Live trading stopped");
    }

protected:
    /**
     * @brief Initialize broker for live trading
     * 
     * @return true if successful
     * @return false if failed
     */
    bool initialize_broker() override {
        // Connect to broker for live trading
        if (!broker_->connect()) {
            logger_->error("Failed to connect to broker");
            return false;
        }
        
        return true;
    }
    
    /**
     * @brief Prepare data providers for live trading
     * 
     * @return true if successful
     * @return false if failed
     */
    bool prepare_data_providers() override {
        // Prepare data providers for real-time data
        for (auto& data_provider : data_providers_) {
            if (!data_provider->prepare_data()) {
                logger_->error("Failed to prepare data provider: {}", data_provider->get_data_prefix());
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief Clean up broker resources
     */
    void cleanup_broker() override {
        // Disconnect from broker
        if (broker_) {
            broker_->disconnect();
        }
    }

private:
    /**
     * @brief Main trading loop for live execution
     */
    void trading_loop() {
        // Start real-time data feeds
        start_data_providers();
        
        // Register callbacks for real-time data and trading events
        // TODO: Implement proper callback registration
        
        // Main trading loop
        while (!stop_flag_.load()) {
            // Process any new data or trading events
            process_strategies_on_tick();
            process_strategies_on_bar();
            
            // Process any trade signals
            for (auto& strategy : strategies_) {
                strategy->on_trade();
            }
            
            // Sleep to prevent high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::shared_ptr<std::thread> trading_thread_;
};

} // namespace cerebro
} // namespace quanttrader