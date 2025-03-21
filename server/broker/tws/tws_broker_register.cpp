#include "tws_broker_adapter.h"
#include "broker_provider_factory.h"

namespace quanttrader {
namespace broker {

// Function to register the TWS broker provider with the factory
bool registerTwsBrokerProvider() {
    auto creatorFunc = [](const std::string& configPath) -> std::shared_ptr<BrokerProvider> {
        auto adapter = TwsBrokerAdapter::instance(configPath);
        if (!adapter->prepare()) {
            return nullptr;
        }
        return std::static_pointer_cast<BrokerProvider>(adapter);
    };
    
    return BrokerProviderFactory::instance()->registerProvider("tws", creatorFunc);
}

// Automatic registration
namespace {
    // This will register the TWS provider when the library is loaded
    static bool registered = registerTwsBrokerProvider();
}

} // namespace broker
} // namespace quanttrader