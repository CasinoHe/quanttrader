#pragma once

#include "data_storage.h"
#include "logger/quantlogger.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <zlib.h>

namespace quanttrader {
namespace data {
namespace storage {

/**
 * @brief File-based data storage implementation
 * 
 * This class implements the DataStorage interface using files to store market data.
 * It supports compression with zlib to reduce storage requirements.
 */
class FileStorage : public DataStorage {
public:
    FileStorage();
    ~FileStorage() override;

    bool initialize(const std::string& storage_path) override;
    
    bool store_bars(const std::string& symbol, 
                   BarType bar_type,
                   unsigned int bar_size,
                   const BarSeries& bars) override;
    
    std::optional<BarSeries> load_bars(const std::string& symbol,
                                      BarType bar_type,
                                      unsigned int bar_size,
                                      std::optional<uint64_t> start_time = std::nullopt,
                                      std::optional<uint64_t> end_time = std::nullopt) override;
    
    bool has_data(const std::string& symbol, 
                 BarType bar_type,
                 unsigned int bar_size) override;

    std::vector<std::string> get_available_symbols() override;

private:
    std::string get_bar_type_string(BarType bar_type, unsigned int bar_size) const;
    std::string get_data_file_path(const std::string& symbol, BarType bar_type, unsigned int bar_size) const;
    bool compress_data(const char* source, size_t source_size, std::vector<char>& dest);
    bool decompress_data(const char* source, size_t source_size, std::vector<char>& dest, size_t expected_size);

    std::string base_path_;
    quanttrader::log::LoggerPtr logger_;
};

} // namespace storage
} // namespace data
} // namespace quanttrader