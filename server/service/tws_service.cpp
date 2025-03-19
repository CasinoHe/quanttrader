#include "service/tws_service.h"
#include "broker/twsclient.h"
#include "broker/requests.h"
#include "service/service_consts.h"

#include <chrono>


namespace quanttrader {
namespace service {

TwsService::TwsService(const std::string_view config_path) : ServiceBase<TwsService>("tws_service") {
    logger_ = quanttrader::log::get_common_rotation_logger("Tws", "service");

    if (!set_config_path(config_path)) {
        logger_->error("Cannot set the configuration file path: {}, please check the existence of the file and the config file should be a regular lua file.", config_path);
    } else {
        logger_->info("TwsService instance created with config file: {}", config_path);
    }
}

bool TwsService::is_service_prepared() const {
    bool result = ServiceBase<TwsService>::is_service_prepared();
    if (!result) {
        return false;
    }

    if (!client_) {
        return false;
    }

    if (!request_queue_) {
        return false;
    }
    
    if (!response_queue_) {
        return false;
    }

    return true;
}

void TwsService::run() {
    if (!is_service_prepared()) {
        logger_->error("Service is not prepared. Please prepare the service first.");
        return;
    }

    // start tws thread
    client_thread_ = std::make_shared<std::thread>(&TwsService::run_tws, this);
    client_thread_->join();
}

void TwsService::run_request(std::atomic<int> &tws_version) {
    int last_version = tws_version.load();

    while (true) {
        if (tws_version.load() != last_version) {
            // exit the thread
            break;
        }

        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, stop the request thread.");
            break;
        }

        std::shared_ptr<broker::RequestHeader> request_ptr = nullptr;
        request_queue_->wait_dequeue_timed(request_ptr, wait_timeout_);

        if (request_ptr) {
            logger_->debug("Received request: {}", static_cast<int>(request_ptr->request_type));
            if (request_ptr->request_type == broker::RequestType::REQUEST_CURRENT_TIME) {
                // request current time
                client_->request_current_time();
            } else if (request_ptr->request_type == broker::RequestType::REQUEST_HISTORICAL_DATA) {
                auto Request = std::dynamic_pointer_cast<broker::ReqHistoricalData>(request_ptr);
                if (Request) {
                    Contract contract;
                    contract.symbol = Request->symbol;
                    contract.currency = Request->currency;
                    contract.exchange = Request->exchange;
                    contract.secType = Request->security_type;

                    client_->request_historical_data(Request->request_id, contract, Request->end_time, Request->duration,
                                                     Request->bar_size, Request->what_to_show, Request->use_rth,
                                                     Request->keep_up_to_date);
                } else {
                    logger_->error("Cannot cast the request to ReqHistoricalData.");
                }
            } else if (request_ptr->request_type == broker::RequestType::REQUEST_REALTIME_MKT_DATA) {
                auto Request = std::dynamic_pointer_cast<broker::ReqRealtimeMktData>(request_ptr);
                if (Request) {
                    Contract contract;
                    contract.symbol = Request->symbol;
                    contract.currency = Request->currency;
                    contract.exchange = Request->exchange;
                    contract.secType = Request->security_type;

                    client_->request_real_time_data(Request->request_id, contract);
                }
            } else if (request_ptr->request_type == broker::RequestType::CANCEL_REQUEST_HISTORICAL_DATA) {
                auto Request = std::dynamic_pointer_cast<broker::ReqCancelHistoricalData>(request_ptr);
                if (Request) {
                    client_->cancel_historical_data(Request->request_id);
                }
            } else if (request_ptr->request_type == broker::RequestType::CANCEL_REAL_TIME_MKT_DATA) {
                auto Request = std::dynamic_pointer_cast<broker::ReqCancelRealtimeMktData>(request_ptr);
                if (Request) {
                    client_->cancel_real_time_data(Request->request_id);
                }
            } else {
                logger_->warn("Cannot find the request type: {}", static_cast<int>(request_ptr->request_type));
            }
        }
    }
}

void TwsService::run_response(std::atomic<int> &tws_version) {
    int last_version = tws_version.load();

    while (true) {
        if (tws_version.load() != last_version) {
            // exit the thread
            break;
        }

        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, stop the response thread.");
            break;
        }

        client_->process_messages();
    }
}

