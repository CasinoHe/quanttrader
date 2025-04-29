#pragma once

#include "data/common/data_struct.h"
#include "data/util/bar_line.h"
#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <optional>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace quanttrader {
namespace broker {
class BrokerProvider;
}
namespace data {
namespace provider {

using DataParamsType = std::shared_ptr<std::unordered_map<std::string, std::any>>;

/**
 * @brief Abstract base class for all data providers
 * 
 * This class defines the interface that all data providers must implement.
 * It handles the common functionality of data providers and delegates specific
 * implementations to derived classes.
 */
class DataProvider : public std::enable_shared_from_this<DataProvider> {
public:
    // Replay mode options
    enum class ReplayMode {
        NORMAL,     // Return data as fast as possible
        REALTIME,   // Simulate real-time data delivery based on bar timestamps
        STEPPED     // Manual stepping through data
    };

    DataProvider(const std::string_view &data_name, DataParamsType params)
        : data_name_(data_name), params_(params) {
        logger_ = quanttrader::log::get_common_rotation_logger("DataProvider", "data");
    }
    
    virtual ~DataProvider() = default;

    /**
     * @brief Prepare the data source
     * 
     * This method initializes the data provider and prepares it for data retrieval.
     * 
     * @return true if initialization was successful, false otherwise
     */
    virtual bool prepare_data() = 0;
    
    /**
     * @brief Start requesting data from the source
     * 
     * This method initiates the data request process.
     * 
     * @return true if the request was successfully initiated, false otherwise
     */
    virtual bool start_request_data() = 0;
    
    /**
     * @brief Terminate the data request
     * 
     * This method stops any ongoing data requests.
     * 
     * @return true if the termination was successful, false otherwise
     */
    virtual bool terminate_request_data() = 0;
    
    /**
     * @brief Check if data is ready for consumption
     * 
     * @return true if data is ready, false otherwise
     */
    virtual bool is_data_ready() = 0;

    /**
     * @brief Get the next data point
     * 
     * @return The next data point or nullopt if there is no more data
     */
    virtual std::optional<BarStruct> next() = 0;

    /**
     * @brief Get the data prefix
     * 
     * @return The data prefix
     */
    std::string get_data_name() const { return data_name_; }

    /**
     * @brief Get the symbol name
     * 
     * @return The symbol name
     */
    std::string get_symbol() const { return symbol_; }

    /**
     * @brief Get the current bar type
     * 
     * @return The bar type
     */
    BarType get_bar_type() const { return bar_type_; }

    /**
     * @brief Get the current bar size
     * 
     * @return The bar size
     */
    unsigned int get_bar_size() const { return bar_size_; }

    /**
     * @brief Set the replay mode for the data provider
     * 
     * @param mode The replay mode to set
     */
    void set_replay_mode(ReplayMode mode) {
        replay_mode_ = mode;
        if (mode == ReplayMode::STEPPED) {
            step_ready_ = false;
        }
    }

    /**
     * @brief Get the current replay mode
     * 
     * @return The current replay mode
     */
    ReplayMode get_replay_mode() const {
        return replay_mode_;
    }

    /**
     * @brief Signal to provide the next step in STEPPED replay mode
     */
    void next_step() {
        if (replay_mode_ == ReplayMode::STEPPED) {
            std::lock_guard<std::mutex> lock(step_mutex_);
            step_ready_ = true;
            step_cv_.notify_one();
        }
    }

    /**
     * @brief Resample data to a different timeframe
     * 
     * @param target_type The target bar type
     * @param target_size The target bar size
     * @return A shared pointer to a new DataProvider with resampled data
     */
    virtual std::shared_ptr<DataProvider> resample(BarType target_type, unsigned int target_size);

    /**
     * @brief Rewind the data provider to the beginning
     * 
     * @return true if rewind was successful, false otherwise
     */
    virtual bool rewind();

    /**
     * @brief Convert a string representation of bar type to enum and size
     * 
     * @param bar_type_str String representation of bar type (e.g., "1 day", "5 min")
     * @return A pair containing the bar type enum and size
     */
    static std::pair<BarType, unsigned int> get_bar_type_from_string(const std::string &bar_type_str);

    /**
     * @brief Convert a bar type enum and size to string representation
     * 
     * @param type The bar type enum
     * @param size The bar size
     * @return String representation of the bar type
     */
    static std::string get_bar_type_string(BarType type, unsigned int size);

    virtual void set_broker(std::shared_ptr<broker::BrokerProvider> broker_provider) {
        broker_provider_ = broker_provider;
    }

protected:
    /**
     * @brief Helper method to retrieve data from parameters by prefix
     * 
     * @tparam T the type of the data to retrieve
     * @param data the key suffix to look for
     * @param default_value optional default value if key is not found
     * @return The retrieved value or the default value
     */
    template<typename T>
    T get_config_value(const std::string &&key, std::optional<T> default_value=std::nullopt) {
        auto iter = params_->find(key);
        if (iter == params_->end()) {
            if (default_value.has_value()) {
                return default_value.value();
            } else {
                return {};
            }
        }
        return std::any_cast<T>(iter->second);
    }

    /**
     * @brief Wait for the appropriate time in REALTIME replay mode
     * 
     * @param current_bar The current bar
     * @param last_bar The previous bar
     */
    void wait_for_realtime(const BarStruct &current_bar, const BarStruct &last_bar) {
        if (replay_mode_ == ReplayMode::REALTIME && last_bar.time > 0) {
            auto time_diff_ns = current_bar.time - last_bar.time;
            auto sleep_time = std::chrono::nanoseconds(time_diff_ns);
            
            // Cap the sleep time to a reasonable value (e.g., 10 seconds)
            auto max_sleep = std::chrono::seconds(10);
            if (sleep_time > max_sleep) {
                sleep_time = max_sleep;
            }
            
            std::this_thread::sleep_for(sleep_time);
        }
    }

    /**
     * @brief Wait for step signal in STEPPED replay mode
     */
    void wait_for_step() {
        if (replay_mode_ == ReplayMode::STEPPED) {
            std::unique_lock<std::mutex> lock(step_mutex_);
            step_cv_.wait(lock, [this] { return step_ready_; });
            step_ready_ = false;
        }
    }

    std::string data_name_ {};
    DataParamsType params_ {nullptr};
    quanttrader::log::LoggerPtr logger_ {nullptr};
    
    // Common attributes for data providers
    std::string symbol_ {};
    BarType bar_type_ {BarType::NONE};
    unsigned int bar_size_ {0};
    std::shared_ptr<util::BarLine> bar_line_ {nullptr};
    bool data_ready_ {false};
    
    // Replay control
    ReplayMode replay_mode_ {ReplayMode::NORMAL};
    BarStruct last_bar_ {};
    bool step_ready_ {false};
    std::mutex step_mutex_;
    std::condition_variable step_cv_;
    std::shared_ptr<broker::BrokerProvider> broker_provider_ {nullptr};
};

} // namespace provider
} // namespace data
} // namespace quanttrader