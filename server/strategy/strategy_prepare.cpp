#include "strategy_prepare.h"
#include "strategy_base.h"
#include "strategy_factory.h"
#include "strategy_const.h"
#include "slope_strategy.h"
#include "logger/quantlogger.h"

namespace quanttrader {
namespace strategy {

bool prepare_stategies() {
    auto logger = quanttrader::log::get_common_rotation_logger("StrategyPrepare", "strategy");
    
    // Register SlopeStrategy (MA Crossover)
    auto slope_strategy_creator = [](StrategyCreateFuncParemType params) -> std::shared_ptr<StrategyBase> {
        auto strategy = std::make_shared<SlopeStrategy>(params);
        return std::dynamic_pointer_cast<StrategyBase>(strategy);
    };

    if (StrategyFactory::register_strategy(SLOPE_STRATEGY_NAME, slope_strategy_creator)) {
        logger->info("Successfully registered SlopeStrategy");
    } else {
        logger->error("Failed to register SlopeStrategy");
        return false;
    }
    
    // Register other strategies here...
    
    logger->info("All strategies registered successfully");
    return true;
}

}
}