#include "tws_data_feed.h"
#include "broker/tws/tws_broker_adapter.h"
#include "broker/requests.h"
#include "time/time_with_zone.h"
#include <chrono>
#include <cmath>
#include <array>

namespace quanttrader {
namespace data {
namespace feed {

namespace qbroker = quanttrader::broker;
namespace qtime = quanttrader::time;

using ResponseCallBackType = std::function<void(std::shared_ptr<broker::ResponseHeader>)>;

TwsDataFeed::TwsDataFeed(const std::string_view &data_name, provider::DataParamsType params) 
    : provider::DataProvider(data_name, params) {
}

void TwsDataFeed::set_broker(std::shared_ptr<broker::BrokerProvider> broker_adapter) { 
    broker_adapter_ = std::dynamic_pointer_cast<broker::TwsBrokerAdapter>(broker_adapter); 
}

bool TwsDataFeed::prepare_data() {
    // Extract configuration parameters using the data prefix
    symbol_ = get_config_value<std::string>(DATA_TICK_NAME);
    if (symbol_.empty()) {
        logger_->error("Cannot find the tick name for the data provider: {}", data_name_);
        return false;
    }
    
    security_type_ = get_config_value<std::string>(DATA_SECURITY_TYPE_NAME, kDefaultSecurityType);
    exchange_ = get_config_value<std::string>(DATA_EXCHANGE_NAME, kDefaultExchange);
    currency_ = get_config_value<std::string>(DATA_CURRENCY_NAME, kDefaultCurrency);
    use_rth_ = get_config_value<bool>(DATA_USE_RTH_NAME, kDefaultUseRth);
    timezone_ = get_config_value<std::string>(DATA_TIMEZONE_NAME, kDefaultTimezone);
    what_type_ = get_config_value<std::string>(DATA_TRADE_WHAT_NAME, kDefaultWhatToShow);
    keep_up_to_date_ = get_config_value<bool>(KEEP_UP_TO_DATE_NAME, false);

    std::string data_type = get_config_value<std::string>(DATA_TYPE_NAME);
    if (data_type == "historical") {
        is_historical_ = true;
        start_date_ = get_config_value<std::string>(DATA_START_DATE_NAME);
        end_date_ = get_config_value<std::string>(DATA_END_DATE_NAME);
        bar_type_str_ = get_config_value<std::string>(BAR_TYPE_NAME);

        // get bar type and bar size
        auto bar_type_size = get_bar_type_from_string(bar_type_str_);
        if (bar_type_size.first == BarType::NONE) {
            logger_->error("Cannot parse the bar type: {}", bar_type_str_);
            return false;
        } else {
            bar_type_ = bar_type_size.first;
            bar_size_ = bar_type_size.second;
            bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
        }
    } else if (data_type == "realtime") {
        is_realtime_ = true;
        
        // For realtime data, we still need a bar type for data structure
        bar_type_str_ = get_config_value<std::string>(BAR_TYPE_NAME, "1 min");
        auto bar_type_size = get_bar_type_from_string(bar_type_str_);
        if (bar_type_size.first == BarType::NONE) {
            logger_->error("Cannot parse the bar type: {}", bar_type_str_);
            return false;
        } else {
            bar_type_ = bar_type_size.first;
            bar_size_ = bar_type_size.second;
            bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
        }
    } else {
        logger_->error("Unsupported data type: {}", data_type);
        return false;
    }

    // Check if broker adapter is available
    // if (!broker_adapter_ || !broker_adapter_->isConnected()) {
    //     logger_->error("TWS Broker adapter is not available or not connected");
    //     return false;
    // }

    return true;
}

bool TwsDataFeed::terminate_request_data() {
    if (!broker_adapter_) {
        logger_->error("TWS Broker adapter is not available");
        return false;
    }

    if (is_historical_) {
        if (keep_up_to_date_) {
            // Cancel the keep up to date historical data
            broker_adapter_->cancelHistoricalData(request_id_);
            logger_->info("Cancel up-to-date historical data request {}", request_id_);
        } else {
            // Cancel the historical data
            if (!is_historical_completed()) {
                broker_adapter_->cancelHistoricalData(request_id_);
                logger_->info("Cancel historical data request {}", request_id_);
            }
        }
    }

    if (is_realtime_) {
        // Cancel the real time data
        broker_adapter_->cancelRealTimeData(request_id_);
        logger_->info("Cancel real time data request {}", request_id_);
    }

    return true;
}

bool TwsDataFeed::start_request_data() {
    if (!broker_adapter_) {
        logger_->error("TWS Broker adapter is not available");
        return false;
    }

    if (is_historical_) {
        request_id_ = fetch_historical_data();
    }

    if (is_realtime_) {
        request_id_ = subscribe_realtime_data();
    }
    return request_id_ > 0;
}

long TwsDataFeed::subscribe_realtime_data() {
    if (!broker_adapter_) return -1;

    // First request realtime data to get the correct request ID
    long requestId = broker_adapter_->requestRealTimeData(
        symbol_,
        security_type_,
        exchange_,
        currency_
    );
    
    // Only register the callback if we got a valid request ID
    if (requestId > 0) {
        // Register a callback for real-time data using the returned request ID
        broker_adapter_->registerTradeCallback([this](const std::string& symbol, double price, Decimal size) {
            // Basic processing of real-time data
            logger_->info("Received real-time data for {}: price={}, size={}", symbol.empty() ? symbol_ : symbol, price, size);
            
            // In a real implementation, we'd update the bar_line_ with this data
            // For now, just mark data as ready
            data_ready_ = true;
        });
    } else {
        logger_->error("Failed to request realtime data for: {}", symbol_);
    }

    return requestId;
}

std::optional<std::string> TwsDataFeed::get_duration() {
    auto time1 = qtime::TimeWithZone::from_datetime_string(start_date_, timezone_);
    if (!time1.has_value()) {
        logger_->error("Cannot parse the start date: {}", start_date_);
        return {};
    }

    uint64_t duration = 0;
    if (end_date_ == "now") {
        auto time2 = qtime::TimeWithZone::from_now();
        if (!time2.has_value()) {
            logger_->error("Cannot get the current time.");
            return std::nullopt;
        }

        auto seconds1 = time1.value().get_seconds_epoch();
        auto seconds2 = time2.value().get_seconds_epoch();
        duration = seconds2 - seconds1;
    } else {
        auto time2 = qtime::TimeWithZone::from_datetime_string(end_date_, timezone_);
        if (!time2.has_value()) {
            logger_->error("Cannot parse the end date: {}", end_date_);
            return std::nullopt;
        }

        auto seconds1 = time1.value().get_seconds_epoch();
        auto seconds2 = time2.value().get_seconds_epoch();
        duration = seconds2 - seconds1;
    }

    if (duration < 0) {
        logger_->error("The duration is negative: {}", duration);
        return std::nullopt;
    } if (duration >= 31536000) {
        return std::to_string(static_cast<int>(std::ceil(static_cast<double>(duration) / 31536000))) + " Y";
    } else if (duration >= 2592000) {
        return std::to_string(static_cast<int>(std::ceil(static_cast<double>(duration) / 2592000))) + " M";
    } else if (duration >= 86400) {
        return std::to_string(static_cast<int>(std::ceil(static_cast<double>(duration) / 86400))) + " D";
    } else {
        return std::to_string(duration) + " S";
    }
}

long TwsDataFeed::fetch_historical_data() {
    if (!broker_adapter_) return -1;
    
    historical_fetch_completed_.store(false);

    auto duration = get_duration();
    if (!duration.has_value()) {
        logger_->error("Cannot get the duration for the historical data.");
        return -1;
    }
    
    // Request historical data first to get the correct request ID
    long requestId = broker_adapter_->requestHistoricalData(
        symbol_,
        security_type_,
        exchange_,
        currency_,
        end_date_ == "now" ? "" : end_date_,  // Empty string for current time
        duration.value(),
        bar_type_str_,
        what_type_,
        use_rth_,
        keep_up_to_date_
    );
    
    // Only register the callback if we got a valid request ID
    if (requestId > 0) {
        // Register a callback for historical data using the returned request ID
        broker_adapter_->registerBarDataCallback(requestId, [this](const broker::BarData& barData) {
            if (barData.is_last) {
                historical_fetch_completed_.store(true);
                data_ready_ = true;
                logger_->info("Historical data {} response for: {} is completed.", data_name_, symbol_);
                return;
            }

            // Push historical bar data into the bar line
            if (bar_line_->push_data(
                    barData.time,
                    barData.open,
                    barData.high,
                    barData.low,
                    barData.close,
                    barData.volume,
                    barData.wap,
                    barData.count)) {
                historical_data_length_++;
            }
        });

        logger_->info("Request historical data succeed: request id:{} symbol: {} security {} bar size {} rth {} duration {} up to date {}", 
                    requestId,
                    symbol_,
                    security_type_,
                    bar_type_str_,
                    use_rth_,
                    duration.value(),
                    keep_up_to_date_
                    );
    
    } else {
        logger_->error("Request historical data failed: request id:{} symbol: {} security {} bar size {} rth {} duration {} up to date {}", 
                    requestId,
                    symbol_,
                    security_type_,
                    bar_type_str_,
                    use_rth_,
                    duration.value(),
                    keep_up_to_date_
                    );
    }

    return requestId;
}

bool TwsDataFeed::is_data_ready() {
    if (is_historical_) {
        return is_historical_completed();
    }

    if (is_realtime_) {
        return data_ready_;
    }

    return false;
}

std::optional<BarStruct> TwsDataFeed::next() {
    if (!is_data_ready() || !bar_line_) {
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

} // namespace feed
} // namespace data
} // namespace quanttrader