#include "strategy_factory.h"
#include "strategy_loader.h"

namespace quanttrader {
namespace strategy {

void StrategyFactory::track_plugin_registration(const std::string& strategy_name) {
    std::string current_plugin = StrategyLoader::get_current_loading_plugin();
    if (!current_plugin.empty()) {
        StrategyLoader::track_strategy_registration(current_plugin, strategy_name);
    }
}

} // namespace strategy
} // namespace quanttrader
