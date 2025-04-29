#pragma once

#include "data/common/data_provider.h"
#include "data/util/bar_line.h"
#include "data/storage/data_storage.h"
#include <curl/curl.h>
#include <memory>
#include <string>
#include <optional>

namespace quanttrader {
namespace data {
namespace feed {

/**
 * @brief Yahoo Finance data feed implementation
 * 
 * This class implements a data feed that fetches market data from Yahoo Finance
 * and optionally persists it to the storage layer.
 */
class YahooDataFeed : public provider::DataProvider {
public:
    YahooDataFeed(const std::string_view &data_name, provider::DataParamsType params);
    ~YahooDataFeed();

    // Implementation of DataProvider interface
    bool prepare_data() override;
    bool start_request_data() override;
    bool terminate_request_data() override;
    bool is_data_ready() override;
    std::optional<BarStruct> next() override;

private:
    bool download_data();
    bool parse_yahoo_data(const std::string& data);
    bool load_from_storage();
    bool save_to_storage();
    static size_t curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

    std::string symbol_;
    std::string period_ = "1d";       // 1d, 1wk, 1mo
    std::string interval_ = "1d";     // 1m, 2m, 5m, 15m, 30m, 60m, 1h, 1d, 5d, 1wk, 1mo, 3mo
    std::string start_date_;
    std::string end_date_;
    std::string storage_path_;
    std::string response_data_;
    
    bool use_storage_ = false;
    bool store_after_download_ = false;
    bool data_ready_ = false;
    
    BarType bar_type_ = BarType::NONE;
    unsigned int bar_size_ = 0;
    std::shared_ptr<util::BarLine> bar_line_;
    std::shared_ptr<storage::DataStorage> storage_;
    
    CURL* curl_handle_ = nullptr;
};

} // namespace feed
} // namespace data
} // namespace quanttrader