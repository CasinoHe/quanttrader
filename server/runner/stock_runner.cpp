#include "stock_runner.h"
#include "data/data_provider.h"
#include "runner_const.h"
#include "strategy/strategy_factory.h"
#include <boost/algorithm/string.hpp>
#include <thread>

namespace quanttrader {
namespace runner {

namespace qdata = quanttrader::data;
namespace qstrategy = quanttrader::strategy;

bool StockRunner::on_init() {
    // prepare data request or read data
    auto series_iter = params_->find(DATA_SERIES_VARIABLE_NAME);
    if (series_iter == params_->end()) {
        logger_->error("Cannot find {} column", DATA_SERIES_VARIABLE_NAME);
        return false;
    }

    const std::string &strategy_name = std::any_cast<std::string>(params_->at(STRATEGY_NAME_VARIABLE));
    if (strategy_name.empty()) {
        logger_->error("Cannot find the strategy name in the runner params.");
        return false;
    }

    std::string data_series = std::any_cast<std::string>(series_iter->second);
    std::vector<std::string> data_keys {};
    boost::split(data_keys, data_series, boost::is_any_of(SPLIT_DATA_SERIES_VARIABLE));
    for (auto &data_prefix : data_keys) {
        auto data_provider = std::make_shared<qdata::DataProvider>(data_prefix, params_);
        data_providers_.emplace_back(data_provider);
    }

    for (auto &data_provider : data_providers_) {
        if (!data_provider->prepare_data()) {
            logger_->error("Cannot prepare data for data provider: {}", data_provider->get_data_prefix());
            return false;
        }
    }

    strategy_ = qstrategy::StrategyFactory::create_strategy(strategy_name, *params_);
    if (!strategy_) {
        logger_->error("Cannot create the strategy: {}", strategy_name);
        return false;
    }

    // initialize the runner
    return Runner::on_init();
}

void StockRunner::on_start() {
    // start the runner, there is data to process, all the data is prepared
    for (auto &data_provider : data_providers_) {
        logger_->info("Data provider {} {} is started.", data_provider->get_ticker_name(), data_provider->get_data_prefix());
        data_provider->start_request_data();
    }
}

void StockRunner::on_finished() {
    // finish the runner, prepare to exit
}

void StockRunner::on_tick() {
    // process the tick data
}

void StockRunner::on_bar() {
    // process the bar data
}

void StockRunner::on_trade() {

}

void StockRunner::on_history() {

}

void StockRunner::run_frame() {
    // check the availability of data
    for (auto &data_provider : data_providers_) {
        if (!data_provider->is_data_ready()) {
            return;
        }
    }

    // fetch data from data_provider

    // run a frame
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}
}