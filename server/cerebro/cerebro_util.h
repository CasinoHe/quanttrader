#pragma once

#include "cerebro_base.h"
#include "cerebro_factory.h"
#include "cerebro_consts.h"
#include "strategy/strategy_util.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Utility class for working with CerebroBase instances
 * 
 * This class provides static methods to simplify the creation and use of CerebroBase instances.
 * It works directly with the CerebroFactory instead of using a separate manager.
 */
class CerebroUtil {
public:
    /**
     * @brief Create a backtest cerebro instance
     * 
     * @param name Name for this cerebro instance
     * @return std::shared_ptr<CerebroBase> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<CerebroBase> create_backtest(const std::string_view& name) {
        auto factory = CerebroFactory::instance();
        return factory->create_cerebro(BACKTEST_CEREBRO_TYPE, name);
    }
    
    /**
     * @brief Create a live trading cerebro instance
     * 
     * @param name Name for this cerebro instance
     * @return std::shared_ptr<CerebroBase> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<CerebroBase> create_live(const std::string_view& name) {
        auto factory = CerebroFactory::instance();
        return factory->create_cerebro(LIVE_CEREBRO_TYPE, name);
    }

    /**
     * @brief Add a moving average crossover strategy to a cerebro instance
     * 
     * @param cerebro The cerebro instance to add the strategy to
     * @param name The name to give the strategy
     * @param symbol The symbol to trade
     * @param fast_period The fast moving average period
     * @param slow_period The slow moving average period
     * @return true if successfully added
     * @return false if failed to add
     */
    static bool add_ma_crossover_strategy(
        std::shared_ptr<CerebroBase> cerebro,
        const std::string& name,
        const std::string& symbol,
        int fast_period = 20,
        int slow_period = 50) {
        
        if (!cerebro) {
            return false;
        }
        
        auto strategy = strategy::StrategyUtil::create_ma_crossover(
            name, symbol, fast_period, slow_period);
        
        if (!strategy) {
            return false;
        }
        
        return cerebro->add_strategy(strategy);
    }
    
    /**
     * @brief Add a generic strategy to a cerebro instance
     * 
     * @param cerebro The cerebro instance to add the strategy to
     * @param type The type of strategy to create
     * @param params Parameters for strategy creation
     * @return true if successfully added
     * @return false if failed to add
     */
    static bool add_strategy(
        std::shared_ptr<CerebroBase> cerebro,
        const std::string& type,
        strategy::StrategyCreateFuncParemType& params) {
        
        if (!cerebro) {
            return false;
        }
        
        auto strategy = strategy::StrategyUtil::create_strategy(type, params);
        
        if (!strategy) {
            return false;
        }
        
        return cerebro->add_strategy(strategy);
    }
};

} // namespace cerebro
} // namespace quanttrader