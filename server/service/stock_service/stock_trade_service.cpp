#include "stock_trade_service.h"
#include "service/service_consts.h"
#include "broker/broker_provider_factory.h"
#include "data/common/data_provider_factory.h"
#include "strategy/strategy_factory.h"
#include "strategy/strategy_loader.h"
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

bool StockTradeService::prepare_broker_service() {
    // Get and prepare broker provider
    auto broker_provider_name = get_string_value("broker_provider");
    auto broker_provider_config = get_string_value("broker_config");
    // if the broker provider config is set to "this", then use the current config file
    if (broker_provider_config == "this") {
        broker_provider_config = get_config_path();
    }
    if (!broker_provider_name.empty()) {
        broker_provider_ = broker::BrokerProviderFactory::instance()->createProvider(broker_provider_name, broker_provider_config);
        if (!broker_provider_) {
            logger_->error("Failed to create the broker provider: {}, please check the configuration file.", broker_provider_name);
            qlog::Error("Failed to create the broker provider: {}, please check the configuration file.", broker_provider_name);
            return false;
        }
        logger_->info("Created broker provider: {}", broker_provider_name);
        return true;
    } else {
        logger_->warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
        qlog::Warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
        return false;
    }
}

bool StockTradeService::prepare_data_series() {
    // Parse data provider configurations
    auto data_series = get_string_value("data_series");
    std::vector<std::string> data_names;
    std::stringstream ds_ss(data_series);
    std::string data_name;
    while (std::getline(ds_ss, data_name, ',')) {
        if (!data_name.empty()) {
            data_names.push_back(data_name);
        }
    }

    for (const auto& prefix : data_names) {
        std::string provider_type = get_string_value(prefix + ".provider_type");
        auto provider_config = get_string_value(prefix + ".provider_config");
        auto tickers = get_string_value(prefix + ".tickers");
        if (provider_type.empty()) {
            logger_->warn("Data provider type is empty for prefix: {}", prefix);
            continue;
        }
        if (provider_config == "this") {
            provider_config = get_config_path();
        }
        auto config_loader = quanttrader::luascript::LuaConfigLoader(provider_config);
        config_loader.load_config();
        // TODO: Hot reload provider config
        
        // each ticker has its own data provider, so we need to create a data provider for each ticker
        std::stringstream ss(tickers);
        std::string ticker;
        while (std::getline(ss, ticker, ',')) {
            if (ticker.empty()) {
                continue;
            }

            // get all params from configuration file
            auto params = std::make_shared<std::unordered_map<std::string, std::any>>();
            auto data_name = prefix + "_" + ticker;
            // read from config file and overwrite with crutial params
            config_loader.get_all_values(prefix, *params);
            (*params)["data_name"] = data_name;
            (*params)["broker_provider"] = broker_provider_;
            (*params)["provider_type"] = provider_type;
            (*params)["provider_config"] = provider_config;
            (*params)["ticker"] = ticker;

            auto provider = data::provider::DataProviderFactory::instance()->create_provider(provider_type, data_name, params);
            if (!provider) {
                logger_->error("Failed to create data provider with name: {} and type: {}", data_name, provider_type);
                return false;
            }

            // set broker provider, the data provider may need a broker provider to get data
            provider->set_broker(broker_provider_);
            
            if (!provider->prepare_data()) {
                logger_->error("Failed to prepare data provider: {}", prefix);
                return false;
            }
            
            data_providers_.push_back(provider);
            logger_->info("Created and prepared data provider: {} of type: {} for {}", prefix, provider_type, ticker);
        }
    }
    return true;
}

