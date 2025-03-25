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
 * @brief Utility class for working with CerebroBase instances
 * 
 * This class provides static methods to simplify the creation and use of CerebroBase instances.
 */
class CerebroUtil {
public:
    /**
     * @brief Create a backtest CerebroBase instance
     * 
     * @param name Name for this cerebro instance
     * @param configPath Path to the configuration file for the cerebro
     * @return std::shared_ptr<CerebroBase> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<CerebroBase> create_backtest(
        const std::string& name,
        const std::string& configPath) {
        
        return CerebroManager::instance().create_cerebro(BACKTEST_CEREBRO_TYPE, name, configPath);
    }
    
    /**
     * @brief Create a live trading CerebroBase instance
     * 
     * @param name Name for this cerebro instance
     * @param configPath Path to the configuration file for the cerebro
     * @return std::shared_ptr<CerebroBase> The created cerebro instance or nullptr if creation failed
     */
    static std::shared_ptr<CerebroBase> create_live(
        const std::string& name,
        const std::string& configPath) {
        
        return CerebroManager::instance().create_cerebro(LIVE_CEREBRO_TYPE, name, configPath);
    }
    
    /**
     * @brief Get a CerebroBase instance by name
     * 
     * @param name Name of the cerebro instance to retrieve
     * @return std::shared_ptr<CerebroBase> The cerebro instance or nullptr if not found
     */
    static std::shared_ptr<CerebroBase> get(const std::string& name) {
        return CerebroManager::instance().get_cerebro(name);
    }
    
    /**
     * @brief Destroy a CerebroBase instance by name
     * 
     * @param name Name of the cerebro instance to destroy
     * @return true if successfully destroyed
     * @return false if not found
     */
    static bool destroy(const std::string& name) {
        return CerebroManager::instance().destroy_cerebro(name);
    }
};

} // namespace cerebro
} // namespace quanttrader