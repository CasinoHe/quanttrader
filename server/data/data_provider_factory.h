#pragma once

#include "data_provider.h"
#include "common/singleton.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

namespace quanttrader {
namespace data {
namespace provider {

// Function type for creating data providers
using DataProviderCreatorFunc = std::function<std::shared_ptr<DataProvider>(const std::string_view&, DataParamsType)>;

/**
 * @brief Factory class for creating data providers
 * 
 * This class is responsible for registering and creating different types of data providers.
 * It follows the factory pattern to allow for dynamic provider creation based on type.
 */
class DataProviderFactory : public Singleton<DataProviderFactory> {

public:
    /**
     * @brief Register a data provider type with the factory
     * 
     * @param provider_type The type identifier for the provider
     * @param creator Function that creates a provider of that type
     */
    void register_provider(const std::string& provider_type, DataProviderCreatorFunc creator) {
        creators_[provider_type] = std::move(creator);
    }

    /**
     * @brief Create a data provider of the specified type
     * 
     * @param provider_type The type of provider to create
     * @param data_prefix Prefix for data parameters
     * @param params Parameters for the provider
     * @return A shared pointer to the created provider or nullptr if the type is not registered
     */
    std::shared_ptr<DataProvider> create_provider(
        const std::string& provider_type, 
        const std::string_view& data_prefix,
        DataParamsType params) {
        
        auto it = creators_.find(provider_type);
        if (it != creators_.end()) {
            return it->second(data_prefix, params);
        }
        return nullptr;
    }

    /**
     * Checks if a provider type is registered.
     * 
     * @param providerType The type name to check
     * @return true if the provider type is registered
     */
    bool hasProvider(const std::string& providerType) const {
        return creators_.find(providerType) != creators_.end();
    }

    /**
     * Removes a provider type from the registry.
     * 
     * @param providerType The type name to remove
     * @return true if successfully removed, false if not found
     */
    bool unregisterProvider(const std::string& providerType) {
        auto it = creators_.find(providerType);
        if (it == creators_.end()) {
            return false;
        }
        creators_.erase(it);
        return true;
    }

private:
    friend class Singleton<DataProviderFactory>;

    // Private constructor to enforce singleton pattern
    DataProviderFactory() = default;
    ~DataProviderFactory() = default;
    
    // Map of provider types to creator functions
    std::unordered_map<std::string, DataProviderCreatorFunc> creators_;
};

} // namespace provider
} // namespace data
} // namespace quanttrader