bool StockTradeService::prepare_cerebro() {
    std::stringstream ss;

    // Prepare cerebro instances
    auto cerebro_names = get_string_value("cerebro_names");
    ss = std::stringstream(cerebro_names);
    std::string cerebro_name;

    while (std::getline(ss, cerebro_name, ',')) {
        if (cerebro_name.empty()) {
            continue;
        }
        auto cerebro_type = get_string_value(cerebro_name + ".cerebro_type");
        int wait_data_timeout = get_int_value(cerebro_name + ".wait_data_timeout");

        // Create cerebro instance
        auto cerebro = cerebro::CerebroFactory::instance()->create_cerebro(cerebro_type, cerebro_name);
        if (!cerebro) {
            logger_->error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            qlog::Error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            return false;
        }

        // Set wait data ready timeout
        if (wait_data_timeout > 0) {
            cerebro->set_wait_data_ready_timeout(wait_data_timeout);
        }
        
        // Add data providers to cerebro
        for (auto iter = data_providers_.begin(); iter != data_providers_.end(); iter++) {
            auto provider = *iter;
            std::string data_name = provider->get_data_name();
            if (!cerebro->add_data(data_name, provider)) {
                logger_->error("Failed to add data provider {} to cerebro: {}", data_name, cerebro_name);
                return false;
            } else {
                logger_->info("Added data provider {} to cerebro: {}", data_name, cerebro_name);
            }
            
            // Check if we need to resample this data
            if (provider->need_resample()) {
                auto resample_size = provider->get_resample_size();
                if (resample_size.empty()) {
                    logger_->error("Resample size is empty for data provider: {}", data_name);
                    return false;
                }
                auto [target_type, target_size] = data::provider::DataProvider::get_bar_type_from_string(resample_size);
                if (target_type != data::BarType::NONE) {
                    if (!cerebro->resample_data(data_name, target_type, target_size)) {
                        logger_->error("Failed to resample data {} to {}", data_name, resample_size);
                        return false;
                    }
                    logger_->info("Resampled data {} to {}", data_name, resample_size);
                }
            }
        }
        
        // Set replay mode for the cerebro
        std::string replay_mode_str = get_string_value(cerebro_name + ".replay_mode");
        if (!replay_mode_str.empty()) {
            data::provider::DataProvider::ReplayMode mode = data::provider::DataProvider::ReplayMode::NORMAL;
            if (replay_mode_str == "realtime") {
                mode = data::provider::DataProvider::ReplayMode::REALTIME;
            } else if (replay_mode_str == "stepped") {
                mode = data::provider::DataProvider::ReplayMode::STEPPED;
            } else if (replay_mode_str != "normal") {
                logger_->error("Invalid replay mode: {} for cerebro: {}", replay_mode_str, cerebro_name);
                return false;
            }
            cerebro->set_replay_mode(mode);
            logger_->info("Set replay mode to {} for cerebro: {}", replay_mode_str, cerebro_name);
        }
        
        // Add strategies to cerebro
        for (const auto& strat : strategy_map_) {
            const std::string &strat_name = strat.first;
            if (!cerebro->add_strategy(strat.second)) {
                logger_->error("Failed to add strategy {} to cerebro: {}", strat_name, cerebro_name);
                return false;
            }
            logger_->info("Added strategy {} to cerebro: {}", strat_name, cerebro_name);
        }
        
        // Store cerebro instance
        cerebro_map_[cerebro_name] = cerebro;
        
        // Prepare the cerebro instance
        if (!cerebro->prepare()) {
            logger_->error("Failed to prepare cerebro: {}", cerebro_name);
            return false;
        }
        logger_->info("Successfully prepared cerebro: {}", cerebro_name);
    }
    return true;
}

bool StockTradeService::prepare_strategyes() {
    // Load strategy plugins
    std::string plugin_dir = get_string_value(STRATEGY_LIB_PATH);
    if (plugin_dir.empty()) {
        plugin_dir = "strategies";
    }
    strategy::StrategyLoader::load_plugins(plugin_dir);

    // Get strategy configurations
    auto strategy_names = get_string_value("strategy_names");
    std::vector<std::string> strategies;
    std::stringstream ss(strategy_names);
    std::string strategy_name;
    while (std::getline(ss, strategy_name, ',')) {
        if (!strategy_name.empty()) {
            strategies.push_back(strategy_name);
        }
    }

    for (const auto &strategy_name : strategies) {
        // build strategy data
        auto params = std::make_shared<std::unordered_map<std::string, std::any>>();
        get_all_values_in_table(strategy_name, *params);
        (*params)["strategy_name"] = strategy_name;
        const auto strategy_type = get_string_value_in_table(strategy_name, "strategy_type");

        auto strategy = strategy::StrategyFactory::create_strategy(strategy_type, *params);
        if (!strategy) {
            logger_->error("Failed to create strategy: {}", strategy_name);
            return false;
        } else {
            logger_->info("Created strategy: {}", strategy_name);
        }

        strategy_map_[strategy_name] = strategy;
    }

    return true;
}

bool StockTradeService::prepare() {
    auto result = ServiceBase<StockTradeService>::prepare();
    if (!result) {
        logger_->error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        qlog::Error("Cannot load configuration file: {}. Please check the existence of the file or try to run the file.", get_config_path());
        return false;
    }

    // prepare broker service
    if (!prepare_broker_service()) {
        logger_->error("Failed to prepare broker service. Please check the configuration file.");
        return false;
    }

    // prepare data series 
    if (!prepare_data_series()) {
        logger_->error("Failed to prepare data series. Please check the configuration file.");
        return false;
    }

    // prepare strategies
    if (!prepare_strategyes()) {
        logger_->error("Failed to prepare strategies. Please check the configuration file.");
        return false;
    }

    // prepare cerebro
    if (!prepare_cerebro()) {
        logger_->error("Failed to prepare cerebro. Please check the configuration file.");
        return false;
    }

    return !cerebro_map_.empty();
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
    strategy::StrategyLoader::unload_plugins();
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
