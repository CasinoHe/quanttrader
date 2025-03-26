#pragma once

#include "strategy_base.h"
#include "strategy_factory.h"
#include "strategy_const.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace strategy {

/**
 * @brief Utility class for working with strategies
 * 
 * This class provides static methods to simplify the creation and 
 * configuration of strategy instances.
 */
class StrategyUtil {
public:
    /**
     * @brief Create a simple moving average crossover strategy (SlopeStrategy)
     * 
     * @param name The name to assign to this strategy instance
     * @param symbol The trading symbol to use
     * @param fast_period The fast moving average period
     * @param slow_period The slow moving average period
     * @return std::shared_ptr<StrategyBase> The created strategy or nullptr if creation failed
     */
    static std::shared_ptr<StrategyBase> create_ma_crossover(
        const std::string& name,
        const std::string& symbol,
        int fast_period = 20,
        int slow_period = 50) {
        
        StrategyCreateFuncParemType params;
        params["strategy_name"] = name;
        params["symbol"] = symbol;
        params["fast_period"] = fast_period;
        params["slow_period"] = slow_period;
        
        return StrategyFactory::create_strategy(SLOPE_STRATEGY_NAME, params);
    }
    
    /**
     * @brief Create a custom strategy by type
     * 
     * @param type The strategy type to create
     * @param params The parameters to pass to the strategy
     * @return std::shared_ptr<StrategyBase> The created strategy or nullptr if creation failed
     */
    static std::shared_ptr<StrategyBase> create_strategy(
        const std::string& type,
        StrategyCreateFuncParemType& params) {
        
        return StrategyFactory::create_strategy(type, params);
    }
};

} // namespace strategy
} // namespace quanttrader