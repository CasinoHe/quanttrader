#include "cerebro_base.h"

namespace quanttrader {
namespace cerebro {

CerebroBase::CerebroBase(const std::string_view name) : name_(name), stop_flag_(false) {
        logger_ = quanttrader::log::get_common_rotation_logger(name_, "cerebro");
        logger_->info("Created cerebro: {} ", name_);
    logger_ = quanttrader::log::get_common_rotation_logger("CerebroBase", "cerebro");
    replay_controller_ = std::make_shared<data::replay::DataReplayController>();
}

CerebroBase::~CerebroBase() {
    stop();
    logger_->info("Destroying cerebro: {}", name_);
}

bool CerebroBase::add_data(const std::string& name, std::shared_ptr<data::provider::DataProvider> provider) {
    if (!provider) {
        logger_->error("{} Cannot add null data provider with name: {}", name_, name);
        return false;
    }
    
    if (is_running_) {
        logger_->error("{} Cannot add data provider while cerebro is running", name_);
        return false;
    }
    
    if (!replay_controller_->add_data_provider(name, provider)) {
        logger_->error("{} Failed to add data provider: {}", name_, name);
        return false;
    }
    
    logger_->info("{} Added data provider: {} for symbol: {}", name_, name, provider->get_symbol());
    return true;
}

bool CerebroBase::add_strategy(std::shared_ptr<strategy::StrategyBase> strategy) {
    if (!strategy) {
        logger_->error("{} Cannot add null strategy", name_);
        return false;
    }
    
    if (is_running_) {
        logger_->error("{} Cannot add strategy while cerebro is running", name_);
        return false;
    }
    
    strategies_.push_back(strategy);
    logger_->info("{} Added strategy: {}", name_, strategy->get_name());
    return true;
}

void CerebroBase::set_replay_mode(data::provider::DataProvider::ReplayMode mode) {
    replay_controller_->set_replay_mode(mode);
    logger_->info("{} Set replay mode to: {}", name_, static_cast<int>(mode));
}

void CerebroBase::next_step() {
    replay_controller_->next_step();
}

bool CerebroBase::resample_data(const std::string& name, data::BarType target_type, unsigned int target_size) {
    auto provider = replay_controller_->get_data_provider(name);
    if (!provider) {
        logger_->error("{} Data provider not found for resampling: {}", name_, name);
        return false;
    }
    
    // Get current setup before resampling
    std::string symbol = provider->get_symbol();
    auto current_type = provider->get_bar_type();
    auto current_size = provider->get_bar_size();
    
    // Create a resampled provider
    auto resampled_provider = provider->resample(target_type, target_size);
    if (!resampled_provider) {
        logger_->error("{} Failed to resample data provider: {}, {}:{}",
                     name_,
                     name,
                     data::provider::DataProvider::get_bar_type_string(current_type, current_size),
                     data::provider::DataProvider::get_bar_type_string(target_type, target_size));
        return false;
    }
    
    // Replace the original provider with the resampled one
    std::string resampled_name = name + "_resampled";
    if (!replay_controller_->remove_data_provider(name)) {
        logger_->error("{} Failed to remove original data provider during resampling: {}", name_, name);
        return false;
    }
    
    if (!replay_controller_->add_data_provider(resampled_name, resampled_provider)) {
        logger_->error("{} Failed to add resampled data provider: {}", name_, resampled_name);
        // Try to restore the original provider
        replay_controller_->add_data_provider(name, provider);
        return false;
    }
    
    logger_->info("{} Resampled data provider: {} for symbol: {}, {}:{}",
                name_,
                name,
                symbol,
                data::provider::DataProvider::get_bar_type_string(current_type, current_size),
                data::provider::DataProvider::get_bar_type_string(target_type, target_size));
    
    return true;
}

bool CerebroBase::prepare() {
    if (is_prepared_) {
        return true;
    }
    
    if (strategies_.empty()) {
        logger_->error("{} No strategies added to cerebro", name_);
        return false;
    }
    
    // Start the data providers
    if (!replay_controller_->start()) {
        logger_->error("{} Failed to start data providers", name_);
        return false;
    }
    
    // Wait for all data to be ready
    int wait_time = 0;
    const int max_wait_time = static_cast<int>(wait_data_timeout_ / 1000);  // seconds
    bool all_ready = false;
    
    while (wait_time < max_wait_time) {
        if (replay_controller_->all_data_ready()) {
            all_ready = true;
            break;
        }
        
        logger_->info("{} Waiting for data providers to be ready, elapsed: {}s", name_, wait_time);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        wait_time++;
    }
    
    if (!all_ready) {
        logger_->error("{} Timed out({} seconds) waiting for data providers to be ready", name_, max_wait_time);
        return false;
    }
    
    is_prepared_ = true;
    logger_->info("CerebroBase {} preparation completed successfully", name_);
    return true;
}

bool CerebroBase::stop() {
    if (!is_running_) {
        logger_->warn("{} CerebroBase is not running", name_);
        return true;
    }
    
    // Stop all data providers
    replay_controller_->stop();
    
    // Close all strategies
    for (auto& strategy : strategies_) {
        strategy->on_stop();
    }
    
    is_running_ = false;
    logger_->info("{} CerebroBase stopped successfully", name_);
    return true;
}

bool CerebroBase::process_next() {
    if (!is_prepared_) {
        logger_->error("{} CerebroBase is not prepared", name_);
        return false;
    }
    
    // Check if we have more data
    if (!replay_controller_->has_more_data()) {
        logger_->info("{} No more data to process", name_);
        return false;
    }
    
    // Get next synchronized data points
    auto data_map = replay_controller_->next_synchronized();
    if (data_map.empty()) {
        logger_->info("{} Empty data returned from replay controller", name_);
        return false;
    }
    
    // Update our historical data storage with the new data points
    for (const auto& [name, bar] : data_map) {
        if (historical_data_.find(name) == historical_data_.end()) {
            historical_data_[name] = std::vector<std::optional<data::BarStruct>>();
        }
        historical_data_[name].push_back(bar);
    }
    
    // Create a map of data provider names to vectors of all historical bars for each strategy
    std::map<std::string, std::vector<std::optional<data::BarStruct>>> complete_data;
    for (const auto& [name, bars] : historical_data_) {
        complete_data[name] = bars;
    }
    
    // Feed the complete historical data to all strategies
    for (auto& strategy : strategies_) {
        strategy->on_data(complete_data);
    }
    
    return true;
}

} // namespace cerebro
} // namespace quanttrader