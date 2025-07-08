#pragma once

#include <map>
#include <string>
#include <cstdint>

namespace quanttrader {
namespace observer {

class ObserverBase {
public:
    virtual ~ObserverBase() = default;

    virtual void record_trade(uint64_t time,
                              const std::string& symbol,
                              int quantity,
                              double price,
                              bool is_buy) = 0;

    virtual void update_market_value(uint64_t time,
                                     const std::map<std::string, double>& prices) = 0;

    virtual void report() const = 0;
};

} // namespace observer
} // namespace quanttrader
