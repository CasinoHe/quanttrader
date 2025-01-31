#include "data_provider.h"
#include "service/tws_service.h"
#include "service/service_factory.h"
#include "broker/requests.h"
#include "time/time_with_zone.h"
#include <chrono>
#include <cmath>

namespace quanttrader {
namespace data {

namespace qservice = quanttrader::service;
namespace qbroker = quanttrader::broker;
namespace qtime = quanttrader::time;

using ResponseCallBackType = std::function<void(std::shared_ptr<broker::ResponseHeader>)>;

DataProvider::DataProvider(const std::string_view &data_prefix, DataParamsType params) : data_prefix_(data_prefix), params_(params) {
    logger_ = quanttrader::log::get_common_rotation_logger("DataProvider", "data", false);
    broker_service_ = qservice::ServiceFactory::get_exist_service<qservice::TwsService>();
}

bool DataProvider::prepare_data() {
    // prepare data

    tick_name_ = get_data_by_prefix<std::string>(DATA_TICK_NAME);
    if (tick_name_.empty()) {
        logger_->error("Cannot find the tick name for the data provider: {}", data_prefix_);
        return false;
    }
    security_type_ = get_data_by_prefix<std::string>(DATA_SECURITY_TYPE_NAME, kDefaultSecurityType);
    exchange_ = get_data_by_prefix<std::string>(DATA_EXCHANGE_NAME, kDefaultExchange);
    currency_ = get_data_by_prefix<std::string>(DATA_CURRENCY_NAME, kDefaultCurrency);
    use_rth_ = get_data_by_prefix<bool>(DATA_USE_RTH_NAME, kDefaultUseRth);
    timezone_ = get_data_by_prefix<std::string>(DATA_TIMEZONE_NAME, kDefaultTimezone);
    what_type_ = get_data_by_prefix<std::string>(DATA_TRADE_WHAT_NAME, kDefaultWhatToShow);

    std::string data_type = get_data_by_prefix<std::string>(DATA_TYPE_NAME);
    if (data_type == "historical") {
        is_historical_ = true;
        start_date_ = get_data_by_prefix<std::string>(DATA_START_DATE_NAME);
        end_date_ = get_data_by_prefix<std::string>(DATA_END_DATE_NAME);
        bar_type_ = get_data_by_prefix<std::string>(BAR_TYPE_NAME);
    } else if (data_type == "realtime") {
        is_realtime_ = true;
    } else {

    }

    return true;
}

bool DataProvider::start_request_data() {
    if (is_realtime_) {
        subscribe_realtime_data();
    }

    if (is_historical_) {
        fetch_historical_data();
    }
    return true;
}

long DataProvider::subscribe_realtime_data() {
    auto request = std::make_shared<qbroker::ReqRealtimeMktData>();
    request->symbol = tick_name_;
    request->currency = currency_;
    request->exchange = exchange_;
    request->security_type = security_type_;

    auto callback = [this](std::shared_ptr<broker::ResponseHeader> response) {
        // process the real time data
        realtime_data_response(std::dynamic_pointer_cast<qbroker::ResRealtimeData>(response));
    };

    return broker_service_->push_request(request, callback);
}

std::optional<std::string> DataProvider::get_duration() {
    auto time1 = qtime::TimeWithZone::from_zone_string(start_date_);
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
        auto time2 = qtime::TimeWithZone::from_zone_string(end_date_);
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
        return std::to_string(static_cast<int>(std::ceil(static_cast<double>(duration) / 60))) + " M";
    } else {
        return std::to_string(duration) + " S";
    }

    return std::nullopt;
}

long DataProvider::fetch_historical_data() {
    auto request = std::make_shared<qbroker::ReqHistoricalData>();
    request->symbol = tick_name_;
    request->currency = currency_;
    request->exchange = exchange_;
    request->security_type = security_type_;
    request->bar_size = bar_type_;
    request->what_to_show = what_type_;
    request->use_rth = use_rth_;
    auto duration = get_duration();
    if (!duration.has_value()) {
        logger_->error("Cannot get the duration for the historical data.");
        return -1;
    }
    request->duration = duration.value();

    logger_->info("Request historical data for: {} security {} bar size {} rth {} duration {}", 
                tick_name_,
                security_type_,
                bar_type_,
                use_rth_,
                request->duration
                );
    auto callback = [this](std::shared_ptr<broker::ResponseHeader> response) {
        // process the historical data
        historical_data_response(std::dynamic_pointer_cast<qbroker::ResHistoricalData>(response));
    };

    return broker_service_->push_request(std::dynamic_pointer_cast<qbroker::RequestHeader>(request), callback);
}

void DataProvider::historical_data_response(std::shared_ptr<broker::ResHistoricalData> response) {
    // process the historical data
    logger_->info("Historical data response for: {}", tick_name_);
}

void DataProvider::realtime_data_response(std::shared_ptr<broker::ResRealtimeData> response) {
    // process the real time data
    logger_->info("Realtime data response for: {}", tick_name_);
}

}
}