#pragma once

#include "cerebro_consts.h"
#include "data/data_provider.h"
#include "data/replay/data_replay_controller.h"
#include "strategy/strategy_base.h"
#include "logger/quantlogger.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace quanttrader {
namespace broker {
class BrokerProvider;
}
}

namespace cerebro {

/**
 * @brief Base class for all cerebro types
 * 
 * This class provides the common functionality for all cerebro implementations,
 * which are responsible for coordinating data feeds, strategies, and brokers.
 */
class CerebroBase {
public:
    /**
     * @brief Construct a new Cerebro object
     * 
     * @param name Name of this Cerebro instance
     * @param configPath Path to the configuration file for this Cerebro
     */
    CerebroBase(const std::string_view name, const std::string& configPath)
        : name_(name), config_path_(configPath), stop_flag_(false) {
        logger_ = quanttrader::log::get_common_rotation_logger(name_, "cerebro");
        logger_->info("Created cerebro: {} with config: {}", name_, config_path_);
    }

    virtual ~CerebroBase();

    /**
     * @brief Add a data provider to cerebro
     * 
     * @param name A unique identifier for this data feed
     * @param provider The data provider to add
     * @return true if successful, false otherwise
     */
    bool add_data(const std::string& name, std::shared_ptr<data::provider::DataProvider> provider);

    /**
     * @brief Add a strategy to cerebro
     * 
     * @param strategy The strategy to add
     * @return true if successful, false otherwise
     */
    bool add_strategy(std::shared_ptr<strategy::StrategyBase> strategy);

    /**
     * @brief Set the replay mode for all data providers
     * 
     * @param mode The replay mode to set
     */
    void set_replay_mode(data::provider::DataProvider::ReplayMode mode);

    /**
     * @brief For STEPPED replay mode, advance to the next data point
     */
    void next_step();

    /**
     * @brief Set the resample parameters for a data feed
     * 
     * @param name The name of the data feed to resample
     * @param target_type The target bar type for resampling
     * @param target_size The target bar size for resampling
     * @return true if successful, false otherwise
     */
    bool resample_data(const std::string& name, data::BarType target_type, unsigned int target_size);

    /**
     * @brief Prepare cerebro for execution
     * 
     * This method initializes all components and prepares them for the run.
     * 
     * @return true if preparation is successful, false otherwise
     */
    virtual bool prepare();

    /**
     * @brief Run the cerebro engine
     * 
     * This method executes the entire backtest or trading process.
     * 
     * @return true if the run is successful, false otherwise
     */
    virtual bool run() = 0;

    /**
     * @brief Stop cerebro execution
     * 
     * This method terminates any ongoing execution.
     * 
     * @return true if the stop is successful, false otherwise
     */
    virtual bool stop();

protected:
    /**
     * @brief Process the next set of data
     * 
     * This method fetches the next data point and passes it to strategies.
     * 
     * @return true if there is more data to process, false if done
     */
    virtual bool process_next();

    // Data management
    std::shared_ptr<data::replay::DataReplayController> replay_controller_;
    
    // Strategy management
    std::vector<std::shared_ptr<strategy::StrategyBase>> strategies_;
    
    // Execution state
    bool is_running_ = false;
    bool is_prepared_ = false;
    
    // Logging
    quanttrader::log::LoggerPtr logger_;
    std::string name_;
    std::string config_path_;
    std::shared_ptr<broker::BrokerProvider> broker_;
    std::atomic<bool> stop_flag_;
};

} // namespace cerebro
} // namespace quanttrader