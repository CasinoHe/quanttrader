#pragma once

#include "strategy_base.h"
#include "strategy_factory.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace strategy {

/**
 * @brief Utility class for custom strategy creation and registration
 * 
 * This class provides templates and utilities to simplify the creation
 * and registration of custom strategies.
 */
class CustomStrategyHelper {
public:
    /**
     * @brief Register a custom strategy type with the strategy factory
     * 
     * @tparam StrategyT The strategy class type (must inherit from StrategyBase)
     * @param strategyName The name to register the strategy under
     * @return true if registration succeeded, false if already registered
     */
    template<typename StrategyT>
    static bool register_strategy(const std::string& strategyName) {
        static_assert(std::is_base_of<StrategyBase, StrategyT>::value, 
                     "StrategyT must inherit from StrategyBase");
        
        // Create a lambda that constructs the strategy
        auto createFunc = [](StrategyCreateFuncParemType& params) -> std::shared_ptr<StrategyBase> {
            auto strategy = std::make_shared<StrategyT>(params);
            return std::dynamic_pointer_cast<StrategyBase>(strategy);
        };
        
        return StrategyFactory::register_strategy(strategyName, createFunc);
    }
    
    /**
     * @brief Create a custom strategy by name
     * 
     * @param strategyName The registered name of the strategy to create
     * @param params Parameters to pass to the strategy constructor
     * @return std::shared_ptr<StrategyBase> The created strategy or nullptr if creation failed
     */
    static std::shared_ptr<StrategyBase> create_strategy(
        const std::string& strategyName,
        StrategyCreateFuncParemType& params) {
        
        return StrategyFactory::create_strategy(strategyName, params);
    }
};

} // namespace strategy
} // namespace quanttrader