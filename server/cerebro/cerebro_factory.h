#pragma once

#include "cerebro_base.h"
#include "common/singleton.h"
#include "cerebro/cerebro_consts.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace quanttrader {
namespace cerebro {

// Function type for creating cerebro instances
using CerebroCreatorFunc = std::function<std::shared_ptr<CerebroBase>(const std::string_view&)>;

/**
 * @brief Factory class for creating and managing CerebroBase instances
 * 
 * This class follows the Singleton pattern to ensure only one factory exists
 * in the application.
 */
class CerebroFactory : public Singleton<CerebroFactory> {
public:
    /**
     * @brief Create a cerebro instance of the specified type
     * 
     * @param cerebroType The type of cerebro to create
     * @param name The name for the new cerebro instance
     * @param configPath Path to the configuration file for the cerebro
     * @return std::shared_ptr<CerebroBase> A shared pointer to the created cerebro
     */
    std::shared_ptr<CerebroBase> create_cerebro(
        const std::string& cerebroType, 
        const std::string_view& name) {
        auto it = cerebro_creators_.find(cerebroType);
        if (it == cerebro_creators_.end()) {
            return nullptr;
        }
        return it->second(name);
    }

    /**
     * @brief Register a new cerebro type with its creator function
     * 
     * @param cerebroType The type name of the cerebro
     * @param creatorFunc The function to create instances of this cerebro
     * @return true if registration succeeded, false if already registered
     */
    bool register_cerebro(const std::string& cerebroType, CerebroCreatorFunc creatorFunc) {
        if (cerebro_creators_.find(cerebroType) != cerebro_creators_.end()) {
            return false;
        }
        cerebro_creators_[cerebroType] = creatorFunc;
        return true;
    }

    /**
     * @brief Check if a cerebro type is registered
     * 
     * @param cerebroType The type name to check
     * @return true if the cerebro type is registered
     */
    bool has_cerebro(const std::string& cerebroType) const {
        return cerebro_creators_.find(cerebroType) != cerebro_creators_.end();
    }

    /**
     * @brief Remove a cerebro type from the registry
     * 
     * @param cerebroType The type name to remove
     * @return true if successfully removed, false if not found
     */
    bool unregister_cerebro(const std::string& cerebroType) {
        auto it = cerebro_creators_.find(cerebroType);
        if (it == cerebro_creators_.end()) {
            return false;
        }
        cerebro_creators_.erase(it);
        return true;
    }

private:
    friend class Singleton<CerebroFactory>;
    
    // Private constructor to enforce Singleton pattern
    CerebroFactory() = default;
    ~CerebroFactory() = default;

    // Map of cerebro type names to their creator functions
    std::unordered_map<std::string, CerebroCreatorFunc> cerebro_creators_;
};

} // namespace cerebro
} // namespace quanttrader