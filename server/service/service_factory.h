#pragma once

#include "common/singleton.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

namespace quanttrader {
namespace service {

// Forward declaration for the service base interface
class ServiceInterface;

// Function type for creating services
using ServiceCreatorFunc = std::function<std::shared_ptr<ServiceInterface>(const std::string&)>;

/**
 * Factory class for creating and managing services.
 * This class follows the Singleton pattern.
 */
class ServiceFactory : public Singleton<ServiceFactory> {
public:
    /**
     * Creates a service of the specified type.
     * 
     * @param serviceName The name of the service to create
     * @param configPath Path to the configuration file for the service
     * @return A shared pointer to the created service
     */
    std::shared_ptr<ServiceInterface> createService(const std::string& serviceName, const std::string& configPath) {
        auto it = serviceCreators_.find(serviceName);
        if (it == serviceCreators_.end()) {
            return nullptr;
        }
        return it->second(configPath);
    }

    /**
     * Registers a new service type with its creator function.
     * 
     * @param serviceName The name of the service
     * @param creatorFunc The function to create instances of this service
     * @return true if registration succeeded, false if already registered
     */
    bool registerService(const std::string& serviceName, ServiceCreatorFunc creatorFunc) {
        if (serviceCreators_.find(serviceName) != serviceCreators_.end()) {
            return false;
        }
        serviceCreators_[serviceName] = creatorFunc;
        return true;
    }

    /**
     * Checks if a service is registered.
     * 
     * @param serviceName The service name to check
     * @return true if the service is registered
     */
    bool hasService(const std::string& serviceName) const {
        return serviceCreators_.find(serviceName) != serviceCreators_.end();
    }

    /**
     * Removes a service from the registry.
     * 
     * @param serviceName The service name to remove
     * @return true if successfully removed, false if not found
     */
    bool unregisterService(const std::string& serviceName) {
        auto it = serviceCreators_.find(serviceName);
        if (it == serviceCreators_.end()) {
            return false;
        }
        serviceCreators_.erase(it);
        return true;
    }

private:
    friend class Singleton<ServiceFactory>;
    
    ServiceFactory() = default;
    ~ServiceFactory() = default;

    // Map of service names to their creator functions
    std::unordered_map<std::string, ServiceCreatorFunc> serviceCreators_;
};

} // namespace service
} // namespace quanttrader
