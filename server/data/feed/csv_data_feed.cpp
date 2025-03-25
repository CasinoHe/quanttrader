#include "csv_data_feed.h"
#include "data/storage/storage_factory.h"
#include "time/time_with_zone.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <ctime>

namespace fs = std::filesystem;
namespace qtime = quanttrader::time;

namespace quanttrader {
namespace data {
namespace feed {

CsvDataFeed::CsvDataFeed(const std::string_view &data_prefix, provider::DataParamsType params)
    : provider::DataProvider(data_prefix, params) {
    // Constructor implementation
}

bool CsvDataFeed::prepare_data() {
    // Extract configuration from parameters
    csv_file_path_ = get_data_by_prefix<std::string>("_csv_file_path");
    if (csv_file_path_.empty()) {
        logger_->error("CSV file path is missing for {}", get_data_prefix());
        return false;
    }
    
    // Set the symbol name
    symbol_ = get_data_by_prefix<std::string>("_symbol");
    if (symbol_.empty()) {
        // Try to extract symbol from filename
        fs::path file_path(csv_file_path_);
        symbol_ = file_path.stem().string();
        logger_->info("Using filename as symbol: {}", symbol_);
    }
    
    // Parse bar type and size
    std::string bar_type_str = get_data_by_prefix<std::string>("_bar_type", "1 day");
    auto [type, size] = get_bar_type_from_string(bar_type_str);
    if (type == BarType::NONE) {
        logger_->error("Invalid bar type: {}", bar_type_str);
        return false;
    }
    bar_type_ = type;
    bar_size_ = size;
    
    // Configure CSV parsing options
    delimiter_ = get_data_by_prefix<char>("_delimiter", ',');
    date_time_column_ = get_data_by_prefix<int>("_datetime_column", 0);
    open_column_ = get_data_by_prefix<int>("_open_column", 1);
    high_column_ = get_data_by_prefix<int>("_high_column", 2);
    low_column_ = get_data_by_prefix<int>("_low_column", 3);
    close_column_ = get_data_by_prefix<int>("_close_column", 4);
    volume_column_ = get_data_by_prefix<int>("_volume_column", 5);
    has_header_ = get_data_by_prefix<bool>("_has_header", true);
    date_format_ = get_data_by_prefix<std::string>("_date_format", "%Y-%m-%d");
    
    // Configure storage options
    use_storage_ = get_data_by_prefix<bool>("_use_storage", false);
    store_after_load_ = get_data_by_prefix<bool>("_store_after_load", false);
    storage_path_ = get_data_by_prefix<std::string>("_storage_path", "./data");
    
    // Get storage type from configuration or use default
    std::string storage_type = get_data_by_prefix<std::string>("_storage_type", "file");
    
    // Initialize the bar line
    bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
    
    // Initialize storage if needed using the factory
    if (use_storage_ || store_after_load_) {
        storage_ = storage::StorageFactory::create(storage_type);
        if (!storage_->initialize(storage_path_)) {
            logger_->error("Failed to initialize {} storage at path: {}", storage_type, storage_path_);
            return false;
        }
    }
    
    return true;
}

bool CsvDataFeed::start_request_data() {
    // Try to load from storage first if enabled
    if (use_storage_ && storage_) {
        if (storage_->has_data(symbol_, bar_type_, bar_size_)) {
            logger_->info("Loading {} data for {} from storage", 
                get_bar_type_string(bar_type_, bar_size_), symbol_);
            if (load_from_storage()) {
                data_ready_ = true;
                return true;
            } else {
                logger_->warn("Failed to load data from storage, falling back to CSV");
            }
        }
    }
    
    // Load from CSV file
    if (load_from_csv()) {
        data_ready_ = true;
        
        // Store the data if requested
        if (store_after_load_ && storage_) {
            save_to_storage();
        }
        
        return true;
    }
    
    return false;
}

bool CsvDataFeed::terminate_request_data() {
    // Nothing to do for CSV data feed
    return true;
}

bool CsvDataFeed::is_data_ready() {
    return data_ready_;
}

std::optional<BarStruct> CsvDataFeed::next() {
    if (!data_ready_ || !bar_line_) {
        return std::nullopt;
    }
    
    auto bar_opt = bar_line_->next();
    if (bar_opt.has_value()) {
        // Handle replay modes
        BarStruct current_bar = bar_opt.value();
        
        // For realtime mode, wait appropriately
        wait_for_realtime(current_bar, last_bar_);
        
        // For stepped mode, wait for step signal
        wait_for_step();
        
        // Update last bar for next time
        last_bar_ = current_bar;
    }
    
    return bar_opt;
}

bool CsvDataFeed::load_from_csv() {
    std::ifstream file(csv_file_path_);
    if (!file.is_open()) {
        logger_->error("Failed to open CSV file: {}", csv_file_path_);
        return false;
    }
    
    std::string line;
    int line_count = 0;
    int data_count = 0;
    
    // Skip header if needed
    if (has_header_ && std::getline(file, line)) {
        line_count++;
    }
    
    // Process each line
    while (std::getline(file, line)) {
        line_count++;
        
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        
        // Split the line by delimiter
        while (std::getline(ss, token, delimiter_)) {
            tokens.push_back(token);
        }
        
        // Check if we have enough columns
        size_t max_col = std::max({date_time_column_, open_column_, high_column_, 
                                 low_column_, close_column_, volume_column_});
        if (tokens.size() <= max_col) {
            logger_->warn("Line {} has insufficient columns (found {}, need {})", 
                        line_count, tokens.size(), max_col + 1);
            continue;
        }
        
        // Parse the values
        uint64_t time_ns;
        if (!parse_date_time(tokens[date_time_column_], time_ns)) {
            logger_->warn("Failed to parse date/time on line {}: {}", 
                        line_count, tokens[date_time_column_]);
            continue;
        }
        
        try {
            double open = std::stod(tokens[open_column_]);
            double high = std::stod(tokens[high_column_]);
            double low = std::stod(tokens[low_column_]);
            double close = std::stod(tokens[close_column_]);
            
            // Volume might be empty or invalid in some datasets
            Decimal volume(0);
            if (volume_column_ < tokens.size() && !tokens[volume_column_].empty()) {
                try {
                    volume = Decimal(tokens[volume_column_]);
                } catch (...) {
                    // Use default volume of 0
                }
            }
            
            // Add the bar to our bar line
            if (bar_line_->push_data(time_ns, open, high, low, close, volume, Decimal(0), 0)) {
                data_count++;
            }
        }
        catch (const std::exception& e) {
            logger_->warn("Error parsing values on line {}: {}", line_count, e.what());
            continue;
        }
    }
    
    logger_->info("Loaded {} data points from CSV file {} (processed {} lines)", 
                data_count, csv_file_path_, line_count);
    
    // Rewind to the beginning for subsequent access
    if (bar_line_) {
        bar_line_->reset();
    }
    
    return data_count > 0;
}

bool CsvDataFeed::load_from_storage() {
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
    
    // Rewind to the beginning for subsequent access
    if (bar_line_) {
        bar_line_->reset();
    }
    
    return count > 0;
}

bool CsvDataFeed::save_to_storage() {
    if (!storage_ || !bar_line_) {
        return false;
    }
    
    // Save current position
    auto current_pos = bar_line_->get_position();
    
    // Rewind to beginning
    bar_line_->reset();
    
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
    
    // Restore position
    bar_line_->set_position(current_pos);
    
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

bool CsvDataFeed::parse_date_time(const std::string& date_time_str, uint64_t& time_ns) {
    std::tm tm = {};
    std::istringstream ss(date_time_str);
    
    ss >> std::get_time(&tm, date_format_.c_str());
    if (ss.fail()) {
        return false;
    }
    
    // Convert to time_t (seconds since epoch)
    std::time_t time_sec = std::mktime(&tm);
    if (time_sec == -1) {
        return false;
    }
    
    // Convert to nanoseconds
    time_ns = static_cast<uint64_t>(time_sec) * 1000000000ULL;
    return true;
}

} // namespace feed
} // namespace data
} // namespace quanttrader