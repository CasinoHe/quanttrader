#pragma once

#include "runner_base.h"
#include "cerebro/cerebro.h"
#include <memory>

namespace quanttrader {
namespace runner {

/**
 * @brief Runner implementation that uses the Cerebro system
 * 
 * This class bridges the gap between the existing Runner system and the new Cerebro system,
 * allowing strategies to be executed through either interface while maintaining compatibility.
 */
class CerebroRunner : public Runner {
public:
    /**
     * @brief Construct a new Cerebro Runner
     * 
     * @param params Configuration parameters
     */
    CerebroRunner(RunnerParamsType params)
        : Runner("cerebro_runner", params), cerebro_(nullptr) {
    }

    virtual ~CerebroRunner() = default;

    void on_tick() override {
        // Tick processing is handled by the Cerebro instance
    }

    void on_bar() override {
        // Bar processing is handled by the Cerebro instance
    }

    void on_trade() override {
        // Trade processing is handled by the Cerebro instance
    }

protected:
    bool on_init() override {
        logger_->info("Initializing CerebroRunner");

        // Extract cerebro type from parameters (default to backtest)
        std::string cerebroType = "backtest";
        auto typeIter = params_->find(cerebro::CEREBRO_TYPE_KEY);
        if (typeIter != params_->end()) {
            try {
                cerebroType = std::any_cast<std::string>(typeIter->second);
            } catch (const std::bad_any_cast&) {
                logger_->error("Invalid cerebro type in parameters");
                return false;
            }
        }

        // Create a cerebro instance
        cerebro_ = cerebro::CerebroFactory::instance().create_cerebro(
            cerebroType, "cerebro_" + strategy_name_, params_);
        
        if (!cerebro_) {
            logger_->error("Failed to create cerebro instance of type: {}", cerebroType);
            return false;
        }

        // Create and add strategy
        auto strategy = strategy::StrategyFactory::create_strategy(strategy_name_, *params_);
        if (!strategy) {
            logger_->error("Failed to create strategy: {}", strategy_name_);
            return false;
        }
        cerebro_->add_strategy(strategy);

        // Add data providers
        auto series_iter = params_->find(DATA_SERIES_VARIABLE_NAME);
        if (series_iter == params_->end()) {
            logger_->error("Cannot find {} column", DATA_SERIES_VARIABLE_NAME);
            return false;
        }

        std::string data_series = std::any_cast<std::string>(series_iter->second);
        std::vector<std::string> data_keys {};
        boost::split(data_keys, data_series, boost::is_any_of(SPLIT_DATA_SERIES_VARIABLE));
        
        for (auto &data_prefix : data_keys) {
            auto dataProvider = std::make_shared<data::DataProvider>(data_prefix, params_);
            if (!dataProvider->prepare_data()) {
                logger_->error("Failed to prepare data provider: {}", data_prefix);
                return false;
            }
            
            cerebro_->add_data(dataProvider);
            logger_->info("Added data provider: {}", data_prefix);
        }

        // Set broker provider
        std::string brokerType = "simulated";
        auto brokerIter = params_->find(cerebro::BROKER_TYPE_KEY);
        if (brokerIter != params_->end()) {
            try {
                brokerType = std::any_cast<std::string>(brokerIter->second);
            } catch (const std::bad_any_cast&) {
                logger_->error("Invalid broker type in parameters");
                return false;
            }
        }

        auto broker = broker::BrokerProviderFactory::instance().createProvider(brokerType, "");
        if (!broker) {
            logger_->error("Failed to create broker of type: {}", brokerType);
            return false;
        }
        
        cerebro_->set_broker(broker);
        logger_->info("Set broker of type: {}", brokerType);

        // Initialize the cerebro
        if (!cerebro_->initialize()) {
            logger_->error("Failed to initialize cerebro");
            return false;
        }

        logger_->info("CerebroRunner initialized successfully");
        return Runner::on_init();
    }

    void on_start() override {
        logger_->info("Starting CerebroRunner");
        // The actual running happens in run_frame()
    }

    void run_frame() override {
        // The first call to run_frame will start the cerebro
        if (!cerebro_run_started_) {
            cerebro_run_started_ = true;
            
            // Start the cerebro in the current thread
            logger_->info("Starting cerebro execution");
            cerebro_->run();
            
            // Set the ended flag when cerebro completes
            // This will happen for backtests, but not for live trading
            // For live trading, the on_finished method will be called when the runner is stopped
            ended_flag_.store(true);
        } else {
            // Sleep to prevent high CPU usage
            // For live trading, the cerebro is already running in a separate thread
            // For backtesting, the cerebro will have completed by this point
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void on_finished() override {
        logger_->info("Finishing CerebroRunner");
        
        if (cerebro_) {
            cerebro_->stop();
            cerebro_.reset();
        }
        
        Runner::on_finished();
    }

private:
    std::shared_ptr<cerebro::Cerebro> cerebro_;
    bool cerebro_run_started_ = false;
};

} // namespace runner
} // namespace quanttrader