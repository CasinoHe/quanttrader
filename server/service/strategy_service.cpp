#include "service/strategy_service.h"
#include "broker/twsclient.h"

namespace quanttrader {
namespace service {

StrategyService::StrategyService(const std::string_view config_path) {
    logger_ = quanttrader::log::get_common_rotation_logger("StrategyService", "service", false);

    if (!set_config_path(config_path)) {
        logger_->error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
    } else {
        logger_->info("StrategyService instance created with config file: {}", config_path);
    }
}

bool StrategyService::is_service_prepared() const {
    bool result = ServiceBase<StrategyService>::is_service_prepared();
    if (!result) {
        return false;
    }

    if (!client_) {
        return false;
    }

    return true;
}

void StrategyService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    client_->connect();
}

bool StrategyService::prepare() {
    bool result = ServiceBase<StrategyService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // Get config information from config file
    int port = get_int_value("port");
    std::string ip = get_string_value("host");
    int clientid = get_int_value("clientid");

    if (ip.empty() || port == 0 || clientid == 0) {
        logger_->error("Cannot get the configuration information from the config file. host {}, port {}, clientid {}", ip, port, clientid);
        return false;
    }

    client_ = std::make_shared<broker::TwsClient>(ip, port, clientid);
    return true;
}

}
}