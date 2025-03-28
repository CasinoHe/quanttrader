#include "yahoo_data_feed.h"
#include "data/storage/storage_factory.h"
#include "time/time_with_zone.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <ctime>
#include <filesystem>
// Replace nlohmann/json with rapidjson
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

namespace fs = std::filesystem;
namespace qtime = quanttrader::time;

namespace quanttrader {
namespace data {
namespace feed {

YahooDataFeed::YahooDataFeed(const std::string_view &data_name, provider::DataParamsType params)
    : provider::DataProvider(data_name, params) {
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle_ = curl_easy_init();
}

YahooDataFeed::~YahooDataFeed() {
    // Clean up curl
    if (curl_handle_) {
        curl_easy_cleanup(curl_handle_);
        curl_handle_ = nullptr;
    }
    curl_global_cleanup();
}

bool YahooDataFeed::prepare_data() {
    // Extract configuration from parameters
    symbol_ = get_config_value<std::string>("symbol");
    if (symbol_.empty()) {
        logger_->error("Symbol is required for Yahoo Finance data feed");
        return false;
    }
    
    // Parse time range parameters
    start_date_ = get_config_value<std::string>("start_date", "");
    end_date_ = get_config_value<std::string>("end_date", "");
    
    // Parse interval and period
    interval_ = get_config_value<std::string>("interval", "1d");
    period_ = get_config_value<std::string>("period", "1d");
    
    // Map Yahoo interval to our bar type
    if (interval_ == "1m" || interval_ == "2m" || interval_ == "5m" || 
        interval_ == "15m" || interval_ == "30m" || interval_ == "60m") {
        
        bar_type_ = BarType::Minute;
        
        if (interval_ == "1m") bar_size_ = 1;
        else if (interval_ == "2m") bar_size_ = 2;
        else if (interval_ == "5m") bar_size_ = 5;
        else if (interval_ == "15m") bar_size_ = 15;
        else if (interval_ == "30m") bar_size_ = 30;
        else if (interval_ == "60m") bar_size_ = 60;
    }
    else if (interval_ == "1h") {
        bar_type_ = BarType::Hour;
        bar_size_ = 1;
    }
    else if (interval_ == "1d" || interval_ == "5d") {
        bar_type_ = BarType::Day;
        bar_size_ = (interval_ == "1d") ? 1 : 5;
    }
    else if (interval_ == "1wk") {
        bar_type_ = BarType::Week;
        bar_size_ = 1;
    }
    else if (interval_ == "1mo" || interval_ == "3mo") {
        bar_type_ = BarType::Month;
        bar_size_ = (interval_ == "1mo") ? 1 : 3;
    }
    else {
        logger_->error("Unsupported Yahoo Finance interval: {}", interval_);
        return false;
    }
    
    // Configure storage options
    use_storage_ = get_config_value<bool>("use_storage", false);
    store_after_download_ = get_config_value<bool>("store_after_download", false);
    storage_path_ = get_config_value<std::string>("storage_path", "./data");
    
    // Get storage type from configuration or use default
    std::string storage_type = get_config_value<std::string>("storage_type", "file");
    
    // Initialize the bar line
    bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
    
    // Initialize storage if needed using the factory
    if (use_storage_ || store_after_download_) {
        storage_ = storage::StorageFactory::create(storage_type);
        if (!storage_->initialize(storage_path_)) {
            logger_->error("Failed to initialize {} storage at path: {}", storage_type, storage_path_);
            return false;
        }
    }
    
    return true;
}

bool YahooDataFeed::start_request_data() {
    // Try to load from storage first if enabled
    if (use_storage_ && storage_) {
        if (storage_->has_data(symbol_, bar_type_, bar_size_)) {
            logger_->info("Loading {} data for {} from storage", 
                get_bar_type_string(bar_type_, bar_size_), symbol_);
            if (load_from_storage()) {
                data_ready_ = true;
                return true;
            } else {
                logger_->warn("Failed to load data from storage, falling back to Yahoo Finance API");
            }
        }
    }
    
    // Download data from Yahoo Finance
    if (download_data()) {
        if (parse_yahoo_data(response_data_)) {
            data_ready_ = true;
            
            // Store the data if requested
            if (store_after_download_ && storage_) {
                save_to_storage();
            }
            
            return true;
        }
    }
    
    return false;
}

bool YahooDataFeed::terminate_request_data() {
    // Nothing to do for Yahoo data feed
    return true;
}

bool YahooDataFeed::is_data_ready() {
    return data_ready_;
}

std::optional<BarStruct> YahooDataFeed::next() {
    if (!data_ready_ || !bar_line_) {
        return std::nullopt;
    }
    
    return bar_line_->next();
}

size_t YahooDataFeed::curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Append the data to our string
    if (userdata) {
        std::string *str = static_cast<std::string*>(userdata);
        str->append(ptr, size * nmemb);
        return size * nmemb;
    }
    return 0;
}

bool YahooDataFeed::download_data() {
    if (!curl_handle_) {
        logger_->error("CURL not initialized");
        return false;
    }
    
    // Prepare URL
    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/";
    url += symbol_;
    url += "?interval=" + interval_;
    url += "&period1=" + start_date_;
    url += "&period2=" + end_date_;
    
    // Set up curl request
    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, curl_write_callback);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response_data_);
    curl_easy_setopt(curl_handle_, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 30L);
    
