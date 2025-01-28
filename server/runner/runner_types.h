#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <any>

namespace quanttrader {
namespace runner {

class Runner;

using RunnerParamsType = std::shared_ptr<std::unordered_map<std::string, std::any>>;
using RunnerCreatorFuncType = std::function<std::shared_ptr<Runner>(RunnerParamsType)>;

constexpr const char* STOCK_RUNNER_NAME = "StockRunner";

}
}