#include "data_replay_controller.h"
#include "logger/quantlogger.h"
#include <algorithm>
#include <limits>

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

std::map<std::string, std::optional<BarStruct>> DataReplayController::next_synchronized() {
    if (providers_.empty()) {
        return {};
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
            } else {
                has_more_data_[name] = false;
                latest_bars_[name] = std::nullopt;
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
        return {};
    }

    // Prepare the result map
    std::map<std::string, std::optional<BarStruct>> result;
    
    // Process each provider based on its timestamp compared to the earliest time
    for (auto& [name, provider] : providers_) {
        if (latest_bars_.find(name) != latest_bars_.end() && latest_bars_[name].has_value()) {
            // This provider has data
            if (latest_bars_[name]->time == earliest_time) {
                // This provider has the earliest timestamp, include its data
                result[name] = latest_bars_[name];
                // Clear this provider's cached bar so it fetches a new one next time
                latest_bars_[name] = std::nullopt;
            } else {
                // This provider's data is from a future time, roll it back
                provider->rollback();
                result[name] = std::nullopt;
            }
        } else {
            // No data for this provider
            result[name] = std::nullopt;
        }
    }

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
    
    // Clear cached bars
    latest_bars_.clear();
    
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