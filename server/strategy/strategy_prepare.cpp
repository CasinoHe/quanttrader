#include "strategy_prepare.h"
#include "strategy_base.h"
#include "strategy_factory.h"
#include "strategy_const.h"

#include "slope_strategy.h"

namespace quanttrader {
namespace strategy {

bool prepare_stategies() {
    auto slope_strategy_creator = [](StrategyCreateFuncParemType params) -> std::shared_ptr<StrategyBase> {
        auto strategy = std::make_shared<SlopeStrategy>(params);
        return std::dynamic_pointer_cast<StrategyBase>(strategy);
    };

    if (StrategyFactory::register_strategy(SLOPE_STRATEGY_NAME, slope_strategy_creator)) {
        return true;
    } else {
        return false;
    }
}

}
}