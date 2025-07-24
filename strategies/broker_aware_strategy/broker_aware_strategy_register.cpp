#include "broker_aware_strategy.h"
#include "strategy/strategy_factory.h"
#include <memory>

namespace quanttrader {
namespace strategy {

// Local strategy name - each plugin defines its own
constexpr const char* STRATEGY_NAME = "broker_aware";

// Function to register the BrokerAwareStrategy with the factory
bool registerBrokerAwareStrategy() {
    auto creatorFunc = [](StrategyCreateFuncParemType &param) -> std::shared_ptr<StrategyBase> {
        auto strategy = std::make_shared<BrokerAwareStrategy>(param);
        strategy->initialize();
        return std::static_pointer_cast<StrategyBase>(strategy);
    };
    
    return StrategyFactory::instance()->register_strategy(STRATEGY_NAME, creatorFunc);
}

// Automatic registration
namespace {
    // This will register the BrokerAwareStrategy when the library is loaded
    static bool registered = registerBrokerAwareStrategy();
}

} // namespace strategy
} // namespace quanttrader

extern "C" {
    void register_strategies() {
        // Registration happens automatically when the library is loaded
    }
}
