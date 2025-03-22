#include "data/provider/data_provider_factory.h"
#include "data/feed/tws_data_feed.h"
#include "data/feed/csv_data_feed.h"
#include "data/feed/yahoo_data_feed.h"

namespace quanttrader {
namespace data {

/**
 * @brief Register all data feed implementations with the factory
 * 
 * This function must be called during application initialization to register
 * all data feed implementations with the factory.
 */
void register_data_feeds() {
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
}

} // namespace data
} // namespace quanttrader