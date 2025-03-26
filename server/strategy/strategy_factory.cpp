#include "strategy_factory.h"
#include "strategy_base.h"
#include <unordered_map>
#include <memory>
#include <functional>
#include <string>

namespace quanttrader {
namespace strategy {

// We now have a static variable in the header, so no implementation needed here

// For backwards compatibility, provide a global function to create strategies
std::shared_ptr<StrategyBase> create_strategy(
    const std::string& strategy_type,
    StrategyCreateFuncParemType& params) {
    
    return StrategyFactory::create_strategy(strategy_type, params);
}

}
}