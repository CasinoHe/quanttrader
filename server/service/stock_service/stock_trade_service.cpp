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

    // Get and prepare broker provider
    auto broker_provider_name = get_string_value("broker_provider");
    auto broker_provider_config = get_string_value("broker_config");
    if (!broker_provider_name.empty()) {
        broker_provider_ = broker::BrokerProviderFactory::instance()->createProvider(broker_provider_name, broker_provider_config);
        if (!broker_provider_) {
            logger_->error("Failed to create the broker provider: {}, please check the configuration file.", broker_provider_name);
            qlog::Error("Failed to create the broker provider: {}, please check the configuration file.", broker_provider_name);
            return false;
        }
        logger_->info("Created broker provider: {}", broker_provider_name);
    } else {
        logger_->warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
        qlog::Warn("Broker provider name is empty. Please check the {}.broker_provider section of configuration file {}.", get_service_name(), get_config_path());
    }

    // Parse data provider configurations
    auto data_series = get_string_value("data_series");
    std::vector<std::string> data_prefixes;
    std::stringstream ds_ss(data_series);
    std::string data_prefix;
    while (std::getline(ds_ss, data_prefix, ',')) {
        if (!data_prefix.empty()) {
            data_prefixes.push_back(data_prefix);
        }
    }

    // Create data providers
    std::shared_ptr<std::unordered_map<std::string, std::any>> params = std::make_shared<std::unordered_map<std::string, std::any>>();
    auto& config_loader = qconfig::LuaConfigLoader::instance();
    config_loader.read_data_values(params.get());
    
    std::vector<std::shared_ptr<data::provider::DataProvider>> data_providers;
    for (const auto& prefix : data_prefixes) {
        std::string provider_type = get_string_value(prefix + ".provider_type");
        if (provider_type.empty()) {
            logger_->warn("Data provider type is empty for prefix: {}", prefix);
            continue;
        }
        
        auto provider = data::provider::DataProviderFactory::instance()->create_provider(provider_type, prefix, params);
        if (!provider) {
            logger_->error("Failed to create data provider with prefix: {} and type: {}", prefix, provider_type);
            return false;
        }
        
        if (!provider->prepare_data()) {
            logger_->error("Failed to prepare data provider: {}", prefix);
            return false;
        }
        
        data_providers.push_back(provider);
        logger_->info("Created and prepared data provider: {} of type: {}", prefix, provider_type);
    }

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

    // Prepare cerebro instances
    auto cerebro_names = get_string_value("cerebro_names");
    ss = std::stringstream(cerebro_names);
    std::string cerebro_name;

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

        // Create cerebro instance
        auto cerebro = cerebro::CerebroFactory::instance()->create_cerebro(cerebro_type, cerebro_name, cerebro_config);
        if (!cerebro) {
            logger_->error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            qlog::Error("Failed to create the cerebro: {}, please check the configuration file.", cerebro_name);
            return false;
        }
        
        // Add data providers to cerebro
        for (size_t i = 0; i < data_providers.size(); ++i) {
            std::string data_name = "data" + std::to_string(i+1);
            if (!cerebro->add_data(data_name, data_providers[i])) {
                logger_->error("Failed to add data provider {} to cerebro: {}", data_prefixes[i], cerebro_name);
                return false;
            }
            logger_->info("Added data provider {} to cerebro: {}", data_prefixes[i], cerebro_name);
            
            // Check if we need to resample this data
            std::string resample_key = cerebro_name + "." + data_name + ".resample";
            auto resample_str = get_string_value(resample_key);
            if (!resample_str.empty()) {
                auto [target_type, target_size] = data::provider::DataProvider::get_bar_type_from_string(resample_str);
                if (target_type != data::BarType::NONE) {
                    if (!cerebro->resample_data(data_name, target_type, target_size)) {
                        logger_->error("Failed to resample data {} to {}", data_name, resample_str);
                        return false;
                    }
                    logger_->info("Resampled data {} to {}", data_name, resample_str);
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
            }
            cerebro->set_replay_mode(mode);
            logger_->info("Set replay mode to {} for cerebro: {}", replay_mode_str, cerebro_name);
        }
        
        // Add strategies to cerebro
        for (const auto& strat_name : strategies) {
            auto strategy_params = std::make_shared<std::unordered_map<std::string, std::any>>(*params);
            (*strategy_params)["strategy_name"] = strat_name;
            
            auto strategy = strategy::StrategyFactory::create_strategy(strat_name, *strategy_params);
            if (!strategy) {
                logger_->error("Failed to create strategy: {}", strat_name);
                return false;
            }
            
            if (!cerebro->add_strategy(strategy)) {
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
