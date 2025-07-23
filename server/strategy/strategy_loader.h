#pragma once

#include <string>

namespace quanttrader {
namespace strategy {

class StrategyLoader {
public:
    static bool load_plugins(const std::string &directory);
    static void unload_plugins();
};

}
}
