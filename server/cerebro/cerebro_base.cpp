#include "cerebro_base.h"
#include "broker/backtest_broker.h"
#include <thread>

namespace quanttrader {
namespace cerebro {

CerebroBase::CerebroBase(const std::string_view name) 
    : name_(name), stop_flag_(false) {
    logger_ = quanttrader::log::get_common_rotation_logger(name_, "cerebro");
    logger_->info("Created cerebro: {}", name_);
    replay_controller_ = std::make_shared<data::replay::DataReplayController>();
    observers_.push_back(std::make_shared<observer::PerformanceObserver>());
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
    
    // Set the broker for the strategy if one is available
    if (broker_) {
        strategy->set_broker(broker_);
    }
    
    for (auto& obs : observers_) {
        strategy->add_observer(obs);
    }
    logger_->info("{} Added strategy: {}", name_, strategy->get_name());
    return true;
}

void CerebroBase::set_broker(std::shared_ptr<broker::AbstractBroker> broker) {
    broker_ = broker;
    
    // Set the broker for all existing strategies
    for (auto& strategy : strategies_) {
        strategy->set_broker(broker_);
    }
    
    logger_->info("{} Set broker for cerebro", name_);
}

bool CerebroBase::set_broker_type(const std::string& broker_type) {
    if (is_running_) {
        logger_->error("{} Cannot change broker type while running", name_);
        return false;
    }
    
    broker_type_ = broker_type;
    
    logger_->info("{} Set broker type to: {}", name_, broker_type_);
    return true;
}

bool CerebroBase::configure_backtest_broker(double starting_cash, double commission, double slippage, 
                                           double initial_margin, double maintenance_margin) {
    if (is_running_) {
        logger_->error("{} Cannot configure broker while running", name_);
        return false;
    }
    
    // Store the configuration temporarily, will be applied when broker is created
    backtest_config_.starting_cash = starting_cash;
    backtest_config_.commission_per_trade = commission;
    backtest_config_.slippage_percent = slippage;
    backtest_config_.initial_margin_percent = initial_margin;
    backtest_config_.maintenance_margin_percent = maintenance_margin;
    
    logger_->info("{} Configured backtest broker: starting_cash={}, commission={}, slippage={}%, initial_margin={}%, maintenance_margin={}%",
                 name_, starting_cash, commission, slippage, initial_margin, maintenance_margin);
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
    
    // Create and configure broker if not already set
    if (!broker_) {
        if (broker_type_ == "backtest") {
            auto backtest_broker = std::make_shared<broker::BacktestBroker>(backtest_config_.starting_cash);
            backtest_broker->set_commission(backtest_config_.commission_per_trade);
            backtest_broker->set_slippage(backtest_config_.slippage_percent);
            backtest_broker->set_margin_requirements(backtest_config_.initial_margin_percent, backtest_config_.maintenance_margin_percent);
            broker_ = backtest_broker;
        } else {
            logger_->error("{} Unsupported broker type: {}", name_, broker_type_);
            return false;
        }
        
        // Set the broker for all strategies
        for (auto& strategy : strategies_) {
            strategy->set_broker(broker_);
        }
        
        // Connect observers to broker
        for (auto& observer : observers_) {
            auto performance_observer = std::dynamic_pointer_cast<observer::PerformanceObserver>(observer);
            if (performance_observer) {
                performance_observer->set_broker(broker_);
            }
        }
        
        logger_->info("{} Created and configured {} broker", name_, broker_type_);
    }
    
    // Start the data providers
    if (!replay_controller_->start()) {
        logger_->error("{} Failed to start data providers", name_);
        return false;
    }

    // Check timezone consistency among all providers
    std::string first_timezone;
    bool timezone_inconsistent = false;
    for (const auto& [name, provider] : replay_controller_->get_providers()) {
        std::string tz = provider->get_timezone();
        if (first_timezone.empty()) {
            first_timezone = tz;
        } else if (tz != first_timezone) {
            logger_->error("{} Data provider '{}' has timezone '{}', expected '{}'", name_, name, tz, first_timezone);
            timezone_inconsistent = true;
        }
    }
    if (timezone_inconsistent) {
        logger_->error("{} Timezone inconsistency detected among data providers. All data must use the same timezone.", name_);
        return false;
    }

    // Set timezone for all observers from data providers
    if (!first_timezone.empty()) {
        for (auto& observer : observers_) {
            observer->set_timezone(first_timezone);
            logger_->info("{} Set timezone '{}' for observer", name_, first_timezone);
        }
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

    // Initialize cached BarSeries for all data providers
    cached_bar_series_.clear();

    is_prepared_ = true;
    logger_->info("CerebroBase {} preparation completed successfully", name_);
    return true;
}

bool CerebroBase::stop() {
    if (!is_running_) {
        logger_->warn("{} CerebroBase is not running", name_);
        return true;
    }
    
    stop_flag_.store(true);
    
    // Stop all data providers
    replay_controller_->stop();
    
    // Clear cached data for clean restart
    cached_bar_series_.clear();
    
    // Close all strategies
    for (auto& strategy : strategies_) {
        strategy->on_stop();
    }
    
    // Observer reports
    for (const auto& observer : observers_) {
        observer->report();
    }
    
    is_running_ = false;
    logger_->info("{} CerebroBase stopped successfully", name_);
    return true;
}

void CerebroBase::update_cached_bar_series(const std::string& data_provider_name, const std::optional<data::BarStruct>& new_bar) {
    if (!new_bar.has_value()) {
        return;
    }
    
    const auto& bar = new_bar.value();
    
    // Get or create the cached series for this data provider
    auto& series = cached_bar_series_[data_provider_name];
    
    // Add the new bar data to the series
    series.start_time.push_back(bar.time);
    series.open.push_back(bar.open);
    series.high.push_back(bar.high);
    series.low.push_back(bar.low);
    series.close.push_back(bar.close);
    series.volume.push_back(bar.volume);
    series.count.push_back(bar.count);
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
    auto sync_result = replay_controller_->next_synchronized();
    if (sync_result.data.empty()) {
        logger_->info("{} Empty data returned from replay controller", name_);
        return false;
    }
    
    // Update our historical data storage with the new data points
    for (const auto& [name, bar] : sync_result.data) {
        if (historical_data_.find(name) == historical_data_.end()) {
            historical_data_[name] = std::vector<std::optional<data::BarStruct>>();
        }
        historical_data_[name].push_back(bar);
        
        // Update cached BarSeries for efficient strategy access
        update_cached_bar_series(name, bar);
    }
    
    // Feed the cached BarSeries directly to all strategies (readonly access)
    for (auto& strategy : strategies_) {
        strategy->on_data_series(cached_bar_series_, sync_result.day_changed, 
                                 sync_result.hour_changed, sync_result.minute_changed);
    }

    std::map<std::string, double> price_map;
    for (const auto& [name, bar] : sync_result.data) {
        if (bar.has_value()) {
            auto provider = replay_controller_->get_data_provider(name);
            if (provider) {
                price_map[provider->get_symbol()] = bar->close;
            }
        }
    }
    
    // Update broker with market prices
    if (broker_) {
        broker_->update_market_prices(price_map);
        
        // If using backtest broker, process market data
        auto backtest_broker = std::dynamic_pointer_cast<broker::BacktestBroker>(broker_);
        if (backtest_broker) {
            backtest_broker->process_market_data(sync_result.current_time, price_map);
        }
    }
    
    for (auto& obs : observers_) {
        obs->update_market_value(sync_result.current_time, price_map);
    }

    return true;
}

bool CerebroBase::run() {
    if (!prepare()) {
        logger_->error("{} Failed to prepare for execution", name_);
        return false;
    }
    
    is_running_ = true;
    logger_->info("{} Starting execution...", name_);
    
    // Initialize all strategies
    for (auto& strategy : strategies_) {
        if (!strategy->on_start()) {
            logger_->error("{} Failed to start strategy: {}", name_, strategy->get_name());
            return false;
        }
    }
    
    // Main execution loop
    while (is_running_ && !stop_flag_.load()) {
        if (!process_next()) {
            logger_->info("{} No more data to process, execution complete", name_);
            break;
        }
        
        // Small delay to prevent tight loop in real-time scenarios
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    // Stop all strategies
    for (auto& strategy : strategies_) {
        strategy->on_stop();
    }
    
    // moved to stop function
    // Observer reports
    // for (const auto& observer : observers_) {
    //     observer->report();
    // }
    // is_running_ = false;

    logger_->info("{} Execution completed", name_);
    return true;
}

} // namespace cerebro
} // namespace quanttrader