#include "data_replay_controller.h"
#include "basic/time/time_util.h"
#include "basic/time/time_with_zone.h"
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
    logger_ = quanttrader::log::get_common_rotation_logger("DataReplayController", "data");
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
            logger_->error("Failed to start data provider: {}", name);
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

void compare_time_changes(uint64_t previous_time, uint64_t current_time, const std::string& timezone, SynchronizedDataResult& result, const std::shared_ptr<spdlog::logger>& logger, std::string_view data_name) {
    if (previous_time > 0) {
        quanttrader::time::TimeWithZone prev_twz(previous_time, timezone);
        quanttrader::time::TimeWithZone curr_twz(current_time, timezone);

        auto prev_local = prev_twz.get_local_time();
        auto curr_local = curr_twz.get_local_time();

        auto prev_days = std::chrono::floor<std::chrono::days>(prev_local);
        auto curr_days = std::chrono::floor<std::chrono::days>(curr_local);
        if (prev_days != curr_days) {
            result.day_changed = true;
            logger->debug("Day changed (epoch): {} -> {}", prev_days.time_since_epoch().count(), curr_days.time_since_epoch().count());
        }

        auto prev_hour = std::chrono::floor<std::chrono::hours>(prev_local);
        auto curr_hour = std::chrono::floor<std::chrono::hours>(curr_local);
        if (prev_hour != curr_hour || result.day_changed) {
            result.hour_changed = true;
            logger->debug("Hour changed (epoch): {} -> {}", prev_hour.time_since_epoch().count(), curr_hour.time_since_epoch().count());
        }

        auto prev_min = std::chrono::floor<std::chrono::minutes>(prev_local);
        auto curr_min = std::chrono::floor<std::chrono::minutes>(curr_local);
        if (prev_min != curr_min || result.hour_changed) {
            result.minute_changed = true;
            logger->debug("Minute changed (epoch): {} -> {}", prev_min.time_since_epoch().count(), curr_min.time_since_epoch().count());
        }
        logger->info("Current aligned time: {} (ns: {}), data {}", curr_twz.to_string_with_name(), current_time, data_name);
    } else {
        quanttrader::time::TimeWithZone curr_twz(current_time, timezone);
        logger->info("Initializing with time: {} (ns: {}), data {}", curr_twz.to_string_with_name(), current_time, data_name);
    }
}

// Helper: fetch next valid bar for a provider
void fetch_next_valid_bar(const std::string& name, std::shared_ptr<provider::DataProvider> provider, std::map<std::string, std::optional<BarStruct>>& latest_bars_, std::map<std::string, bool>& has_more_data_, uint64_t previous_time_, const std::shared_ptr<spdlog::logger>& logger) {
    bool need_new_data = false;
    if (latest_bars_.find(name) == latest_bars_.end() || !latest_bars_[name].has_value()) {
        need_new_data = true;
    } else if (previous_time_ > 0 && latest_bars_[name]->time < previous_time_) {
        need_new_data = true;
        logger->debug("Advancing provider {} past old cached time {} (current time: {})", name, latest_bars_[name]->time, previous_time_);
    }
    if (need_new_data) {
        std::optional<BarStruct> next_bar;
        do {
            next_bar = provider->next();
            if (next_bar.has_value()) {
                logger->debug("Fetched bar for provider {}: time={}", name, next_bar->time);
                if (previous_time_ == 0 || next_bar->time >= previous_time_) {
                    latest_bars_[name] = next_bar;
                    break;
                } else {
                    logger->info("Skipping old bar for provider {} (time: {} < current: {})", name, next_bar->time, previous_time_);
                }
            } else {
                has_more_data_[name] = false;
                latest_bars_[name] = std::nullopt;
                logger->debug("No more data for provider {}", name);
                break;
            }
        } while (next_bar.has_value() && next_bar->time < previous_time_);
    }
}

// Helper: find earliest timestamp among all providers
void find_earliest_time(const std::map<std::string, std::optional<BarStruct>>& latest_bars_, const std::map<std::string, bool>& has_more_data_, uint64_t previous_time_, uint64_t& earliest_time, std::string_view& data_name) {
    earliest_time = std::numeric_limits<uint64_t>::max();
    for (const auto& [name, bar_opt] : latest_bars_) {
        if (!has_more_data_.at(name)) continue;
        if (bar_opt.has_value() && (previous_time_ == 0 || bar_opt->time >= previous_time_) && bar_opt->time < earliest_time) {
            earliest_time = bar_opt->time;
            data_name = name;
        }
    }
}

// Helper: process each provider for result.data
void process_providers_for_result(const std::map<std::string, std::optional<BarStruct>>& latest_bars_, std::map<std::string, std::optional<BarStruct>>& result_data, uint64_t earliest_time, const std::shared_ptr<spdlog::logger>& logger) {
    for (const auto& [name, bar_opt] : latest_bars_) {
        if (bar_opt.has_value()) {
            if (bar_opt->time <= earliest_time) {
                result_data[name] = bar_opt;
                logger->debug("Including data from provider {} at time {}", name, bar_opt->time);
            } else {
                result_data[name] = std::nullopt;
                logger->debug("Keeping future data from provider {} (future time: {} vs current: {})", name, bar_opt->time, earliest_time);
            }
        } else {
            result_data[name] = std::nullopt;
            logger->debug("No data available for provider {}", name);
        }
    }
}

SynchronizedDataResult DataReplayController::next_synchronized() {
    SynchronizedDataResult result;
    if (providers_.empty()) {
        logger_->debug("No providers available for synchronization");
        return result;
    }

    // Step 1: Fetch next valid bar for each provider
    for (auto& [name, provider] : providers_) {
        if (!has_more_data_[name]) continue;
        fetch_next_valid_bar(name, provider, latest_bars_, has_more_data_, previous_time_, logger_);
    }

    // Step 2: Find the earliest timestamp among all providers
    uint64_t earliest_time = std::numeric_limits<uint64_t>::max();
    std::string_view data_name;
    find_earliest_time(latest_bars_, has_more_data_, previous_time_, earliest_time, data_name);

    // Step 3: If no valid bar was found, return empty result
    if (earliest_time == std::numeric_limits<uint64_t>::max()) {
        logger_->debug("No valid bars found, returning empty result");
        return result;
    }

    result.current_time = earliest_time;

    // Step 4: Compare time changes (day/hour/minute)
    std::string timezone = "UTC";
    if (!providers_.empty()) {
        timezone = providers_.begin()->second->get_timezone();
    }
    compare_time_changes(previous_time_, earliest_time, timezone, result, logger_, data_name);

    // Step 5: Process each provider for result.data and clear used bars
    process_providers_for_result(latest_bars_, result.data, earliest_time, logger_);

    // Step 6: Update previous time for next iteration
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