void TwsService::distribute_response(std::atomic<int> &tws_version) {
    int last_version = tws_version.load();

    while (true) {
        if (tws_version.load() != last_version) {
            // exit the thread
            break;
        }

        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, stop the response thread.");
            break;
        }

        std::shared_ptr<broker::ResponseHeader> response_ptr = nullptr;
        response_queue_->wait_dequeue_timed(response_ptr, wait_timeout_);
        if (!response_ptr) {
            continue;
        }

        logger_->debug("Received response: {}", static_cast<int>(response_ptr->response_type));
        if (response_ptr->response_type == broker::RequestType::ERROR_MSG) {
            // TODO:
            logger_->error("Received error message.");
            continue;
        } else if (response_ptr->response_type == broker::RequestType::REQUEST_CURRENT_TIME) {
            // TODO: record current time
            continue;
        } else {
            TickerId request_id = response_ptr->request_id;
            auto iter = response_callbacks_.find(request_id);
            if (iter != response_callbacks_.end()) {
                iter->second(response_ptr);
            } else {
                logger_->warn("Cannot find the callback for request id: {}", request_id);
            }
        }
    }
}

void TwsService::run_monitor(std::atomic<int> &tws_version) {
    int last_version = tws_version.load();
    auto now = std::chrono::system_clock::now();

    while (true) {
        if (tws_version.load() != last_version) {
            // exit the thread
            break;
        }

        if (stop_flag_.load()) {
            logger_->info("Stop flag is set, stop the monitor thread.");
            break;
        }

        std::this_thread::sleep_for(wait_timeout_);

        // check the configuration file
        auto current = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current - now) > update_config_interval_) {
            now = current;
            set_reload_config();
            if (!load_config()) {
                logger_->error("Cannot update the configuration file.");
                continue;
            } else {
                update_config(tws_version);
            }
        }
    }
}

void TwsService::run_tws() {
    auto connect_func = [this]() {
        while (true) {
            if (stop_flag_.load()) {
                logger_->info("Stop flag is set, exit the tws client thread.");
                break;
            }
            // try connecting to TWS infinitely
            if (!client_->connect()) {
                logger_->error("Connect to TWS failed. Retry in {} milliseconds.", retry_interval_);
                std::this_thread::sleep_for(retry_interval_);
            } else {
                break;
            }
        }
    };

    // We will start four threads here
    // 1. this thread: connect to TWS and keep the connection, when the connection is lost, try to reconnect
    // 2. receive the data from TWS and process the data
    // 3. receive request from the strategy and send the request to TWS
    // 4. monitor the configuration file, update the configuration file when the file is changed
    // 5. distribute response to certain handler

    std::atomic<int> tws_version = 0;
    int last_tws_version = tws_version.load();

    std::shared_ptr<std::thread> process_request_thread = nullptr;
    std::shared_ptr<std::thread> process_response_thread = nullptr;
    std::shared_ptr<std::thread> config_monitor_thread = nullptr;
    std::shared_ptr<std::thread> distribute_response_thread = nullptr;

    auto wait_all = [&process_request_thread, &process_response_thread, &config_monitor_thread, &distribute_response_thread]() {
        if (process_request_thread && process_request_thread->joinable()) {
            process_request_thread->join();
        }
        if (process_response_thread && process_response_thread->joinable()) {
            process_response_thread->join();
        }
        if (config_monitor_thread && config_monitor_thread->joinable()) {
            config_monitor_thread->join();
        }
        if (distribute_response_thread && distribute_response_thread->joinable()) {
            distribute_response_thread->join();
        }
    };

    while (true) {
        if (!client_->is_connected()) {
            // try connect to TWS
            connect_func();

            if (client_->is_connected()) {
                tws_version.fetch_add(1);
            } else {
                if (stop_flag_.load()) {
                    logger_->info("Stop flag is set, exit the tws client thread.");
                    break;
                } else {
                    std::this_thread::sleep_for(retry_interval_);
                }
            }
        } else {
            if (stop_flag_.load()) {
                logger_->info("Stop flag is set, exit the tws client thread.");
                break;
            }

            if (tws_version.load() != last_tws_version) {
                last_tws_version = tws_version.load();
                // wait for all threads
                wait_all();

                // init data before race condition
                init_after_connected();

                // start the threads
                process_request_thread = std::make_shared<std::thread>(&TwsService::run_request, this, std::ref(tws_version));
                process_response_thread = std::make_shared<std::thread>(&TwsService::run_response, this, std::ref(tws_version));
                config_monitor_thread = std::make_shared<std::thread>(&TwsService::run_monitor, this, std::ref(tws_version));
                distribute_response_thread = std::make_shared<std::thread>(&TwsService::distribute_response, this, std::ref(tws_version));
            }

            // avoid 100% cpu usage
            std::this_thread::sleep_for(retry_interval_);

            // keep alive
            keep_alive();
        }
    }

    wait_all();
}

void TwsService::keep_alive() {
    static auto now = std::chrono::system_clock::now();

    auto alive_interval = std::chrono::milliseconds(30000);
    auto current = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(current - now) > alive_interval) {
        now = current;
        auto Request = std::make_shared<broker::ReqCurrentTime>();
        Request->request_id = 0;  // current time doesn't check up the request id
        push_request(std::dynamic_pointer_cast<broker::RequestHeader>(Request), std::nullopt);
    }
}

