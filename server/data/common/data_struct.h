#pragma once

#include "Decimal.h"
#include <memory>
#include <chrono>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

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

    std::string to_string() const {
        std::time_t timestamp = time / 1000000000; // convert nanoseconds to seconds
        std::tm tm_utc;

        // Convert to tm (local time)
#ifdef _WIN32
        // Windows uses gmtime_s(result, &timestamp)
        if (gmtime_s(&tm_utc, &timestamp) != 0) {
            throw std::runtime_error("gmtime_s failed");
        }
#else
        // POSIX uses gmtime_r(&timestamp, result)
        if (gmtime_r(&timestamp, &tm_utc) == nullptr) {
            throw std::runtime_error("gmtime_r failed");
        }
#endif
        std::ostringstream ss;
        ss << std::put_time(&tm_utc, "%Y-%m-%d %H:%M:%S");

        // nanoseconds to date string
        return ss.str() + "UTC " + std::to_string(time) + " " + std::to_string(open) + " " + std::to_string(high) + " " +
               std::to_string(low) + " " + std::to_string(close) + " " + std::to_string(volume) +
               " " + std::to_string(swap) + " " + std::to_string(count);
    }
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