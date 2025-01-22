#include "back_test_service.h"
#include "tws_service.h"
#include "broker/requests.h"
#include "ta_libc.h"
#include "service/service_consts.h"
#include <boost/algorithm/string.hpp>

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
    // std::string historical_data_path = get_string_value("historical_data_path");
    // if (historical_data_path.empty()) {
    //     logger_->error("Cannot find the historical data path in the configuration file.");
    //     return false;
    // }

    // logger_->info("BackTestService prepared with historical data path: {}", historical_data_path);
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

    while (true) {
        if (stop_flag_.load()) {
            break;
        }

        // get the process keys from the back_test_process_ map
        std::vector<std::string> process_keys;
        {
            std::lock_guard<std::mutex> lock(back_test_process_mutex_);
            for (auto &process : back_test_process_) {
                process_keys.push_back(process.first);
            }
        }

        // check if there is a back testing request
        for (auto &key: process_keys) {
            std::lock_guard<std::mutex> lock(back_test_process_mutex_);
            auto it = back_test_process_.find(key);
            if (it == back_test_process_.end()) {
                continue;
            }

            auto process = it->second;
            if (!process) {
                // delete the process from the back_test_process_ map
                back_test_process_.erase(it);
                continue;
            }

            if (process->state == BackTestState::INIT) {
                if (process->expected_state == BackTestState::RUNNING) {
                    process->state = BackTestState::WAIT_STARTING;
                    process->process = std::make_shared<std::thread>(&BackTestService::run_back_test, this, process);
                }
            } else if (process->state == BackTestState::RUNNING) {
                if (process->expected_state == BackTestState::STOPPED) {
                    process->state = BackTestState::WAIT_STOPPING;
                    logger_->info("Start stopping back test process: {}", key);
                }
            } else if (process->state == BackTestState::STOPPED) {
                if (process->expected_state == BackTestState::STOPPED) {
                    process->process.reset();
                    it->second.reset();
                    back_test_process_.erase(it);
                    logger_->info("Back test process removed: {}", key);
                }
            }
        }

        std::this_thread::sleep_for(wait_timeout_);
    }

    if (read_config_thread.joinable()) {
        read_config_thread.join();
    }

    // There should be no process possessed back_test_process_mutex_ lock
    for (auto &process : back_test_process_) {
        if (process.second && process.second->process) {
            if (process.second->process->joinable()) {
                process.second->process->join();
            }
        }
    }

    logger_->info("BackTestService has completed running.");
}

void BackTestService::stop() {
    stop_flag_.store(true);
    logger_->info("Set stop flag");
}

void BackTestService::run_back_test(std::shared_ptr<BackTestServiceStruct> back_test) {
    // TODO: seperate the back test process to a dynamic library
    logger_->info("Start back test process: {}", back_test->config_key);

    while(true) {
        BackTestState state {BackTestState::INIT};
        BackTestState expected_state {BackTestState::INIT};
        {
            std::lock_guard<std::mutex> lock(back_test_process_mutex_);
            state = back_test->state;
            expected_state = back_test->expected_state;
        }

        if (expected_state == BackTestState::STOPPED && state == BackTestState::WAIT_STOPPING) {
            std::lock_guard<std::mutex> lock(back_test_process_mutex_);
            back_test->state = BackTestState::STOPPED;
            logger_->info("Stop back test process: {}", back_test->config_key);
            break;
        }

        std::this_thread::sleep_for(wait_timeout_);

        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, stop the back test process: {}", back_test->config_key);
            break;
        }
    }
}

void BackTestService::update_config() {
    auto update_time = std::chrono::system_clock::now();

    while(true) {
        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, exit the update config thread.");
            break;
        }

        auto cur = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(cur - update_time) > update_config_interval_) {
            update_time = cur;
            set_reload_config();
            if (!load_config()) {
                logger_->error("Cannot update the configuration file.");
                std::this_thread::sleep_for(update_config_interval_);
                continue;
            } else {
                // update the interval to reload the configuration file
                uint64_t wait_interval = get_int_value(UPDATE_CONFIG_INTERVAL_VARIABLE);
                if (wait_interval != 0) {
                    update_config_interval_ = std::chrono::milliseconds(wait_interval);
                }
                uint64_t wait_timeout = get_int_value(WAIT_TIMEOUT_VARIABLE);
                if (wait_timeout != 0) {
                    wait_timeout_ = std::chrono::milliseconds(wait_timeout);
                }

                // handle the data change in back test
                const auto stoped_process = handle_need_stop_process();
                handle_need_start_process(stoped_process);
                handle_need_restarting_process();

                // handle stop flag
                int stop_flag = get_int_value(STOP_FLAG_VARIABLE);
                if (stop_flag > 0) {
                    stop_flag_.store(true);
                }
                logger_->info("Reloaded config.");
            }
        }

        std::this_thread::sleep_for(wait_timeout_);
    }
}

