#include "data_provider.h"
#include "service/tws_service.h"
#include "service/service_factory.h"
#include "broker/requests.h"
#include "data_consts.h"
#include "time/time_with_zone.h"
#include <chrono>
#include <cmath>
#include <array>

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
    keep_up_to_date_ = get_data_by_prefix<bool>(KEEP_UP_TO_DATE_NAME, false);

    std::string data_type = get_data_by_prefix<std::string>(DATA_TYPE_NAME);
    if (data_type == "historical") {
        is_historical_ = true;
        start_date_ = get_data_by_prefix<std::string>(DATA_START_DATE_NAME);
        end_date_ = get_data_by_prefix<std::string>(DATA_END_DATE_NAME);
        bar_type_ = get_data_by_prefix<std::string>(BAR_TYPE_NAME);

        // get bar type and bar size
        auto bar_type_size = get_bar_type_from_string(bar_type_);
        if (bar_type_size.first == BarType::NONE) {
            logger_->error("Cannot parse the bar type: {}", bar_type_);
            return false;
        } else {
            bar_line_ = std::make_shared<BarLine>(0, bar_type_size.first, bar_type_size.second);
        }
    } else if (data_type == "realtime") {
        is_realtime_ = true;
    } else {

    }

    return true;
}

bool DataProvider::terminate_request_data() {
    if (is_historical_) {
        if (keep_up_to_date_) {
            // cancel the keep up to date
            auto request = std::make_shared<qbroker::ReqCancelHistoricalData>();
            request->request_id = request_id_;
            auto cancel_id = broker_service_->push_request(request, std::nullopt);
            logger_->info("Cancel up-to-date historical data request {}, cancel id {}", request_id_, cancel_id);
        } else {
            // cancel the historical data
            if (!is_historical_completed()) {
                auto request = std::make_shared<qbroker::ReqCancelHistoricalData>();
                request->request_id = request_id_;
                auto cancel_id = broker_service_->push_request(request, std::nullopt);
                logger_->info("Cancel historical data request {}, cancel id {}", request_id_, cancel_id);
            }
        }
    }

    if (is_realtime_) {
        // cancel the real time data
        auto request = std::make_shared<qbroker::ReqCancelRealtimeMktData>();
        request->request_id = request_id_;
        auto cancel_id = broker_service_->push_request(request, std::nullopt);
        logger_->info("Cancel real time data request {}, cancel id {}", request_id_, cancel_id);
    }

    return true;
}

bool DataProvider::start_request_data() {
    if (is_historical_) {
        request_id_ = fetch_historical_data();
    }

    if (is_realtime_) {
        request_id_ = subscribe_realtime_data();
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
    historical_fetch_completed_.store(false);

    auto request = std::make_shared<qbroker::ReqHistoricalData>();
    request->symbol = tick_name_;
    request->currency = currency_;
    request->exchange = exchange_;
    request->security_type = security_type_;
    request->bar_size = bar_type_;
    request->what_to_show = what_type_;
    request->use_rth = use_rth_;
    request->keep_up_to_date = keep_up_to_date_;
    auto duration = get_duration();
    if (!duration.has_value()) {
        logger_->error("Cannot get the duration for the historical data.");
        return -1;
    }
    request->duration = duration.value();

    logger_->info("Request historical data for: {} security {} bar size {} rth {} duration {} up to date {}", 
                tick_name_,
                security_type_,
                bar_type_,
                use_rth_,
                request->duration,
                request->keep_up_to_date
                );
    auto callback = [this](std::shared_ptr<broker::ResponseHeader> response) {
        // process the historical data
        historical_data_response(std::dynamic_pointer_cast<qbroker::ResHistoricalData>(response));
    };

    return broker_service_->push_request(std::dynamic_pointer_cast<qbroker::RequestHeader>(request), callback);
}

std::pair<BarType, unsigned int> DataProvider::get_bar_type_from_string(const std::string &bar_type) {
    auto pos = bar_type.find(" ");
    int size = 0;

    BarType bar_type_enum = BarType::NONE;
    if (pos == std::string::npos) {
        return std::make_pair(BarType::NONE, 0);
    } else {
        size = std::stoi(bar_type.substr(0, pos));
    }

    if (std::find(kSecondsBarType, kSecondsBarType + sizeof(kSecondsBarType) / sizeof(kSecondsBarType[0]), bar_type) != kSecondsBarType + sizeof(kSecondsBarType) / sizeof(kSecondsBarType[0])) {
        bar_type_enum = BarType::Second;
    } else if (std::find(kMinutesBarType, kMinutesBarType + sizeof(kMinutesBarType) / sizeof(kMinutesBarType[0]), bar_type) != kMinutesBarType + sizeof(kMinutesBarType) / sizeof(kMinutesBarType[0])) {
        bar_type_enum = BarType::Minute;
    } else if (std::find(kHoursBarType, kHoursBarType + sizeof(kHoursBarType) / sizeof(kHoursBarType[0]), bar_type) != kHoursBarType + sizeof(kHoursBarType) / sizeof(kHoursBarType[0])) {
        bar_type_enum = BarType::Hour;
    } else if (std::find(kDaysBarType, kDaysBarType + sizeof(kDaysBarType) / sizeof(kDaysBarType[0]), bar_type) != kDaysBarType + sizeof(kDaysBarType) / sizeof(kDaysBarType[0])) {
        bar_type_enum = BarType::Day;
    } else if (std::find(kWeekBarType, kWeekBarType + sizeof(kWeekBarType) / sizeof(kWeekBarType[0]), bar_type) != kWeekBarType + sizeof(kWeekBarType) / sizeof(kWeekBarType[0])) {
        bar_type_enum = BarType::Week;
    } else if (std::find(kMonthBarType, kMonthBarType + sizeof(kMonthBarType) / sizeof(kMonthBarType[0]), bar_type) != kMonthBarType + sizeof(kMonthBarType) / sizeof(kMonthBarType[0])) {
        bar_type_enum = BarType::Month;
    }
    
    return std::make_pair(bar_type_enum, size);
}

void DataProvider::historical_data_response(std::shared_ptr<broker::ResHistoricalData> response) {
    if (response->is_end) {
        historical_fetch_completed_.store(true);
        logger_->info("Historical data response for: {} is completed. start_date {}, end_date {}", tick_name_, response->start_date, response->end_date);
        return;
    }

    uint64_t start_time = 0;
    // push historical bar data into the bar line
    try {
        auto time_str = std::get<std::string>(response->date);
        auto time_with_zone = qtime::TimeWithZone::from_ibapi_string(time_str, timezone_);
        start_time = time_with_zone.value().get_nano_epoch();
    } catch (std::bad_variant_access) {
        auto time = std::get<int>(response->date);
        start_time = time * kSecondsToNano;
    } catch (std::exception &e) {
        logger_->error("Cannot parse the Historical data: {}", e.what());
        return;
    }

    if (bar_line_->push_data(start_time, response->open, response->high, response->low, response->close, response->volume, response->wap, response->count)) {
        historical_data_length_++;
    }
}

void DataProvider::realtime_data_response(std::shared_ptr<broker::ResRealtimeData> response) {
    // process the real time data
    logger_->info("Realtime data response for: {}", tick_name_);
}

bool DataProvider::is_data_ready() {
    if (is_historical_) {
        return is_historical_completed();
    }

    if (is_realtime_) {
        return true;
    }

    return false;
}

// fetch data interface
std::optional<BarStruct> DataProvider::next() {
    if (bar_line_ == nullptr) {
        return std::nullopt;
    }

    return bar_line_->next();
}

}
}