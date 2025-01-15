#include "service/strategy_service.h"
#include "broker/twsclient.h"

namespace quanttrader {
namespace service {

StrategyService::StrategyService(const std::string_view config_path) {
    set_config_path(config_path);
    logger_ = quanttrader::log::get_common_rotation_logger("StrategyService", "service", false);
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
        return false;
    }

    // Get config information from config file
    int port = get_int_value("port");
    std::string ip = get_string_value("host");
    int clientid = get_int_value("clientid");

    client_ = std::make_shared<broker::TwsClient>(ip, port, clientid);
    return true;
}

}
}