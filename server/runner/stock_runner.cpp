#include "stock_runner.h"
#include "data/data_provider.h"
#include "runner_const.h"
#include <boost/algorithm/string.hpp>
#include <thread>

namespace quanttrader {
namespace runner {

namespace qdata = quanttrader::data;

bool StockRunner::on_init() {
    // prepare data request or read data
    auto series_iter = params_->find(DATA_SERIES_VARIABLE_NAME);
    if (series_iter == params_->end()) {
        logger_->error("Cannot find {} column", DATA_SERIES_VARIABLE_NAME);
        return false;
    }

    std::string data_series = std::any_cast<std::string>(series_iter->second);
    std::vector<std::string> data_keys {};
    boost::split(data_keys, data_series, boost::is_any_of(SPLIT_DATA_SERIES_VARIABLE));
    for (auto &data_prefix : data_keys) {
        auto data_provider = std::make_shared<qdata::DataProvider>(data_prefix, params_);
        data_providers_.emplace_back(data_provider);
    }

    // initialize the runner
    return Runner::on_init();
}

void StockRunner::on_start() {
    // start the runner, there is data to process, all the data is prepared
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

void StockRunner::run_frame() {
    // run a frame
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}
}