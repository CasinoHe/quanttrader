#include "stock_trade_service.h"
#include "service/service_consts.h"
#include "broker/broker_provider_factory.h"
#include "cerebro/cerebro_factory.h"
#include "config/lua_config_loader.h"

#include <sstream>

namespace quanttrader {
namespace service {

namespace qlog = quanttrader::log;
namespace qconfig = quanttrader::luascript;

StockTradeService::StockTradeService(const std::string_view config_path) : ServiceBase<StockTradeService>(STOCK_TRADE_SERVICE_NAME) {
    logger_ = qlog::get_common_rotation_logger("StockTrade", "service");

    if (!set_config_path(config_path)) {
        logger_->error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
        qlog::Error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
    } else {
        logger_->info("instance created with config file: {}", config_path);
    }
}

bool StockTradeService::prepare() {
    auto result = ServiceBase<StockTradeService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        qlog::Error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // get and prepare broker provider
    auto broker_provider_name = get_string_value("broker_provider");
    auto broker_provider_config = get_string_value("broker_config");
    if (!broker_provider_name.empty()) {
        broker_provider_ = broker::BrokerProviderFactory::instance()->createProvider(broker_provider_name, broker_provider_config);
    } else {
        logger_->warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
        qlog::Warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
    }

    // prepare cerebro
    auto cerebro_names = get_string_value("cerebro_names");
    std::string cerebro_name;
    std::stringstream ss;

    while (std::getline(ss, cerebro_name, ',')) {
        if (cerebro_name.empty()) {
            continue;
        }
        auto cerebro_type = get_string_value(cerebro_name + ".cerebro_type");
        auto cerebro_config = get_string_value(cerebro_name + ".cerebro_config");
        if (cerebro_type.empty() || cerebro_config.empty()) {
            logger_->warn("Cerebro type or config is empty. Please check the {}.cerebro_names section of configuration file {}.", get_service_name(), get_config_path());
            qlog::Warn("Cerebro type or config is empty. Please check the {}.cerebro_names section of configuration file {}.", get_service_name(), get_config_path());
            continue;
        }

        auto config_loader = qconfig::LuaConfigLoader(cerebro_config);
        config_loader.load_config();
        auto params = std::make_shared<std::unordered_map<std::string, std::any>>();
        config_loader.get_all_values(cerebro_name, *params);
        auto cerebro = cerebro::CerebroFactory::instance()->create_cerebro(cerebro_type, cerebro_name, params);
        if (!cerebro) {
            logger_->error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            qlog::Error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            return false;
        }
        cerebro_ = cerebro;
    }

    // // prepare back test service
    // std::string back_test_config = get_string_value("back_test_config");
    // if (back_test_config == "this") {
    //     back_test_config = get_config_path();
    // }

    // // Use the factory to create the back test service
    // auto back_test_service = std::dynamic_pointer_cast<BackTestService>(factory.createService("back_test", back_test_config));
    // if (!back_test_service) {
    //     logger_->error("Failed to create the back test service.");
    //     return false;
    // }
    
    // back_test_service_ = back_test_service;
    // back_test_service->set_response_queue(response_queue_);
    // back_test_service->set_request_queue(request_queue_);

    // if (!back_test_service->prepare()) {
    //     logger_->error("Cannot prepare the back test service. Please check the service.log file for more information.");
    //     return false;
    // }

    // // stock trade service should prepare the runners and all strategies
    // if (!quanttrader::runner::prepare_stock_runners()) {
    //     logger_->error("Cannot prepare the stock runners.");
    //     return false;
    // }

    // if (!quanttrader::strategy::prepare_stategies()) {
    //     logger_->error("Cannot prepare the stock strategies.");
    //     return false;
    // }

    return true;
}

void StockTradeService::run() {
//     if (!is_service_prepared()) {
//         logger_->error("Service is not prepared. Please prepare the service first.");
//         return;
//     }

//     auto broker_service = std::static_pointer_cast<quanttrader::service::TwsService>(broker_service_);
//     auto back_test_service = std::static_pointer_cast<quanttrader::service::BackTestService>(back_test_service_);

//     std::thread broker_thread([&broker_service]() {
//         broker_service->run();
//     });

//     std::thread back_test_thread([&back_test_service]() {
//         back_test_service->run();
//     });

//     broker_thread.join();
//     back_test_thread.join();
// }

// void StockTradeService::stop() {
//     auto broker_service = std::static_pointer_cast<quanttrader::service::TwsService>(broker_service_);
//     auto back_test_service = std::static_pointer_cast<quanttrader::service::BackTestService>(back_test_service_);

//     if (broker_service) {
//         broker_service->stop();
//     }

//     if (back_test_service) {
//         back_test_service->stop();
//     }
//     logger_->info("Stop broker and back test service.");
// }

// bool StockTradeService::is_service_prepared() const {
//     if (!ServiceBase<StockTradeService>::is_service_prepared()) {
//         return false;
//     }

//     auto broker_service = std::static_pointer_cast<quanttrader::service::TwsService>(broker_service_);
//     if (!broker_service || !broker_service->is_service_prepared()) {
//         return false;
//     }

//     auto back_test_service = std::static_pointer_cast<quanttrader::service::BackTestService>(back_test_service_);
//     if (!back_test_service || !back_test_service->is_service_prepared()) {
//         return false;
//     }

//     return true;
}

}
}
