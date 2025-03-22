#pragma once

#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <vector>

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
        : name_(name), config_path_(configPath) {
        logger_ = quanttrader::log::get_common_rotation_logger(name_, "cerebro");
        logger_->info("Created cerebro: {} with config: {}", name_, config_path_);
    }

    virtual ~Cerebro() {
        logger_->info("Destroying cerebro: {}", name_);
    }

    /**
     * @brief Initialize the Cerebro engine with the provided configuration
     * 
     * @return true if initialization succeeded
     * @return false if initialization failed
     */
    virtual bool initialize() = 0;

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
    virtual void stop() = 0;

    /**
     * @brief Add a strategy to the Cerebro engine
     * 
     * @param strategy Strategy to add
     * @return true if the strategy was added successfully
     * @return false if the strategy couldn't be added
     */
    virtual bool add_strategy(std::shared_ptr<strategy::Strategy> strategy) = 0;

    /**
     * @brief Add a data provider to the Cerebro engine
     * 
     * @param data_provider Data provider to add
     * @return true if the data provider was added successfully
     * @return false if the data provider couldn't be added
     */
    virtual bool add_data(std::shared_ptr<data::DataProvider> data_provider) = 0;

    /**
     * @brief Set the broker provider to use for order execution
     * 
     * @param broker Broker provider to use
     * @return true if the broker was set successfully
     * @return false if the broker couldn't be set
     */
    virtual bool set_broker(std::shared_ptr<broker::BrokerProvider> broker) = 0;

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
    std::string name_;
    std::string config_path_;
    quanttrader::log::LoggerPtr logger_;
    std::vector<std::shared_ptr<strategy::Strategy>> strategies_;
    std::vector<std::shared_ptr<data::DataProvider>> data_providers_;
    std::shared_ptr<broker::BrokerProvider> broker_;
};

} // namespace cerebro
} // namespace quanttrader