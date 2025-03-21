#pragma once

#include "data/provider/data_provider.h"
#include "data/util/bar_line.h"
#include "data/storage/data_storage.h"
#include <memory>
#include <string>
#include <optional>

namespace quanttrader {
namespace data {
namespace feed {

/**
 * @brief CSV file data feed implementation
 * 
 * This class implements a data feed that reads market data from CSV files
 * and optionally persists it to the storage layer.
 */
class CsvDataFeed : public provider::DataProvider {
public:
    CsvDataFeed(const std::string_view &data_prefix, provider::DataParamsType params);
    ~CsvDataFeed() = default;

    // Implementation of DataProvider interface
    bool prepare_data() override;
    bool start_request_data() override;
    bool terminate_request_data() override;
    bool is_data_ready() override;
    std::optional<BarStruct> next() override;

private:
    bool load_from_csv();
    bool load_from_storage();
    bool save_to_storage();
    bool parse_date_time(const std::string& date_time_str, uint64_t& time_ns);

    std::string csv_file_path_;
    std::string symbol_;
    std::string date_format_;
    std::string storage_path_;
    bool use_storage_ = false;
    bool store_after_load_ = false;
    bool data_ready_ = false;
    
    BarType bar_type_ = BarType::NONE;
    unsigned int bar_size_ = 0;
    std::shared_ptr<util::BarLine> bar_line_;
    std::shared_ptr<storage::DataStorage> storage_;
    
    // CSV parsing options
    char delimiter_ = ',';
    int date_time_column_ = 0;
    int open_column_ = 1;
    int high_column_ = 2;
    int low_column_ = 3;
    int close_column_ = 4;
    int volume_column_ = 5;
    bool has_header_ = true;
};

} // namespace feed
} // namespace data
} // namespace quanttrader