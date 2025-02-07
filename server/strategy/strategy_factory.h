#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include <string>
#include <any>

namespace quanttrader {
namespace strategy {

class Strategy;

using StrategyCreateFuncParemType = std::unordered_map<std::string, std::any>;
using StrategyCreateFuncType = std::function<std::shared_ptr<Strategy>(StrategyCreateFuncParemType &)>;

class StrategyFactory {
public:
    static bool register_strategy(const std::string &name, StrategyCreateFuncType create_func) {
        if (strategies_.find(name) != strategies_.end()) {
            return false;
        } else {
            strategies_[name] = create_func;
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

    static std::shared_ptr<Strategy> create_strategy(const std::string &name, StrategyCreateFuncParemType &params) {
        auto it = strategies_.find(name);
        if (it != strategies_.end()) {
            return it->second(params);
        }
        return nullptr;
    }

private:
    static inline std::unordered_map<std::string, StrategyCreateFuncType> strategies_;
};

}
}