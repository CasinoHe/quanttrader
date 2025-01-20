#include "back_test_service.h"
#include "tws_service.h"
#include "broker/requests.h"
#include "ta_libc.h"

namespace quanttrader {
namespace service {

BackTestService::BackTestService(const std::string_view config_path) : ServiceBase<BackTestService>("back_test_service") {
    logger_ = quanttrader::log::get_common_rotation_logger("BackTestService", "service", false);

    if (!set_config_path(config_path)) {
        logger_->error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
    } else {
        logger_->info("BackTestService instance created with config file: {}", config_path);
    }
}

bool BackTestService::prepare() {
    bool result = ServiceBase<BackTestService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // Load additional configuration specific to back testing
    std::string historical_data_path = get_string_value("historical_data_path");
    if (historical_data_path.empty()) {
        logger_->error("Cannot find the historical data path in the configuration file.");
        return false;
    }

    // Initialize TwsService
    std::string tws_config = get_string_value("tws_config");
    if (tws_config == "this") {
        tws_config = get_config_path();
    }

    tws_service_ = quanttrader::service::ServiceFactory::get_service<quanttrader::service::TwsService>(tws_config);
    if (!tws_service_->prepare()) {
        logger_->error("Cannot prepare the TwsService. Please check the service.log file for more information.");
        return false;
    }

    response_queue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>>();
    tws_service_->set_response_queue(response_queue_);

    logger_->info("BackTestService prepared with historical data path: {}", historical_data_path);
    return true;
}

void BackTestService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    logger_->info("BackTestService is running...");

    // Request historical data from TwsService
    auto request_id = quanttrader::broker::TwsClient::next_request_id();
    quanttrader::broker::Contract contract;
    contract.symbol = "AAPL";
    contract.secType = "STK";
    contract.currency = "USD";
    contract.exchange = "SMART";

    tws_service_->push_request(std::make_shared<quanttrader::broker::ReqHistoricalData>(request_id, contract, "20220101 00:00:00", "1 M", "1 day", "TRADES", 1));

    // Process responses
    std::shared_ptr<broker::ResponseHeader> response;
    while (response_queue_->wait_dequeue_timed(response, std::chrono::milliseconds(100))) {
        if (response->response_type == broker::ResponseType::HISTORICAL_DATA) {
            auto historical_data = std::dynamic_pointer_cast<broker::ResHistoricalData>(response);
            logger_->info("Received historical data: Date: {}, Open: {}, High: {}, Low: {}, Close: {}, Volume: {}",
                          historical_data->bar.time, historical_data->bar.open, historical_data->bar.high,
                          historical_data->bar.low, historical_data->bar.close, historical_data->bar.volume);

            // Use ta-lib indicators for back testing logic
            TA_Initialize();
            TA_Real close_prices[historical_data->bars.size()];
            for (size_t i = 0; i < historical_data->bars.size(); ++i) {
                close_prices[i] = historical_data->bars[i].close;
            }

            TA_Integer outBeg, outNbElement;
            TA_Real outRSI[historical_data->bars.size()];
            TA_RetCode retCode = TA_RSI(0, historical_data->bars.size() - 1, close_prices, 14, &outBeg, &outNbElement, outRSI);

            if (retCode == TA_SUCCESS) {
                for (int i = 0; i < outNbElement; ++i) {
                    logger_->info("RSI: {}", outRSI[i]);
                }
            } else {
                logger_->error("Error calculating RSI: {}", retCode);
            }

            TA_Shutdown();
        }
    }

    logger_->info("BackTestService has completed running.");
}

void BackTestService::stop() {
    logger_->info("BackTestService is stopping...");
    // Implement any cleanup or stopping logic here
    logger_->info("BackTestService has stopped.");
}

}
}
