#include "data_provider.h"
#include "service/tws_service.h"
#include "service/service_factory.h"
#include "broker/requests.h"

namespace quanttrader {
namespace data {

namespace qservice = quanttrader::service;
namespace qbroker = quanttrader::broker;

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
    use_rth_ = static_cast<bool>(get_data_by_prefix<int>(DATA_USE_RTH_NAME, kDefaultUseRth));
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

long DataProvider::fetch_historical_data() {
    auto request = std::make_shared<qbroker::ReqHistoricalData>();
    request->symbol = tick_name_;
    request->currency = currency_;
    request->exchange = exchange_;
    request->security_type = security_type_;
    request->bar_size = bar_type_;
    request->what_to_show = what_type_;
    request->use_rth = use_rth_;
    request->format_date = true;
    request->duration = "120 D";

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