#include "runner_prepare.h"
#include "runner_types.h"
#include "stock_runner.h"
#include "runner_factory.h"

namespace quanttrader {
namespace runner {

bool prepare_stock_runners() {
    auto runner_creator = [](RunnerParamsType params) {
        return std::make_shared<StockRunner>(params);
    };

    if (RunnerFactory::instance()->register_runner(STOCK_RUNNER_NAME, runner_creator)) {
        return true;
    } else {
        return false;
    }
}

}
}