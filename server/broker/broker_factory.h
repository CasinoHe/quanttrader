#pragma once

#include "abstract_broker.h"
#include "backtest_broker.h"
#include "common/singleton.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace quanttrader {
namespace broker {

// Function type for creating brokers
using BrokerCreatorFunc = std::function<std::shared_ptr<AbstractBroker>(double)>;

/**
 * Factory class for creating different types of brokers
 */
class BrokerFactory : public Singleton<BrokerFactory> {
public:
    /**
     * Creates a broker of the specified type
     * 
     * @param broker_type The type of broker to create ("backtest", "live", etc.)
     * @param starting_cash Initial cash amount
     * @return A shared pointer to the created broker
     */
    std::shared_ptr<AbstractBroker> create_broker(const std::string& broker_type, double starting_cash = 100000.0) {
        auto it = broker_creators_.find(broker_type);
        if (it == broker_creators_.end()) {
            return nullptr;
        }
        return it->second(starting_cash);
    }
    
    /**
     * Registers a new broker type with its creator function
     * 
     * @param broker_type The type name of the broker
     * @param creator_func The function to create instances of this broker
     * @return true if registration succeeded, false if already registered
     */
    bool register_broker(const std::string& broker_type, BrokerCreatorFunc creator_func) {
        if (broker_creators_.find(broker_type) != broker_creators_.end()) {
            return false;
        }
        broker_creators_[broker_type] = creator_func;
        return true;
    }
    
    /**
     * Checks if a broker type is registered
     * 
     * @param broker_type The type name to check
     * @return true if the broker type is registered
     */
    bool has_broker(const std::string& broker_type) const {
        return broker_creators_.find(broker_type) != broker_creators_.end();
    }
    
    /**
     * Gets a list of all registered broker types
     * 
     * @return Vector of registered broker type names
     */
    std::vector<std::string> get_broker_types() const {
        std::vector<std::string> types;
        for (const auto& [type, creator] : broker_creators_) {
            types.push_back(type);
        }
        return types;
    }

private:
    friend class Singleton<BrokerFactory>;
    
    BrokerFactory() {
        // Register default broker types
        register_broker("backtest", [](double starting_cash) -> std::shared_ptr<AbstractBroker> {
            return std::make_shared<BacktestBroker>(starting_cash);
        });
    }
    
    ~BrokerFactory() = default;
    
    // Map of broker type names to their creator functions
    std::unordered_map<std::string, BrokerCreatorFunc> broker_creators_;
};

} // namespace broker
} // namespace quanttrader
