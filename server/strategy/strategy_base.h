#pragma once

#include <string>
#include <unordered_map>
#include <any>

namespace quanttrader {
namespace strategy {


struct StrategyConfig {
    std::string strategy_name;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    std::string time_zone;
    std::unordered_map<std::string, std::any> addtionals;
};

class Strategy {

};

}
}