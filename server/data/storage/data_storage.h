#pragma once

#include "data/common/data_struct.h"
#include <string>
#include <memory>
#include <optional>
#include <vector>

namespace quanttrader {
namespace data {
namespace storage {

/**
 * @brief Abstract base class for data storage
 * 
 * This class defines the interface for storing and retrieving market data
 * from various storage backends (files, databases, etc.)
 */
class DataStorage {
public:
    DataStorage() = default;
    virtual ~DataStorage() = default;

    /**
     * @brief Initialize the storage system
     * 
     * @param storage_path Path to the storage location
     * @return true if initialization was successful, false otherwise
     */
    virtual bool initialize(const std::string& storage_path) = 0;

    /**
     * @brief Store a series of bar data
     * 
     * @param symbol The symbol/ticker for the data
     * @param bar_type The type of bar (second, minute, etc.)
     * @param bar_size The size of the bar
     * @param bars The series of bars to store
     * @return true if storage was successful, false otherwise
     */
    virtual bool store_bars(const std::string& symbol, 
                           BarType bar_type, 
                           unsigned int bar_size,
                           const BarSeries& bars) = 0;

    /**
     * @brief Load a series of bar data
     * 
     * @param symbol The symbol/ticker for the data
     * @param bar_type The type of bar (second, minute, etc.)
     * @param bar_size The size of the bar
     * @param start_time Optional start time to filter data
     * @param end_time Optional end time to filter data
     * @return The loaded bar series or nullopt if loading failed
     */
    virtual std::optional<BarSeries> load_bars(const std::string& symbol,
                                              BarType bar_type,
                                              unsigned int bar_size,
                                              std::optional<uint64_t> start_time = std::nullopt,
                                              std::optional<uint64_t> end_time = std::nullopt) = 0;

    /**
     * @brief Check if data exists for a symbol
     * 
     * @param symbol The symbol/ticker to check
     * @param bar_type The type of bar
     * @param bar_size The size of the bar
     * @return true if data exists, false otherwise
     */
    virtual bool has_data(const std::string& symbol, 
                         BarType bar_type, 
                         unsigned int bar_size) = 0;

    /**
     * @brief Get list of available symbols
     * 
     * @return Vector of available symbols
     */
    virtual std::vector<std::string> get_available_symbols() = 0;
};

} // namespace storage
} // namespace data
} // namespace quanttrader