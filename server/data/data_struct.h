#pragma once

#include <memory>
#include <chrono>
#include <vector>

namespace quanttrader {
namespace data {

struct BarStruct {
    uint64_t time_; // time in nanoseconds
    double open_;   // open price
    double high_;   // high price
    double low_;    // low price
    double close_;  // close price

    double vol_;   // volume

    BarStruct() {memset(this, 0, sizeof(BarStruct));}
};

struct BarSeries {
    // in order to use the data in ta-lib and avoid copying the data, we don't use vector of BarStruct
    std::vector<uint64_t> time_; // time in nanoseconds
    std::vector<double> open_;   // open price
    std::vector<double> high_;   // high price
    std::vector<double> low_;    // low price
    std::vector<double> close_;  // close price
    std::vector<double> vol_;    // volume
};

struct LineStruct {
    BarSeries bars_;

    unsigned int cur_;
    unsigned int size_;
    unsigned int capacity_;
};

}
}