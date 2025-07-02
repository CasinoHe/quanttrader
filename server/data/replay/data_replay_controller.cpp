#include "data_replay_controller.h"
#include "logger/quantlogger.h"
#include "basic/time/time_util.h"
#include <algorithm>
#include <limits>
#include <ctime>

// Avoid macro conflicts with std::numeric_limits::max
#undef min
#undef max

namespace quanttrader {
namespace data {
namespace replay {

DataReplayController::DataReplayController()
    : replay_mode_(provider::DataProvider::ReplayMode::NORMAL) {
}

bool DataReplayController::add_data_provider(const std::string& name, std::shared_ptr<provider::DataProvider> provider) {
    if (providers_.find(name) != providers_.end()) {
        return false;
    }

    providers_[name] = provider;
    has_more_data_[name] = true;
    return true;
}

bool DataReplayController::remove_data_provider(const std::string& name) {
    auto it = providers_.find(name);
    if (it == providers_.end()) {
        return false;
    }

    providers_.erase(it);
    has_more_data_.erase(name);
    latest_bars_.erase(name);
    return true;
}

std::shared_ptr<provider::DataProvider> DataReplayController::get_data_provider(const std::string& name) const {
    auto it = providers_.find(name);
    if (it == providers_.end()) {
        return nullptr;
    }
    return it->second;
}

bool DataReplayController::start() {
    bool success = true;

    // Start and prepare all data providers
    for (auto& [name, provider] : providers_) {
        provider->set_replay_mode(replay_mode_);

        if (!provider->start_request_data()) {
            auto logger = quanttrader::log::get_common_rotation_logger("DataReplayController", "data");
            logger->error("Failed to start data provider: {}", name);
            success = false;
        }
    }

    return success;
}

void DataReplayController::stop() {
    for (auto& [name, provider] : providers_) {
        provider->terminate_request_data();
    }
}

void DataReplayController::set_replay_mode(provider::DataProvider::ReplayMode mode) {
    replay_mode_ = mode;
    for (auto& [name, provider] : providers_) {
        provider->set_replay_mode(mode);
    }
}

SynchronizedDataResult DataReplayController::next_synchronized() {
    SynchronizedDataResult result;
    auto logger = quanttrader::log::get_common_rotation_logger("DataReplayController", "data");
    
    if (providers_.empty()) {
        logger->debug("No providers available for synchronization");
        return result;
    }

    // Find the provider with the earliest timestamp
    uint64_t earliest_time = std::numeric_limits<uint64_t>::max();
    
    // First, check if we need to fetch new data for any provider
    for (auto& [name, provider] : providers_) {
        if (!has_more_data_[name]) {
            continue;  // Skip providers with no more data
        }

        if (latest_bars_.find(name) == latest_bars_.end() || !latest_bars_[name].has_value()) {
            // Fetch next bar for this provider
            auto next_bar = provider->next();
            if (next_bar.has_value()) {
                latest_bars_[name] = next_bar;
                logger->debug("Fetched new bar for provider {}: time={}", name, next_bar->time);
            } else {
                has_more_data_[name] = false;
                latest_bars_[name] = std::nullopt;
                logger->debug("No more data for provider {}", name);
                continue;  // No more data for this provider
            }
        }

        // Find the earliest timestamp among available bars
        if (latest_bars_[name].has_value() && latest_bars_[name]->time < earliest_time) {
            earliest_time = latest_bars_[name]->time;
        }
    }

    // If no valid bar was found, return empty result
    if (earliest_time == std::numeric_limits<uint64_t>::max()) {
        logger->debug("No valid bars found, returning empty result");
        return result;
    }

    result.current_time = earliest_time;
    
    // Check for time switches if we have a previous time
    if (previous_time_ > 0) {
        // Convert timestamps to tm structures for comparison
        std::time_t prev_seconds = previous_time_ / 1000000000ULL;
        std::time_t curr_seconds = earliest_time / 1000000000ULL;
        
        std::tm prev_tm = {};
        std::tm curr_tm = {};
        
#ifdef _WIN32
        gmtime_s(&prev_tm, &prev_seconds);
        gmtime_s(&curr_tm, &curr_seconds);
#else
        gmtime_r(&prev_seconds, &prev_tm);
        gmtime_r(&curr_seconds, &curr_tm);
#endif

        // Check for day change
        if (prev_tm.tm_year != curr_tm.tm_year || 
            prev_tm.tm_mon != curr_tm.tm_mon || 
            prev_tm.tm_mday != curr_tm.tm_mday) {
            result.day_changed = true;
            logger->debug("Day changed from {}-{:02d}-{:02d} to {}-{:02d}-{:02d}", 
                         prev_tm.tm_year + 1900, prev_tm.tm_mon + 1, prev_tm.tm_mday,
                         curr_tm.tm_year + 1900, curr_tm.tm_mon + 1, curr_tm.tm_mday);
        }
        
        // Check for hour change
        if (prev_tm.tm_hour != curr_tm.tm_hour || result.day_changed) {
            result.hour_changed = true;
            logger->debug("Hour changed from {:02d} to {:02d}", prev_tm.tm_hour, curr_tm.tm_hour);
        }
        
        // Check for minute change
        if (prev_tm.tm_min != curr_tm.tm_min || result.hour_changed) {
            result.minute_changed = true;
            logger->debug("Minute changed from {:02d} to {:02d}", prev_tm.tm_min, curr_tm.tm_min);
        }
        
        // Log current aligned time
        logger->debug("Current aligned time: {}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} (ns: {})",
                     curr_tm.tm_year + 1900, curr_tm.tm_mon + 1, curr_tm.tm_mday,
                     curr_tm.tm_hour, curr_tm.tm_min, curr_tm.tm_sec, earliest_time);
    } else {
        // First time initialization
        std::time_t curr_seconds = earliest_time / 1000000000ULL;
        std::tm curr_tm = {};
        
#ifdef _WIN32
        gmtime_s(&curr_tm, &curr_seconds);
#else
        gmtime_r(&curr_seconds, &curr_tm);
#endif
        
        logger->debug("Initializing with time: {}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} (ns: {})",
                     curr_tm.tm_year + 1900, curr_tm.tm_mon + 1, curr_tm.tm_mday,
                     curr_tm.tm_hour, curr_tm.tm_min, curr_tm.tm_sec, earliest_time);
    }

    // Process each provider based on its timestamp compared to the earliest time
    for (auto& [name, provider] : providers_) {
        if (latest_bars_.find(name) != latest_bars_.end() && latest_bars_[name].has_value()) {
            // This provider has data
            if (latest_bars_[name]->time <= earliest_time) {
                // This provider has the earliest timestamp, include its data
                result.data[name] = latest_bars_[name];
                logger->debug("Including data from provider {} at time {}", name, latest_bars_[name]->time);
                // Clear this provider's cached bar so it fetches a new one next time
                latest_bars_[name] = std::nullopt;
            } else {
                // This provider's data is from a future time, roll it back
                provider->rollback();
                result.data[name] = std::nullopt;
                logger->debug("Rolling back provider {} (future time: {} vs current: {})", 
                             name, latest_bars_[name]->time, earliest_time);
            }
        } else {
            // No data for this provider
            result.data[name] = std::nullopt;
            logger->debug("No data available for provider {}", name);
        }
    }

    // Update previous time for next iteration
    previous_time_ = earliest_time;

    return result;
}

void DataReplayController::next_step() {
    if (replay_mode_ == provider::DataProvider::ReplayMode::STEPPED) {
        for (auto& [name, provider] : providers_) {
            provider->next_step();
        }
    }
}

bool DataReplayController::rewind() {
    bool success = true;
    for (auto& [name, provider] : providers_) {
        if (!provider->rewind()) {
            success = false;
        }
        has_more_data_[name] = true;
    }
    
    // Clear cached bars and reset previous time
    latest_bars_.clear();
    previous_time_ = 0;
    
    return success;
}

bool DataReplayController::all_data_ready() const {
    for (const auto& [name, provider] : providers_) {
        if (!provider->is_data_ready()) {
            return false;
        }
    }
    return !providers_.empty();
}

bool DataReplayController::has_more_data() const {
    for (const auto& [name, has_more] : has_more_data_) {
        if (has_more) {
            return true;
        }
    }
    return false;
}

} // namespace replay
} // namespace data
} // namespace quanttrader