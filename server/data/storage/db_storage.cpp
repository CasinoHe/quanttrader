#include "db_storage.h"
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

namespace quanttrader {
namespace data {
namespace storage {

DbStorage::DbStorage() {
    logger_ = quanttrader::log::get_common_rotation_logger("DbStorage", "data");
}

DbStorage::~DbStorage() {
    if (db_handle_) {
        sqlite3_close(db_handle_);
        db_handle_ = nullptr;
    }
}

bool DbStorage::initialize(const std::string& storage_path) {
    // Create directory if it doesn't exist
    fs::path dir_path(storage_path);
    if (!fs::exists(dir_path)) {
        try {
            fs::create_directories(dir_path);
            logger_->info("Created storage directory: {}", storage_path);
        } catch (const std::exception& e) {
            logger_->error("Failed to create storage directory: {}, error: {}", storage_path, e.what());
            return false;
        }
    }
    
    // Build database path and connect to SQLite
    db_path_ = (dir_path / "market_data.db").string();
    int result = sqlite3_open(db_path_.c_str(), &db_handle_);
    if (result != SQLITE_OK) {
        logger_->error("Failed to open database: {}, error: {}", db_path_, sqlite3_errmsg(db_handle_));
        sqlite3_close(db_handle_);
        db_handle_ = nullptr;
        return false;
    }
    
    // Create necessary tables if they don't exist
    if (!create_tables_if_needed()) {
        sqlite3_close(db_handle_);
        db_handle_ = nullptr;
        return false;
    }
    
    logger_->info("Successfully initialized database storage at: {}", db_path_);
    return true;
}

bool DbStorage::create_tables_if_needed() {
    // Create symbols table
    if (!execute_query(create_symbols_table_)) {
        logger_->error("Failed to create symbols table");
        return false;
    }
    
    return true;
}

bool DbStorage::execute_query(const std::string& query) {
    char* error_msg = nullptr;
    int result = sqlite3_exec(db_handle_, query.c_str(), nullptr, nullptr, &error_msg);
    
    if (result != SQLITE_OK) {
        logger_->error("SQL error: {}, Query: {}", error_msg, query);
        sqlite3_free(error_msg);
        return false;
    }
    
    return true;
}

std::string DbStorage::get_bar_type_string(BarType bar_type, unsigned int bar_size) const {
    switch (bar_type) {
        case BarType::Tick:
            return "tick";
        case BarType::Second:
            return std::to_string(bar_size) + "sec";
        case BarType::Minute:
            return std::to_string(bar_size) + "min";
        case BarType::Hour:
            return std::to_string(bar_size) + "hour";
        case BarType::Day:
            return std::to_string(bar_size) + "day";
        case BarType::Week:
            return std::to_string(bar_size) + "week";
        case BarType::Month:
            return std::to_string(bar_size) + "month";
        case BarType::Year:
            return std::to_string(bar_size) + "year";
        default:
            return "unknown";
    }
}

std::string DbStorage::get_table_name(const std::string& symbol, BarType bar_type, unsigned int bar_size) const {
    // Create safe table name: symbol_bartype_barsize
    std::string safe_symbol = symbol;
    std::string bar_type_str = get_bar_type_string(bar_type, bar_size);
    
    // Replace any non-alphanumeric characters with underscore
    for (char& c : safe_symbol) {
        if (!std::isalnum(c)) {
            c = '_';
        }
    }
    
    return "data_" + safe_symbol + "_" + bar_type_str;
}

bool DbStorage::store_bars(const std::string& symbol, 
                          BarType bar_type, 
                          unsigned int bar_size,
                          const BarSeries& bars) {
    if (bars.start_time.empty()) {
        logger_->warn("Attempting to store empty bar series for symbol: {}", symbol);
        return false;
    }
    
    std::string table_name = get_table_name(symbol, bar_type, bar_size);
    std::string bar_type_str = get_bar_type_string(bar_type, bar_size);
    
    // Start a transaction for better performance
    if (!execute_query("BEGIN TRANSACTION;")) {
        return false;
    }
    
    // Create table for this data series if it doesn't exist
    std::stringstream create_table_ss;
    create_table_ss << "CREATE TABLE IF NOT EXISTS " << table_name << " ("
                    << "time INTEGER PRIMARY KEY, "
                    << "open REAL, "
                    << "high REAL, "
                    << "low REAL, "
                    << "close REAL, "
                    << "volume TEXT, "
                    << "wap TEXT, "
                    << "count INTEGER"
                    << ");";
    
    if (!execute_query(create_table_ss.str())) {
        execute_query("ROLLBACK;");
        return false;
    }
    
    // Add or update entry in symbols table
    uint64_t first_time = bars.start_time[0];
    uint64_t last_time = bars.start_time.back();
    size_t count = bars.start_time.size();
    
    std::stringstream upsert_ss;
    upsert_ss << "INSERT INTO " << symbols_table_ 
              << " (symbol, bar_type, bar_size, first_time, last_time, count) VALUES ("
              << "'" << symbol << "', "
              << "'" << bar_type_str << "', "
              << bar_size << ", "
              << first_time << ", "
              << last_time << ", "
              << count
              << ") ON CONFLICT(symbol, bar_type, bar_size) DO UPDATE SET "
              << "first_time = MIN(first_time, excluded.first_time), "
              << "last_time = MAX(last_time, excluded.last_time), "
              << "count = count + excluded.count;";
    
    if (!execute_query(upsert_ss.str())) {
        execute_query("ROLLBACK;");
        return false;
    }
    
    // Prepare statement for inserting data
    sqlite3_stmt* stmt = nullptr;
    std::stringstream insert_ss;
    insert_ss << "INSERT OR REPLACE INTO " << table_name 
              << " (time, open, high, low, close, volume, wap, count) "
              << "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    
    int result = sqlite3_prepare_v2(db_handle_, insert_ss.str().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        logger_->error("Failed to prepare statement: {}", sqlite3_errmsg(db_handle_));
        execute_query("ROLLBACK;");
        return false;
    }
    
    // Insert data rows
    for (size_t i = 0; i < count; ++i) {
        sqlite3_bind_int64(stmt, 1, bars.start_time[i]);
        sqlite3_bind_double(stmt, 2, bars.open[i]);
        sqlite3_bind_double(stmt, 3, bars.high[i]);
        sqlite3_bind_double(stmt, 4, bars.low[i]);
        sqlite3_bind_double(stmt, 5, bars.close[i]);
        
        // Convert Decimal to string for storage
        std::string volume_str = bars.volume[i].toString();
        std::string wap_str = bars.wap[i].toString();
        
        sqlite3_bind_text(stmt, 6, volume_str.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, wap_str.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 8, bars.count[i]);
        
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            logger_->error("Failed to insert row: {}", sqlite3_errmsg(db_handle_));
            sqlite3_finalize(stmt);
            execute_query("ROLLBACK;");
            return false;
        }
        
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    
    // Commit the transaction
    if (!execute_query("COMMIT;")) {
        execute_query("ROLLBACK;");
        return false;
    }
    
    logger_->info("Successfully stored {} bars for symbol: {} bar_type: {} bar_size: {}", 
                 count, symbol, bar_type_str, bar_size);
    return true;
}

std::optional<BarSeries> DbStorage::load_bars(const std::string& symbol,
                                             BarType bar_type,
                                             unsigned int bar_size,
                                             std::optional<uint64_t> start_time,
                                             std::optional<uint64_t> end_time) {
    std::string table_name = get_table_name(symbol, bar_type, bar_size);
    
    // Check if table exists by querying for its metadata
    std::stringstream check_ss;
    check_ss << "SELECT name FROM sqlite_master WHERE type='table' AND name='" << table_name << "';";
    
    sqlite3_stmt* check_stmt = nullptr;
    int result = sqlite3_prepare_v2(db_handle_, check_ss.str().c_str(), -1, &check_stmt, nullptr);
    if (result != SQLITE_OK) {
        logger_->error("Failed to prepare statement: {}", sqlite3_errmsg(db_handle_));
        return std::nullopt;
    }
    
    result = sqlite3_step(check_stmt);
    sqlite3_finalize(check_stmt);
    
    if (result != SQLITE_ROW) {
        logger_->warn("No data found for symbol: {} bar_type: {} bar_size: {}", 
                     symbol, get_bar_type_string(bar_type, bar_size), bar_size);
        return std::nullopt;
    }
    
    // Build query to fetch data with optional time filters
    std::stringstream query_ss;
    query_ss << "SELECT time, open, high, low, close, volume, wap, count FROM " << table_name;
    
    if (start_time.has_value() || end_time.has_value()) {
        query_ss << " WHERE ";
        
        if (start_time.has_value()) {
            query_ss << "time >= " << start_time.value();
            if (end_time.has_value()) {
                query_ss << " AND ";
            }
        }
        
        if (end_time.has_value()) {
            query_ss << "time <= " << end_time.value();
        }
    }
    
    query_ss << " ORDER BY time;";
    
    // Prepare and execute the query
    sqlite3_stmt* stmt = nullptr;
    result = sqlite3_prepare_v2(db_handle_, query_ss.str().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        logger_->error("Failed to prepare statement: {}", sqlite3_errmsg(db_handle_));
        return std::nullopt;
    }
    
    // Create bar series and load data
    BarSeries bars;
    
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Extract basic types
        uint64_t time = sqlite3_column_int64(stmt, 0);
        double open = sqlite3_column_double(stmt, 1);
        double high = sqlite3_column_double(stmt, 2);
        double low = sqlite3_column_double(stmt, 3);
        double close = sqlite3_column_double(stmt, 4);
        int count = sqlite3_column_int(stmt, 7);
        
        // Extract text fields and convert to Decimal
        const char* volume_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* wap_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        
        Decimal volume(volume_str ? volume_str : "0");
        Decimal wap(wap_str ? wap_str : "0");
        
        // Add data to the bar series
        bars.start_time.push_back(time);
        bars.open.push_back(open);
        bars.high.push_back(high);
        bars.low.push_back(low);
        bars.close.push_back(close);
        bars.volume.push_back(volume);
        bars.wap.push_back(wap);
        bars.count.push_back(count);
    }
    
