#pragma once

#include "Decimal.h"
#include <memory>
#include <chrono>
#include <vector>

namespace quanttrader {
namespace data {

enum class BarType {
    Tick,
    Second,
    Minute,
    Hour,
    Day,
    Week,
    Month,
    Year,
    NONE,
};

struct BarStruct {
    uint64_t time; // time in nanoseconds
    double open;   // open price
    double high;   // high price
    double low;    // low price
    double close;  // close price
    Decimal volume;   // volume
    Decimal swap;  // swap
    int count;     // count

    BarStruct() {memset(this, 0, sizeof(BarStruct));}
};

struct BarSeries {
    // in order to use the data in ta-lib and avoid copying the data, we don't use vector of BarStruct
    std::vector<uint64_t> start_time; // time in nanoseconds
    std::vector<double> open;   // open price
    std::vector<double> high;   // high price
    std::vector<double> low;    // low price
    std::vector<double> close;  // close price
    std::vector<Decimal> wap;   // weighted average price
    std::vector<Decimal> volume;  // volume
    std::vector<int> count;
};

}
}