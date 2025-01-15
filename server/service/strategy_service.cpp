#include "service/strategy_service.h"
#include "broker/twsclient.h"

#include <chrono>


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

    // start tws thread
    tws_thread_ = std::make_shared<std::thread>(&StrategyService::run_tws, this);
    tws_thread_->join();
}

void StrategyService::run_tws() {
    // TEST
    int test_count = 30;

    auto connect_func = [this]() {
        while (true) {
            if (stop_flag_.load()) {
                break;
            }
            // try connecting to TWS infinitely
            if (!client_->connect()) {
                logger_->error("Connect to TWS failed. Retry in {} milliseconds.", retry_milliseconds_);
                std::this_thread::sleep_for(std::chrono::milliseconds(retry_milliseconds_));
            } else {
                break;
            }
        }
    };

    while (true) {
        // try connect to TWS
        connect_func();

        while (client_->is_connected()) {
            if (stop_flag_.load()) {
                break;
            }

            client_->process_messages();

            test_count -= 1;
            if (test_count <= 0) {
                stop();
            }
        }

        if (stop_flag_.load()) {
            break;
        }
    }
}

void StrategyService::stop() {
    stop_flag_.store(true);
    logger_->info("Set stop flag");
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
    int retry_milliseconds = get_int_value("retry_milliseconds");
    if (retry_milliseconds > 0) {
        retry_milliseconds_ = retry_milliseconds;
    } else {
        logger_->info("Retry milliseconds is less than 0, use the default retry milliseconds {}", retry_milliseconds_);
    }

    if (ip.empty() || port == 0 || clientid == 0) {
        logger_->error("Cannot get the configuration information from the config file. host {}, port {}, clientid {}", ip, port, clientid);
        return false;
    }

    if (tws_thread_) {
        logger_->warn("Tws thread is already running.");
        return false;
    }

    client_ = std::make_shared<broker::TwsClient>(ip, port, clientid);
    return true;
}

}
}