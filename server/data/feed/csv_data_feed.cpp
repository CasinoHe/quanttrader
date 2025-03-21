#include "csv_data_feed.h"
#include "data/storage/file_storage.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

namespace quanttrader {
namespace data {
namespace feed {

CsvDataFeed::CsvDataFeed(const std::string_view &data_prefix, provider::DataParamsType params)
    : provider::DataProvider(data_prefix, params) {
}

bool CsvDataFeed::prepare_data() {
    // Get required parameters
    csv_file_path_ = get_data_by_prefix<std::string>("_csv_path");
    if (csv_file_path_.empty()) {
        logger_->error("CSV file path not specified for data prefix: {}", data_prefix_);
        return false;
    }

    symbol_ = get_data_by_prefix<std::string>("_symbol");
    if (symbol_.empty()) {
        // Try to use the filename as symbol if not explicitly specified
        symbol_ = fs::path(csv_file_path_).stem().string();
        if (symbol_.empty()) {
            logger_->error("Symbol not specified for data prefix: {}", data_prefix_);
            return false;
        }
    }

    // Get bar type
    std::string bar_type_str = get_data_by_prefix<std::string>("_bar_type", "1 day");
    
    auto pos = bar_type_str.find(" ");
    if (pos == std::string::npos) {
        logger_->error("Invalid bar type format: {}", bar_type_str);
        return false;
    }
    
    try {
        bar_size_ = std::stoi(bar_type_str.substr(0, pos));
        std::string type = bar_type_str.substr(pos + 1);
        
        if (type == "sec" || type == "secs" || type == "second" || type == "seconds") {
            bar_type_ = BarType::Second;
        } else if (type == "min" || type == "mins" || type == "minute" || type == "minutes") {
            bar_type_ = BarType::Minute;
        } else if (type == "hour" || type == "hours") {
            bar_type_ = BarType::Hour;
        } else if (type == "day" || type == "days") {
            bar_type_ = BarType::Day;
        } else if (type == "week" || type == "weeks" || type == "W") {
            bar_type_ = BarType::Week;
        } else if (type == "month" || type == "months" || type == "M") {
            bar_type_ = BarType::Month;
        } else {
            logger_->error("Unknown bar type: {}", type);
            return false;
        }
    } catch (const std::exception& e) {
        logger_->error("Failed to parse bar type: {}, error: {}", bar_type_str, e.what());
        return false;
    }

    // Get optional parameters
    date_format_ = get_data_by_prefix<std::string>("_date_format", "%Y-%m-%d %H:%M:%S");
    delimiter_ = get_data_by_prefix<char>("_delimiter", ',');
    has_header_ = get_data_by_prefix<bool>("_has_header", true);
    
    // Column configuration
    date_time_column_ = get_data_by_prefix<int>("_datetime_column", 0);
    open_column_ = get_data_by_prefix<int>("_open_column", 1);
    high_column_ = get_data_by_prefix<int>("_high_column", 2);
    low_column_ = get_data_by_prefix<int>("_low_column", 3);
    close_column_ = get_data_by_prefix<int>("_close_column", 4);
    volume_column_ = get_data_by_prefix<int>("_volume_column", 5);
    
    // Storage options
    use_storage_ = get_data_by_prefix<bool>("_use_storage", false);
    store_after_load_ = get_data_by_prefix<bool>("_store_after_load", false);
    storage_path_ = get_data_by_prefix<std::string>("_storage_path", "./data_storage");
    
    // Initialize the bar line
    bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
    
    // Initialize storage if needed
    if (use_storage_ || store_after_load_) {
        storage_ = std::make_shared<storage::FileStorage>();
        if (!storage_->initialize(storage_path_)) {
            logger_->error("Failed to initialize storage at path: {}", storage_path_);
            return false;
        }
    }
    
    return true;
}

bool CsvDataFeed::start_request_data() {
    // If we're using storage, try to load from storage first
    if (use_storage_ && storage_) {
        if (load_from_storage()) {
            data_ready_ = true;
            return true;
        }
        
        logger_->info("Data not found in storage, falling back to CSV file");
    }
    
    // Load from CSV
    if (load_from_csv()) {
        data_ready_ = true;
        
        // If requested, save the loaded data to storage
        if (store_after_load_ && storage_) {
            save_to_storage();
        }
        
        return true;
    }
    
    return false;
}

bool CsvDataFeed::terminate_request_data() {
    // Nothing to clean up for this feed type
    return true;
}

bool CsvDataFeed::is_data_ready() {
    return data_ready_;
}

std::optional<BarStruct> CsvDataFeed::next() {
    if (!bar_line_) {
        return std::nullopt;
    }
    
    return bar_line_->next();
}

bool CsvDataFeed::load_from_csv() {
    std::ifstream file(csv_file_path_);
    if (!file) {
        logger_->error("Failed to open CSV file: {}", csv_file_path_);
        return false;
    }
    
    std::string line;
    int line_num = 0;
    int valid_rows = 0;
    
    // Skip header if needed
    if (has_header_) {
        std::getline(file, line);
        line_num++;
    }
    
    while (std::getline(file, line)) {
        line_num++;
        
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        
        // Split line by delimiter
        while (std::getline(ss, token, delimiter_)) {
            tokens.push_back(token);
        }
        
        // Check if we have enough tokens
        int max_column = std::max({date_time_column_, open_column_, high_column_, 
                                  low_column_, close_column_, volume_column_});
        if (static_cast<int>(tokens.size()) <= max_column) {
            logger_->warn("Line {} has insufficient columns ({}), skipping", line_num, tokens.size());
            continue;
        }
        
        // Parse the data
        try {
            uint64_t time_ns;
            if (!parse_date_time(tokens[date_time_column_], time_ns)) {
                logger_->warn("Failed to parse date-time at line {}: {}", line_num, tokens[date_time_column_]);
                continue;
            }
            
            double open = std::stod(tokens[open_column_]);
            double high = std::stod(tokens[high_column_]);
            double low = std::stod(tokens[low_column_]);
            double close = std::stod(tokens[close_column_]);
            Decimal volume = std::stod(tokens[volume_column_]);
            
            if (bar_line_->push_data(time_ns, open, high, low, close, volume, 0, 1)) {
                valid_rows++;
            } else {
                logger_->warn("Failed to push data at line {}", line_num);
            }
        } catch (const std::exception& e) {
            logger_->warn("Error parsing line {}: {}", line_num, e.what());
            continue;
        }
    }
    
    logger_->info("Loaded {} valid rows from CSV file: {}", valid_rows, csv_file_path_);
    return valid_rows > 0;
}

bool CsvDataFeed::load_from_storage() {
    if (!storage_) {
        return false;
    }
    
    if (!storage_->has_data(symbol_, bar_type_, bar_size_)) {
        logger_->info("No data found in storage for symbol: {} bar_type: {} bar_size: {}", 
                    symbol_, static_cast<int>(bar_type_), bar_size_);
        return false;
    }
    
    auto bars_opt = storage_->load_bars(symbol_, bar_type_, bar_size_);
    if (!bars_opt.has_value()) {
        logger_->error("Failed to load data from storage for symbol: {}", symbol_);
        return false;
    }
    
    const auto& bars = bars_opt.value();
    size_t count = bars.start_time.size();
    
    if (count == 0) {
        logger_->warn("Loaded empty bar series from storage for symbol: {}", symbol_);
        return false;
    }
    
    // Push data into bar_line
    for (size_t i = 0; i < count; i++) {
        if (!bar_line_->push_data(bars.start_time[i], 
                                 bars.open[i], 
                                 bars.high[i], 
                                 bars.low[i], 
                                 bars.close[i], 
                                 bars.volume[i], 
                                 bars.wap[i], 
                                 bars.count[i])) {
            logger_->warn("Failed to push data at index {}", i);
        }
    }
    
    logger_->info("Loaded {} bars from storage for symbol: {}", count, symbol_);
    return true;
}

bool CsvDataFeed::save_to_storage() {
    if (!storage_) {
        return false;
    }
    
    // Create a bar series from the current bar line data
    // This is a bit hacky since we don't have direct access to the internal data
    // In a real implementation, we would make the bar series directly accessible
    
    BarSeries bars;
    auto current_bar = bar_line_->next();
    while (current_bar.has_value()) {
        bars.start_time.push_back(current_bar->time);
        bars.open.push_back(current_bar->open);
        bars.high.push_back(current_bar->high);
        bars.low.push_back(current_bar->low);
        bars.close.push_back(current_bar->close);
        bars.volume.push_back(current_bar->volume);
        bars.wap.push_back(current_bar->swap);
        bars.count.push_back(current_bar->count);
        
        current_bar = bar_line_->next();
    }
    
    if (bars.start_time.empty()) {
        logger_->warn("No data to save to storage for symbol: {}", symbol_);
        return false;
    }
    
    if (!storage_->store_bars(symbol_, bar_type_, bar_size_, bars)) {
        logger_->error("Failed to save data to storage for symbol: {}", symbol_);
        return false;
    }
    
    logger_->info("Saved {} bars to storage for symbol: {}", bars.start_time.size(), symbol_);
    return true;
}

bool CsvDataFeed::parse_date_time(const std::string& date_time_str, uint64_t& time_ns) {
    std::tm tm = {};
    std::istringstream ss(date_time_str);
    ss >> std::get_time(&tm, date_format_.c_str());
    
    if (ss.fail()) {
        return false;
    }
    
    // Convert to time_t
    std::time_t time = std::mktime(&tm);
    if (time == -1) {
        return false;
    }
    
    // Convert to nanoseconds
    time_ns = static_cast<uint64_t>(time) * 1000000000;
    return true;
}

} // namespace feed
} // namespace data
} // namespace quanttrader