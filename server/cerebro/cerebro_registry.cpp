#include "cerebro_factory.h"
#include "backtest_cerebro.h"
#include "live_cerebro.h"
#include <memory>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Register all cerebro implementations with the factory
 * 
 * This function must be called during application initialization to register
 * all cerebro implementations with the factory.
 * 
 * @return int Returns 1 if registration was successful
 */
int register_cerebros() {
    auto& factory = CerebroFactory::instance();
    
    // Register BacktestCerebro
    factory.register_cerebro("backtest", [](const std::string_view& name, CerebroParamsType params) {
        return std::make_shared<BacktestCerebro>(name, params);
    });
    
    // Register LiveCerebro
    factory.register_cerebro("live", [](const std::string_view& name, CerebroParamsType params) {
        return std::make_shared<LiveCerebro>(name, params);
    });
    
    return 1;
}

// This ensures the cerebros are registered when the library is loaded
namespace {
    static int registered = register_cerebros();
}

} // namespace cerebro
} // namespace quanttrader