    // Clear the response buffer
    response_data_.clear();
    
    // Perform the request
    CURLcode res = curl_easy_perform(curl_handle_);
    if (res != CURLE_OK) {
        logger_->error("CURL request failed: {}", curl_easy_strerror(res));
        return false;
    }
    
    // Check HTTP status code
    long http_code = 0;
    curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        logger_->error("HTTP error: {}", http_code);
        return false;
    }
    
    logger_->info("Successfully downloaded data for {} ({} bytes)", symbol_, response_data_.size());
    return true;
}

bool YahooDataFeed::parse_yahoo_data(const std::string& data) {
    try {
        // Parse JSON response
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (document.HasParseError()) {
            logger_->error("JSON parse error: {}", rapidjson::GetParseError_En(document.GetParseError()));
            return false;
        }
        
        // Navigate to the actual data
        const rapidjson::Value& result = document["chart"]["result"][0];
        const rapidjson::Value& timestamps = result["timestamp"];
        const rapidjson::Value& quote = result["indicators"]["quote"][0];
        
        // Get data arrays
        const rapidjson::Value& open = quote["open"];
        const rapidjson::Value& high = quote["high"];
        const rapidjson::Value& low = quote["low"];
        const rapidjson::Value& close = quote["close"];
        const rapidjson::Value& volume = quote["volume"];
        
        // Process each data point
        int count = 0;
        for (rapidjson::SizeType i = 0; i < timestamps.Size(); i++) {
            // Skip if any value is null/missing
            if (open[i].IsNull() || high[i].IsNull() || 
                low[i].IsNull() || close[i].IsNull() || 
                volume[i].IsNull()) {
                continue;
            }
            
            // Convert timestamp to nanoseconds (Yahoo uses seconds)
            uint64_t time_ns = timestamps[i].GetUint64() * 1000000000ULL;
            
            // Add the bar to our bar line
            if (bar_line_->push_data(
                time_ns, 
                open[i].GetDouble(), 
                high[i].GetDouble(), 
                low[i].GetDouble(), 
                close[i].GetDouble(), 
                Decimal(volume[i].GetDouble()), 
                Decimal(0), 
                0)) {
                count++;
            }
        }
        
        logger_->info("Successfully parsed {} data points for {}", count, symbol_);
        return count > 0;
    }
    catch (const std::exception& e) {
        logger_->error("Failed to parse Yahoo Finance data: {}", e.what());
        return false;
    }
}

bool YahooDataFeed::load_from_storage() {
    if (!storage_) {
        return false;
    }
    
    auto bars_opt = storage_->load_bars(symbol_, bar_type_, bar_size_);
    if (!bars_opt.has_value()) {
        return false;
    }
    
    BarSeries& bars = bars_opt.value();
    
    // Add all bars to our bar line
    int count = 0;
    for (size_t i = 0; i < bars.start_time.size(); ++i) {
        if (bar_line_->push_data(
            bars.start_time[i], 
            bars.open[i], 
            bars.high[i], 
            bars.low[i], 
            bars.close[i], 
            bars.volume[i], 
            bars.wap[i], 
            bars.count[i])) {
            count++;
        }
    }
    
    logger_->info("Loaded {} data points from storage for {}", count, symbol_);
    return count > 0;
}

bool YahooDataFeed::save_to_storage() {
    if (!storage_ || !bar_line_) {
        return false;
    }
    
    // Extract all data from bar line
    BarSeries bars;
    std::optional<BarStruct> bar;
    while ((bar = bar_line_->next()).has_value()) {
        bars.start_time.push_back(bar->time);
        bars.open.push_back(bar->open);
        bars.high.push_back(bar->high);
        bars.low.push_back(bar->low);
        bars.close.push_back(bar->close);
        bars.volume.push_back(bar->volume);
        bars.wap.push_back(bar->swap);
        bars.count.push_back(bar->count);
    }
    
    if (bars.start_time.empty()) {
        logger_->warn("No data to save to storage for {}", symbol_);
        return false;
    }
    
    bool result = storage_->store_bars(symbol_, bar_type_, bar_size_, bars);
    if (result) {
        logger_->info("Successfully saved {} data points to storage for {}", 
                    bars.start_time.size(), symbol_);
    } else {
        logger_->error("Failed to save data to storage for {}", symbol_);
    }
    
    return result;
}

} // namespace feed
} // namespace data
} // namespace quanttrader