#include "back_test_service.h"
#include "tws_service.h"
#include "broker/requests.h"
#include "ta_libc.h"

namespace quanttrader {
namespace service {

BackTestService::BackTestService(const std::string_view config_path) : ServiceBase<BackTestService>("back_test_service") {
    logger_ = quanttrader::log::get_common_rotation_logger("BackTest", "service", false);

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

    logger_->info("BackTestService prepared with historical data path: {}", historical_data_path);
    return true;
}

void BackTestService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    logger_->info("Start BackTestService...");
    // start update config thread

    std::thread read_config_thread(&BackTestService::update_config, this);
    uint64_t wait_interval = get_int_value("wait_new_strategy_interval");
    if (wait_interval == 0) {
        wait_interval = 1000;
    }
    std::chrono::milliseconds wait_time(wait_interval);

    while (true) {
        if (stop_flag_.load()) {
            break;
        }

        // check if there is a back testing request
        for (auto &back_test : back_test_threads_) {
        }

        std::this_thread::sleep_for(wait_time);
    }

    if (read_config_thread.joinable()) {
        read_config_thread.join();
    }

    logger_->info("BackTestService has completed running.");
}

void BackTestService::stop() {
    stop_flag_.store(true);
    logger_->info("Set stop flag");
}

void BackTestService::run_back_test() {

}

void BackTestService::update_config() {

}

}
}