#include "slope_strategy.h"
#include "strategy/strategy_factory.h"
#include "strategy/strategy_const.h"
#include <memory>

namespace quanttrader {
namespace strategy {

// Function to register the TWS broker provider with the factory
bool registerSlopeStrategy() {
    auto creatorFunc = [](StrategyCreateFuncParemType &param) -> std::shared_ptr<StrategyBase> {
        auto strategy = std::make_shared<SlopeStrategy>(param);
        strategy->initialize();
        return std::static_pointer_cast<StrategyBase>(strategy);
    };
    
    return StrategyFactory::instance()->register_strategy(SLOPE_STRATEGY_NAME, creatorFunc);
}

// Automatic registration
namespace {
    // This will register the TWS provider when the library is loaded
    static bool registered = registerSlopeStrategy();
}

} // namespace broker
} // namespace quanttrader