void TwsService::stop() {
    stop_flag_.store(true);
    logger_->info("Set stop flag");
}

bool TwsService::prepare() {
    bool result = ServiceBase<TwsService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // Get config information from config file
    int port = get_int_value(PORT_VARIABLE);
    std::string ip = get_string_value(HOST_VARIABLE);
    int clientid = get_int_value(CLIENTID_VARIABLE);
    int retry_interval = get_int_value(RETRY_INTERVAL_VARIABLE);
    int wait_timeout = get_int_value(WAIT_TIMEOUT_VARIABLE);
    int update_config_interval = get_int_value(UPDATE_CONFIG_INTERVAL_VARIABLE);

    if (retry_interval > 0) {
        retry_interval_ = std::chrono::milliseconds(retry_interval);
    }

    if (wait_timeout > 0) {
        wait_timeout_ = std::chrono::milliseconds(wait_timeout);
    }

    if (update_config_interval > 0) {
        update_config_interval_ = std::chrono::milliseconds(update_config_interval);
    }

    if (ip.empty() || port == 0 || clientid == 0) {
        logger_->error("Cannot get the configuration information from the config file. host {}, port {}, clientid {}", ip, port, clientid);
        return false;
    }

    if (client_thread_) {
        logger_->warn("Tws thread is already running.");
        return false;
    }

    client_ = std::make_shared<broker::TwsClient>(ip, port, clientid, wait_timeout);

    logger_->info("Configuration: host {}, port {}, clientid {}, retry interval {}, wait timeout {}, update config interval {}",
        ip, port, clientid, retry_interval_, wait_timeout_, update_config_interval_);

    return true;
}

bool TwsService::update_config(std::atomic<int> &tws_version) {
    bool modified = false;

    // Get config information from config file
    int port = get_int_value(PORT_VARIABLE);
    std::string ip = get_string_value(HOST_VARIABLE);
    int clientid = get_int_value(CLIENTID_VARIABLE);
    int retry_interval = get_int_value(RETRY_INTERVAL_VARIABLE);
    int wait_timeout = get_int_value(WAIT_TIMEOUT_VARIABLE);
    int update_config_interval = get_int_value(UPDATE_CONFIG_INTERVAL_VARIABLE);
    int record_log = get_int_value(RECORD_LOG_VARIABLE);
    int version = get_int_value(VERSION_VARIABLE);
    // TODO: use signal to stop rather than set stop in config file
    int stop_flag = get_int_value(STOP_FLAG_VARIABLE);

    const std::string_view old_ip = client_->get_host();
    const int old_port = client_->get_port();
    const int old_clientid = client_->get_clientid();
    const auto old_retry_interval = retry_interval_.count();
    const auto old_wait_timeout = wait_timeout_.count();
    const auto old_update_config_interval = update_config_interval_.count();

    if (retry_interval > 0) {
        if (retry_interval != retry_interval_.count()) {
            modified = true;
            retry_interval_ = std::chrono::milliseconds(retry_interval);
            logger_->info("Update the retry interval to {}", retry_interval_);
        }
    } 

    if (wait_timeout > 0) {
        if (wait_timeout != wait_timeout_.count()) {
            modified = true;
            wait_timeout_ = std::chrono::milliseconds(wait_timeout);
            logger_->info("Update the wait timeout to {}", wait_timeout_);
        }
    }

    if (update_config_interval > 0) {
        if (update_config_interval != update_config_interval_.count()) {
            modified = true;
            update_config_interval_ = std::chrono::milliseconds(update_config_interval);
            logger_->info("Update the update config interval to {}", update_config_interval_);
        }
    }

    // TODO: check the ip, port, clientid modification and restart client if necessary
    if (record_log > 0) {
        logger_->info("Cur configuration: host {}, port {}, clientid {}, retry interval {}, wait timeout {}, update config interval {}, version {}",
            old_ip, old_port, old_clientid, old_retry_interval, old_wait_timeout, old_update_config_interval, tws_version.load());
        logger_->info("New configuration: host {}, port {}, clientid {}, retry interval {}, wait timeout {}, update config interval {}, version {}, stop_flag {}",
            ip, port, clientid, retry_interval, wait_timeout, update_config_interval, version, stop_flag);
    }

    if (version > 0) {
        tws_version.store(version);
    }

    if (stop_flag > 0) {
        stop_flag_.store(true);
        logger_->info("Stop flag is set from configuration file.");
    }

    return true;
}

void TwsService::init_after_connected() {
    // start the threads
    error_queue_ = std::make_shared<moodycamel::BlockingConcurrentQueue<std::shared_ptr<broker::ResErrorMsg>>>();
    client_->set_response_queue(response_queue_);
    client_->set_error_queue(error_queue_);
}

}
}