#pragma once

#include "broker_provider.h"
#include "common/singleton.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace quanttrader {
namespace broker {

// Function type for creating broker providers
using BrokerProviderCreatorFunc = std::function<std::shared_ptr<BrokerProvider>(const std::string&)>;

/**
 * Factory class for creating and managing broker providers.
 * This class follows the Singleton pattern.
 */
class BrokerProviderFactory : public Singleton<BrokerProviderFactory> {
public:
    /**
     * Creates a broker provider of the specified type.
     * 
     * @param providerType The type of broker provider to create
     * @param configPath Path to the configuration file for the broker
     * @return A shared pointer to the created broker provider
     */
    std::shared_ptr<BrokerProvider> createProvider(const std::string& providerType, const std::string& configPath) {
        auto it = providerCreators_.find(providerType);
        if (it == providerCreators_.end()) {
            return nullptr;
        }
        return it->second(configPath);
    }

    /**
     * Registers a new broker provider type with its creator function.
     * 
     * @param providerType The type name of the broker provider
     * @param creatorFunc The function to create instances of this provider
     * @return true if registration succeeded, false if already registered
     */
    bool registerProvider(const std::string& providerType, BrokerProviderCreatorFunc creatorFunc) {
        if (providerCreators_.find(providerType) != providerCreators_.end()) {
            return false;
        }
        providerCreators_[providerType] = creatorFunc;
        return true;
    }

    /**
     * Checks if a provider type is registered.
     * 
     * @param providerType The type name to check
     * @return true if the provider type is registered
     */
    bool hasProvider(const std::string& providerType) const {
        return providerCreators_.find(providerType) != providerCreators_.end();
    }

    /**
     * Removes a provider type from the registry.
     * 
     * @param providerType The type name to remove
     * @return true if successfully removed, false if not found
     */
    bool unregisterProvider(const std::string& providerType) {
        auto it = providerCreators_.find(providerType);
        if (it == providerCreators_.end()) {
            return false;
        }
        providerCreators_.erase(it);
        return true;
    }

private:
    friend class Singleton<BrokerProviderFactory>;
    
    BrokerProviderFactory() = default;
    ~BrokerProviderFactory() = default;

    // Map of provider type names to their creator functions
    std::unordered_map<std::string, BrokerProviderCreatorFunc> providerCreators_;
};

} // namespace broker
} // namespace quanttrader