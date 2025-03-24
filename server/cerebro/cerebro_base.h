#pragma once

#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <vector>
#include <atomic>

namespace quanttrader {
namespace broker {
class BrokerProvider;
}

namespace data {
class DataProvider;
}

namespace strategy {
class Strategy;
}

namespace cerebro {

/**
 * @brief Base class for Cerebro engine implementations
 * 
 * The Cerebro is the main backtesting/live trading engine that coordinates
 * the interaction between strategies, data providers, and brokers.
 */
class Cerebro {
public:
    /**
     * @brief Construct a new Cerebro object
     * 
     * @param name Name of this Cerebro instance
     * @param configPath Path to the configuration file for this Cerebro
     */
    Cerebro(const std::string_view name, const std::string& configPath)
        : name_(name), config_path_(configPath), stop_flag_(false) {
        logger_ = quanttrader::log::get_common_rotation_logger(name_, "cerebro");
        logger_->info("Created cerebro: {} with config: {}", name_, config_path_);
    }

    virtual ~Cerebro() {
        stop();
        logger_->info("Destroying cerebro: {}", name_);
    }

    /**
     * @brief Initialize the Cerebro engine with the provided configuration
     * 
     * @return true if initialization succeeded
     * @return false if initialization failed
     */
    bool initialize() {
        logger_->info("Initializing cerebro: {}", name_);
        
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
        
        // Hook for broker-specific initialization
        if (!initialize_broker()) {
            return false;
        }
        
        // Prepare data providers for specific mode (backtest or live)
        if (!prepare_data_providers()) {
            return false;
        }
        
        // Hook for additional initialization in derived classes
        if (!initialize_derived()) {
            return false;
        }
        
        logger_->info("Cerebro initialized successfully");
        return true;
    }

    /**
     * @brief Run the cerebro engine (backtest or live trading)
     * 
     * @return true if execution completed successfully
     * @return false if execution failed
     */
    virtual bool run() = 0;

    /**
     * @brief Stop the cerebro engine
     */
    virtual void stop() {
        stop_flag_.store(true);
        logger_->info("Stopping cerebro: {}", name_);
        
        // Cleanup broker connection if needed
        cleanup_broker();
    }

    /**
     * @brief Add a strategy to the Cerebro engine
     * 
     * @param strategy Strategy to add
     * @return true if the strategy was added successfully
     * @return false if the strategy couldn't be added
     */
    bool add_strategy(std::shared_ptr<strategy::Strategy> strategy) {
        if (!strategy) {
            logger_->error("Cannot add null strategy");
            return false;
        }
        
        strategies_.push_back(strategy);
        logger_->info("Added strategy to cerebro");
        return true;
    }

    /**
     * @brief Add a data provider to the Cerebro engine
     * 
     * @param data_provider Data provider to add
     * @return true if the data provider was added successfully
     * @return false if the data provider couldn't be added
     */
    bool add_data(std::shared_ptr<data::DataProvider> data_provider) {
        if (!data_provider) {
            logger_->error("Cannot add null data provider");
            return false;
        }
        
        data_providers_.push_back(data_provider);
        logger_->info("Added data provider: {}", data_provider->get_data_prefix());
        return true;
    }

    /**
     * @brief Set the broker provider to use for order execution
     * 
     * @param broker Broker provider to use
     * @return true if the broker was set successfully
     * @return false if the broker couldn't be set
     */
    bool set_broker(std::shared_ptr<broker::BrokerProvider> broker) {
        if (!broker) {
            logger_->error("Cannot set null broker");
            return false;
        }
        
        broker_ = broker;
        logger_->info("Set broker for cerebro");
        return true;
    }

    /**
     * @brief Get the name of this Cerebro instance
     * 
     * @return std::string The name
     */
    std::string get_name() const noexcept { return name_; }

    /**
     * @brief Get the config path of this Cerebro instance
     * 
     * @return std::string The config path
     */
    std::string get_config_path() const noexcept { return config_path_; }

protected:
    /**
     * @brief Hook for broker-specific initialization in derived classes
     * 
     * @return true if broker initialization succeeded
     * @return false if broker initialization failed
     */
    virtual bool initialize_broker() = 0;
    
    /**
     * @brief Hook for data provider preparation in derived classes
     * 
     * @return true if data provider preparation succeeded
     * @return false if data provider preparation failed
     */
    virtual bool prepare_data_providers() = 0;
    
    /**
     * @brief Hook for additional initialization in derived classes
     * 
     * @return true if derived initialization succeeded
     * @return false if derived initialization failed
     */
    virtual bool initialize_derived() {
        return true; // Default implementation does nothing
    }
    
    /**
     * @brief Cleanup broker resources
     */
    virtual void cleanup_broker() {
        // Default implementation does nothing
    }
    
    /**
     * @brief Common method to trigger on_bar for all strategies
     */
    void process_strategies_on_bar() {
        for (auto& strategy : strategies_) {
            strategy->on_bar();
        }
    }
    
    /**
     * @brief Common method to trigger on_tick for all strategies
     */
    void process_strategies_on_tick() {
        for (auto& strategy : strategies_) {
            strategy->on_tick();
        }
    }
    
    /**
     * @brief Common method to start data requests from all providers
     */
    void start_data_providers() {
        for (auto& data_provider : data_providers_) {
            data_provider->start_request_data();
        }
    }

    std::string name_;
    std::string config_path_;
    quanttrader::log::LoggerPtr logger_;
    std::vector<std::shared_ptr<strategy::Strategy>> strategies_;
    std::vector<std::shared_ptr<data::DataProvider>> data_providers_;
    std::shared_ptr<broker::BrokerProvider> broker_;
    std::atomic<bool> stop_flag_;
};

} // namespace cerebro
} // namespace quanttrader