std::vector<std::string> BackTestService::handle_need_stop_process() {
    // stop back test process in the column "stop_test" in the configuration file
    std::string need_stop_process = get_string_value(STOP_BACK_TEST_VARIABLE);
    if (need_stop_process.empty()) {
        return {};
    }

    // process key name is split by comma
    std::vector<std::string> process_keys {};
    boost::split(process_keys, need_stop_process, boost::is_any_of(SPLIT_BACK_TEST_VARIABLE));

    for (auto &key : process_keys) {
        // update process data

        // get the data, compare the version, if the version is the same, skip the process because it is already manipulated 
        int version = get_int_value_in_table(key, VERSION_VARIABLE);
        if (version <= 0) {
            logger_->warn("Cannot find the version for the process: {} version {}", key, version);
            continue;
        }

        // manipulating the back_test_process_ map should be protected by mutex
        std::lock_guard<std::mutex> lock(back_test_process_mutex_);
        
        // find the process in the back_test_process_ list
        auto it = back_test_process_.find(key);
        if (it != back_test_process_.end()) {
            // stop the process
            if (it->second) {
                // compare the version
                if (it->second->version != version) {
                    // tag the need stop process
                    it->second->expected_state = BackTestState::STOPPED;
                    it->second->version = version;
                    logger_->info("The process {} is tagged to stop, version {}.", key, version);
                }
            } else {
                logger_->warn("The process need to stop {} is not running.", key);
            }
        } else {
            logger_->warn("Cannot find the process which need to be stopped: {}", key);
        }
    }

    return process_keys;
}

void BackTestService::handle_need_start_process(const std::vector<std::string> &exclude_processes) {
    // start back test process in the column "new_test" in the configuration file
    std::string need_start_process = get_string_value(START_BACK_TEST_VARIABLE);
    if (need_start_process.empty()) {
        return;
    }

    // process key name is split by comma
    std::vector<std::string> process_keys;
    boost::split(process_keys, need_start_process, boost::is_any_of(","));

    for (const auto& key : process_keys) {
        // exclude the process in the exclude_processes list
        if (std::find(exclude_processes.begin(), exclude_processes.end(), key) != exclude_processes.end()) {
            continue;
        }

        // get the data, compare the version, if the version is the same, skip the process because it is already manipulated 
        int version = get_int_value_in_table(key, VERSION_VARIABLE);
        if (version <= 0) {
            logger_->warn("Cannot find the version for the process: {} version {}", key, version);
            continue;
        }

        // get config data before mutex lock
        std::string strategy_name = get_string_value_in_table(key, STRATEGY_NAME_VARIABLE);
        if (strategy_name.empty()) {
            logger_->warn("Cannot find the strategy name for the process: {}", key);
            continue;
        }
        auto strategy_data = std::make_shared<std::unordered_map<std::string, std::any>>();
        if (!get_all_values_in_table(key, *strategy_data)) {
            logger_->warn("Cannot find the strategy data for the process: {}", key);
            continue;
        }

        // manipulating the back_test_process_ map should be protected by mutex
        std::lock_guard<std::mutex> lock(back_test_process_mutex_);
        
        // find the process in the back_test_process_ list
        auto it = back_test_process_.find(key);
        if (it != back_test_process_.end()) {
            continue;
        }

        // create a new BackTestStruct
        auto back_test_struct = std::make_shared<BackTestServiceStruct>();
        back_test_struct->config_key = key;
        back_test_struct->version = version;
        back_test_struct->state = BackTestState::INIT;
        back_test_struct->expected_state = BackTestState::RUNNING;
        back_test_struct->strategy_name = std::move(strategy_name);
        back_test_struct->strategy_data = std::move(strategy_data);
        // add the new process to the back_test_process_ map
        back_test_process_[key] = back_test_struct;
        logger_->info("Prepare new back test process: {} with version {}.", key, version);
    }
}

void BackTestService::handle_need_restarting_process() {
    // restart back test process in the column "restart_test" in the configuration file
    std::string need_restart_process = get_string_value("restart_test");
}

void BackTestService::update_process_data(const std::string_view key) {
    // update the process data in the back_test_process_ list
    // TODO:
}

}
}
