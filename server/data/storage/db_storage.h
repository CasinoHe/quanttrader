#pragma once

#include "data_storage.h"
#include "logger/quantlogger.h"
#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <sqlite3.h>

namespace quanttrader {
namespace data {
namespace storage {

/**
 * @brief SQLite database storage implementation
 * 
 * This class implements the DataStorage interface using SQLite database
 * to store and retrieve market data.
 */
class DbStorage : public DataStorage {
public:
    DbStorage();
    ~DbStorage() override;

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
    std::string get_table_name(const std::string& symbol, BarType bar_type, unsigned int bar_size) const;
    bool create_tables_if_needed();
    bool execute_query(const std::string& query);
    
    std::string db_path_;
    sqlite3* db_handle_ = nullptr;
    quanttrader::log::LoggerPtr logger_;

    // Table schema
    const std::string symbols_table_ = "symbols";
    const std::string create_symbols_table_ = 
        "CREATE TABLE IF NOT EXISTS symbols ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "symbol TEXT NOT NULL, "
        "bar_type TEXT NOT NULL, "
        "bar_size INTEGER NOT NULL, "
        "first_time INTEGER, "
        "last_time INTEGER, "
        "count INTEGER, "
        "UNIQUE(symbol, bar_type, bar_size))";
};

} // namespace storage
} // namespace data
} // namespace quanttrader