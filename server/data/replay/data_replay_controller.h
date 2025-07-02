#pragma once

#include "data/common/data_provider.h"
#include "logger/quantlogger.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

namespace quanttrader {
namespace data {
namespace replay {

/**
 * @brief Structure to hold synchronized data and time switch information
 */
struct SynchronizedDataResult {
    std::map<std::string, std::optional<BarStruct>> data;
    bool day_changed = false;
    bool hour_changed = false;
    bool minute_changed = false;
    uint64_t current_time = 0;  // Current timestamp in nanoseconds
    bool has_result = false;
};

/**
 * @brief Data replay controller for cerebro
 * 
 * This class manages multiple data providers and synchronizes their data feeds
 * to provide a unified timeline for backtesting and live trading.
 */
class DataReplayController {
public:
    /**
     * @brief Construct a new Data Replay Controller
     */
    DataReplayController();
    
    /**
     * @brief Destroy the Data Replay Controller
     */
    ~DataReplayController() = default;
    
    /**
     * @brief Add a data provider to the controller
     * 
     * @param name A unique name for this data feed
     * @param provider The data provider to add
     * @return true if added successfully, false if name already exists
     */
    bool add_data_provider(const std::string& name, std::shared_ptr<provider::DataProvider> provider);
    
    /**
     * @brief Remove a data provider from the controller
     * 
     * @param name The name of the data provider to remove
     * @return true if removed successfully, false if not found
     */
    bool remove_data_provider(const std::string& name);
    
    /**
     * @brief Get a data provider by name
     * 
     * @param name The name of the data provider to get
     * @return The data provider or nullptr if not found
     */
    std::shared_ptr<provider::DataProvider> get_data_provider(const std::string& name) const;
    
    /**
     * @brief Start all data providers
     * 
     * @return true if all providers started successfully
     */
    bool start();
    
    /**
     * @brief Stop all data providers
     */
    void stop();
    
    /**
     * @brief Set the replay mode for all data providers
     * 
     * @param mode The replay mode to set
     */
    void set_replay_mode(provider::DataProvider::ReplayMode mode);
    
    /**
     * @brief Get the next synchronized data point from all providers
     * 
     * @return SynchronizedDataResult containing data and time switch information
     */
    SynchronizedDataResult next_synchronized();
    
    /**
     * @brief Signal to provide the next step in STEPPED replay mode for all providers
     */
    void next_step();
    
    /**
     * @brief Rewind all data providers to the beginning
     * 
     * @return true if all providers were rewound successfully
     */
    bool rewind();
    
    /**
     * @brief Check if all data providers are ready
     * 
     * @return true if all providers are ready
     */
    bool all_data_ready() const;
    
    /**
     * @brief Check if any data provider still has data
     * 
     * @return true if at least one provider has more data
     */
    bool has_more_data() const;

public:
    // Expose providers for timezone check in CerebroBase
    const std::unordered_map<std::string, std::shared_ptr<provider::DataProvider>>& get_providers() const { return providers_; }
private:
    std::unordered_map<std::string, std::shared_ptr<provider::DataProvider>> providers_;
    provider::DataProvider::ReplayMode replay_mode_;
    std::map<std::string, std::optional<BarStruct>> latest_bars_;
    std::map<std::string, bool> has_more_data_;
    uint64_t previous_time_ = 0;  // Previous timestamp for time change detection
    quanttrader::log::LoggerPtr logger_ = nullptr;  // Logger for this controller
};

} // namespace replay
} // namespace data
} // namespace quanttrader