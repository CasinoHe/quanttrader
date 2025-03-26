#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <functional>
#include <memory>

namespace quanttrader {
namespace strategy {

class StrategyBase;

using StrategyCreateFuncParemType = std::unordered_map<std::string, std::any>;
using StrategyCreateFuncType = std::function<std::shared_ptr<StrategyBase>(StrategyCreateFuncParemType &)>;


class StrategyBase {
public:
    StrategyBase(StrategyCreateFuncParemType params) {
    }

    virtual ~StrategyBase() = default;

    virtual void on_tick() = 0;
    virtual void on_bar() = 0;
    virtual void on_trade() = 0;
    virtual std::string get_name() const = 0;
    virtual bool initialize() = 0;
    virtual bool on_stop() = 0;
    virtual bool on_start() = 0;
    virtual bool on_order() = 0;
    virtual bool on_signal() = 0;
    virtual bool on_data() = 0;
};

}
}