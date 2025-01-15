#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

namespace quanttrader {
namespace service {

class ServiceFactory {
public:
    template <typename ServiceType, typename... Args>
    static std::shared_ptr<ServiceType> get_service(Args&&... args) {
        auto type = std::type_index(typeid(ServiceType));
        auto it = services_.find(type);
        if (it == services_.end()) {
            auto service = ServiceType::instance(std::forward<Args>(args)...);
            services_[type] = service;
            return service;
        }
        return std::static_pointer_cast<ServiceType>(it->second);
    }

private:
    static inline std::unordered_map<std::type_index, std::shared_ptr<void>> services_;
};

}
}
