#include "stock_trade_service.h"
#include "tws_service.h"
#include "back_test_service.h"
#include "service_factory.h"
#include "broker/requests.h"

namespace quanttrader {
namespace service {

StockTradeService::StockTradeService(const std::string_view config_path) : ServiceBase<StockTradeService>("stock_trade_service") {
    logger_ = quanttrader::log::get_common_rotation_logger("StockTradeService", "service", false);

    if (!set_config_path(config_path)) {
        logger_->error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
    } else {
        logger_->info("instance created with config file: {}", config_path);
    }
}

bool StockTradeService::prepare() {
    auto result = ServiceBase<StockTradeService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // prepare broker service first
    std::string broker_service_name = get_string_value("broker_service");
    std::string broker_config = get_string_value("broker_config");

    // only tws now
    if (broker_service_name != "tws") {
        logger_->error("Cannot find the broker service: {}", broker_service_name);
        return false;
    }

    if (broker_config == "this") {
        broker_config = get_config_path();
    }

    std::shared_ptr<quanttrader::service::TwsService> broker_service = quanttrader::service::ServiceFactory::get_service<quanttrader::service::TwsService>(broker_config);
    broker_service_ = std::static_pointer_cast<void>(broker_service);

    if (!broker_service->prepare()) {
        logger_->error("Cannot prepare the broker service. Please check the service.log file for more information.");
        return false;
    }

    // prepare back test service
    std::string back_test_config = get_string_value("back_test_config");
    if (back_test_config == "this") {
        back_test_config = get_config_path();
    }

    std::shared_ptr<quanttrader::service::BackTestService> back_test_service = quanttrader::service::ServiceFactory::get_service<quanttrader::service::BackTestService>(back_test_config);
    back_test_service_ = std::static_pointer_cast<void>(back_test_service);

    if (!back_test_service->prepare()) {
        logger_->error("Cannot prepare the back test service. Please check the service.log file for more information.");
        return false;
    }

    return true;
}

void StockTradeService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    response_queue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResponseHeader>>>();
    auto broker_service = std::static_pointer_cast<quanttrader::service::TwsService>(broker_service_);
    broker_service->set_response_queue(response_queue_);

    auto back_test_service = std::static_pointer_cast<quanttrader::service::BackTestService>(back_test_service_);

    std::thread broker_thread([&broker_service]() {
        broker_service->run();
    });

    std::thread back_test_thread([&back_test_service]() {
        back_test_service->run();
    });

    broker_thread.join();
    back_test_thread.join();
}

void StockTradeService::stop() {

}

bool StockTradeService::is_service_prepared() const {
    if (!ServiceBase<StockTradeService>::is_service_prepared()) {
        return false;
    }

    auto broker_service = std::static_pointer_cast<quanttrader::service::TwsService>(broker_service_);
    if (!broker_service || !broker_service->is_service_prepared()) {
        return false;
    }

    auto back_test_service = std::static_pointer_cast<quanttrader::service::BackTestService>(back_test_service_);
    if (!back_test_service || !back_test_service->is_service_prepared()) {
        return false;
    }

    return true;
}

}
}
