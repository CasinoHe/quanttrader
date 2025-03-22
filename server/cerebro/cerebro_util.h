#pragma once

#include "cerebro_base.h"
#include "cerebro_factory.h"
#include "cerebro_manager.h"
#include "cerebro_consts.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Utility class for working with Cerebro instances
 * 
 * This class provides static methods to simplify the creation and use of Cerebro instances.
 */
class CerebroUtil {
public:
    /**
     * @brief Create a backtest Cerebro instance
     * 
     * @param name Name for this cerebro instance
     * @param params Configuration parameters
     * @return std::shared_ptr<Cerebro> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<Cerebro> create_backtest(
        const std::string& name,
        CerebroParamsType params) {
        
        return CerebroManager::instance().create_cerebro(BACKTEST_CEREBRO_TYPE, name, params);
    }
    
    /**
     * @brief Create a live trading Cerebro instance
     * 
     * @param name Name for this cerebro instance
     * @param params Configuration parameters
     * @return std::shared_ptr<Cerebro> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<Cerebro> create_live(
        const std::string& name,
        CerebroParamsType params) {
        
        return CerebroManager::instance().create_cerebro(LIVE_CEREBRO_TYPE, name, params);
    }
    
    /**
     * @brief Get a Cerebro instance by name
     * 
     * @param name Name of the cerebro instance to retrieve
     * @return std::shared_ptr<Cerebro> The cerebro instance or nullptr if not found
     */
    static std::shared_ptr<Cerebro> get(const std::string& name) {
        return CerebroManager::instance().get_cerebro(name);
    }
    
    /**
     * @brief Destroy a Cerebro instance by name
     * 
     * @param name Name of the cerebro instance to destroy
     * @return true if successfully destroyed
     * @return false if not found
     */
    static bool destroy(const std::string& name) {
        return CerebroManager::instance().destroy_cerebro(name);
    }
    
    /**
     * @brief Create default parameters for a Cerebro instance
     * 
     * @param cerebroType The type of cerebro (e.g., "backtest" or "live")
     * @return CerebroParamsType Default parameters for this cerebro type
     */
    static CerebroParamsType create_default_params(const std::string& cerebroType) {
        auto params = std::make_shared<std::unordered_map<std::string, std::any>>();
        
        // Set common default parameters
        (*params)[CEREBRO_TYPE_KEY] = cerebroType;
        (*params)[INITIAL_CASH_KEY] = DEFAULT_INITIAL_CASH;
        (*params)[COMMISSION_KEY] = DEFAULT_COMMISSION;
        (*params)[SLIPPAGE_KEY] = DEFAULT_SLIPPAGE;
        
        // Set type-specific default parameters
        if (cerebroType == BACKTEST_CEREBRO_TYPE) {
            // Default parameters for backtesting
            (*params)[START_DATE_KEY] = "";  // Empty means use all available data
            (*params)[END_DATE_KEY] = "";    // Empty means use all available data
        }
        else if (cerebroType == LIVE_CEREBRO_TYPE) {
            // Default parameters for live trading
            // Add any live-specific parameters here
        }
        
        return params;
    }
};

} // namespace cerebro
} // namespace quanttrader