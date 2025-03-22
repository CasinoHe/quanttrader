#pragma once

#include "data/common/data_struct.h"
#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <optional>

namespace quanttrader {
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
class DataProvider {
public:
    DataProvider(const std::string_view &data_prefix, DataParamsType params)
        : data_prefix_(data_prefix), params_(params) {
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
    std::string get_data_prefix() const { return data_prefix_; }

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
    T get_data_by_prefix(const std::string &&data, std::optional<T> default_value=std::nullopt) {
        std::string key = data_prefix_ + data;
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

    std::string data_prefix_ {};
    DataParamsType params_ {nullptr};
    quanttrader::log::LoggerPtr logger_ {nullptr};
};

} // namespace provider
} // namespace data
} // namespace quanttrader