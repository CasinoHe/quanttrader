#include "cerebro/cerebro_factory.h"
#include "live_cerebro.h"
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
int register_live_cerebro() {
    auto factory = CerebroFactory::instance();
    
    // Register LiveCerebro
    factory->register_cerebro(LIVE_CEREBRO_TYPE, [](const std::string_view& name) {
        return std::make_shared<LiveCerebro>(name);
    });
    
    return 1;
}

// This ensures the cerebros are registered when the library is loaded
namespace {
    static int registered = register_live_cerebro();
}

} // namespace cerebro
} // namespace quanttrader