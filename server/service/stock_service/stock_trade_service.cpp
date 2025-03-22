#include "stock_trade_service.h"
#include "service/service_consts.h"
#include "broker/broker_provider_factory.h"
#include "cerebro/cerebro_factory.h"
#include "config/lua_config_loader.h"

#include <sstream>
#include <vector>

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

        auto cerebro = cerebro::CerebroFactory::instance()->create_cerebro(cerebro_type, cerebro_name, cerebro_config);
        if (!cerebro) {
            logger_->error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            qlog::Error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            return false;
        }
        cerebro_map_[cerebro_name] = cerebro;
        cerebro->initialize();
    }

    return true;
}

void StockTradeService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    std::vector<std::thread> threads;
    for (auto& pair : cerebro_map_) {
        auto& cerebro = pair.second;
        if (cerebro) {
            threads.emplace_back([cerebro]() {
                cerebro->run();
            });
        } else {
            logger_->error("Cerebro {} is not initialized.", pair.first);
        }
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void StockTradeService::stop() {
    for (auto& pair : cerebro_map_) {
        auto& cerebro = pair.second;
        if (cerebro) {
            cerebro->stop();
        } else {
            logger_->error("Cerebro {} is not initialized.", pair.first);
        }
    }
    logger_->info("Stop broker and back test service.");
}

bool StockTradeService::is_service_prepared() const {
    if (!ServiceBase<StockTradeService>::is_service_prepared()) {
        return false;
    }

    for (auto& pair : cerebro_map_) {
        auto& cerebro = pair.second;
        if (!cerebro) {
            logger_->error("Cerebro {} is not initialized.", pair.first);
            return false;
        }
    }

    if (!broker_provider_) {
        logger_->error("Broker provider is not initialized.");
        return false;
    }

    return true;
}

}
}
