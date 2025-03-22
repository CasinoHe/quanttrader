#include "data/provider/data_provider_factory.h"
#include "data/feed/tws_data_feed.h"
#include "data/feed/csv_data_feed.h"
#include "data/feed/yahoo_data_feed.h"
#include "data/storage/file_storage.h"
#include "data/storage/db_storage.h"
#include <memory>

namespace quanttrader {
namespace data {

/**
 * @brief Create a storage provider based on the specified type
 * 
 * @param storage_type Type of storage ("file" or "db")
 * @return std::shared_ptr<storage::DataStorage> Storage provider instance
 */
std::shared_ptr<storage::DataStorage> create_storage(const std::string& storage_type) {
    if (storage_type == "file") {
        return std::make_shared<storage::FileStorage>();
    } else if (storage_type == "db") {
        return std::make_shared<storage::DbStorage>();
    } else {
        // Default to file storage
        return std::make_shared<storage::FileStorage>();
    }
}

/**
 * @brief Register all data feed implementations with the factory
 * 
 * This function must be called during application initialization to register
 * all data feed implementations with the factory.
 */
int register_data_feeds() {
    auto& factory = provider::DataProviderFactory::instance();
    
    // Register TWS data feed
    factory.register_provider("tws", [](const std::string_view& data_prefix, provider::DataParamsType params) {
        return std::make_shared<feed::TwsDataFeed>(data_prefix, params);
    });
    
    // Register CSV data feed
    factory.register_provider("csv", [](const std::string_view& data_prefix, provider::DataParamsType params) {
        return std::make_shared<feed::CsvDataFeed>(data_prefix, params);
    });
    
    // Register Yahoo Finance data feed
    factory.register_provider("yahoo", [](const std::string_view& data_prefix, provider::DataParamsType params) {
        return std::make_shared<feed::YahooDataFeed>(data_prefix, params);
    });

    return 0;
}

namespace {
    // Static variable to ensure that the data feeds are registered only once
    static int register_data_feeds_once = register_data_feeds();
}

} // namespace data
} // namespace quanttrader