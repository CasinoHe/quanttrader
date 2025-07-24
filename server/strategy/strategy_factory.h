#pragma once

#include "common/singleton.h"

#include <unordered_map>
#include <memory>
#include <functional>
#include <string>
#include <any>

namespace quanttrader {
namespace strategy {

class StrategyBase;
class StrategyLoader;

using StrategyCreateFuncParemType = std::unordered_map<std::string, std::any>;
using StrategyCreateFuncType = std::function<std::shared_ptr<StrategyBase>(StrategyCreateFuncParemType &)>;

class StrategyFactory : public Singleton<StrategyFactory> {
public:
    static bool register_strategy(const std::string &name, StrategyCreateFuncType create_func) {
        if (strategies_.find(name) != strategies_.end()) {
            return false;
        } else {
            strategies_[name] = create_func;
            
            // Track registration with the current loading plugin
            track_plugin_registration(name);
            
            return true;
        }
    }

    static bool unregister_strategy(const std::string &name) {
        auto it = strategies_.find(name);
        if (it != strategies_.end()) {
            strategies_.erase(it);
            return true;
        }
        return false;
    }

    static bool replace_strategy(const std::string &name, StrategyCreateFuncType create_func) {
        auto it = strategies_.find(name);
        if (it != strategies_.end()) {
            strategies_[name] = create_func;
            return true;
        }
        return false;
    }

    static bool has_strategy(const std::string &name) {
        return strategies_.find(name) != strategies_.end();
    }

    static std::vector<std::string> get_strategy_names() {
        std::vector<std::string> names;
        for (const auto& pair : strategies_) {
            names.push_back(pair.first);
        }
        return names;
    }

    static std::shared_ptr<StrategyBase> create_strategy(const std::string &strategy_type, StrategyCreateFuncParemType &params) {
        auto it = strategies_.find(strategy_type);
        if (it != strategies_.end()) {
            return it->second(params);
        }
        return nullptr;
    }

private:
    friend class Singleton<StrategyFactory>;
    
    // Private constructor to enforce Singleton pattern
    StrategyFactory() = default;
    ~StrategyFactory() = default;

    static void track_plugin_registration(const std::string& strategy_name);

    static inline std::unordered_map<std::string, StrategyCreateFuncType> strategies_;
};

}
}