    sqlite3_finalize(stmt);
    
    if (bars.start_time.empty()) {
        logger_->warn("No data found in the specified time range for symbol: {}", symbol);
        return std::nullopt;
    }
    
    logger_->info("Successfully loaded {} bars for symbol: {} bar_type: {} bar_size: {}", 
                 bars.start_time.size(), symbol, get_bar_type_string(bar_type, bar_size), bar_size);
    return bars;
}

bool DbStorage::has_data(const std::string& symbol, 
                        BarType bar_type, 
                        unsigned int bar_size) {
    std::string bar_type_str = get_bar_type_string(bar_type, bar_size);
    
    std::stringstream query_ss;
    query_ss << "SELECT count FROM " << symbols_table_
             << " WHERE symbol = '" << symbol << "'"
             << " AND bar_type = '" << bar_type_str << "'"
             << " AND bar_size = " << bar_size << ";";
    
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db_handle_, query_ss.str().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        logger_->error("Failed to prepare statement: {}", sqlite3_errmsg(db_handle_));
        return false;
    }
    
    result = sqlite3_step(stmt);
    bool has_data = (result == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0);
    
    sqlite3_finalize(stmt);
    return has_data;
}

std::vector<std::string> DbStorage::get_available_symbols() {
    std::vector<std::string> symbols;
    
    std::string query = "SELECT DISTINCT symbol FROM " + symbols_table_ + " ORDER BY symbol;";
    
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db_handle_, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        logger_->error("Failed to prepare statement: {}", sqlite3_errmsg(db_handle_));
        return symbols;
    }
    
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (symbol) {
            symbols.push_back(symbol);
        }
    }
    
    sqlite3_finalize(stmt);
    return symbols;
}

} // namespace storage
} // namespace data
} // namespace quanttrader