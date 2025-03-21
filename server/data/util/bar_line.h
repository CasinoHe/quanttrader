#pragma once

#include "data/common/data_struct.h"
#include "data/common/data_consts.h"
#include "logger/quantlogger.h"
#include <shared_mutex>
#include <optional>
#include <array>

namespace quanttrader {
namespace data {
namespace util {

class BarLine {
public:
    BarLine(unsigned int capacity, BarType bar_type, unsigned int bar_size) : bars_() {
        logger_ = quanttrader::log::get_common_rotation_logger("BarLine", "data");

        bar_type_ = bar_type;
        bar_size_ = bar_size;
        // TODO: optimize the capacity to make calculation effectively
        // if (!set_capacity(capacity)) {
        //     logger_->error("{} Cannot set capacity to {} on init", static_cast<int>(bar_type), capacity);
        // }
    }

    bool push_data(uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count);
    bool push_data(const BarStruct &bar);

    bool set_capacity(unsigned int capacity) {
        if (capacity <= 0) {
            return false;
        }

        if (bars_.start_time.capacity() > capacity) {
            return false;
        }

        bars_.start_time.reserve(capacity);
        bars_.open.reserve(capacity);
        bars_.high.reserve(capacity);
        bars_.low.reserve(capacity);
        bars_.close.reserve(capacity);
        bars_.volume.reserve(capacity);

        capacity_ = capacity;
        return true;
    }

    const BarType get_bar_type() const {
        return bar_type_;
    }

    const unsigned int get_bar_size() const {
        return bar_size_;
    }

    std::optional<BarStruct> next();

    template<typename T, size_t N>
    std::optional<std::array<T, N>> get_bar_data(const std::string &data_name) {
        std::shared_lock lock(bar_mutex_);
        if (data_name == "start_time") {
            return &bars_.start_time;
        } else if (data_name == "open") {
            return &bars_.open;
        } else if (data_name == "high") {
            return &bars_.high;
        } else if (data_name == "low") {
            return &bars_.low;
        } else if (data_name == "close") {
            return &bars_.close;
        } else if (data_name == "wap") {
            return &bars_.wap;
        } else if (data_name == "volume") {
            return &bars_.volume;
        } else if (data_name == "count") {
            return &bars_.count;
        } else {
            return std::nullopt;
        }
    }

protected:
    std::optional<int> find_bar_position(uint64_t time);  // should lock the mutex outside
    bool emplace_back(const BarStruct &bar);
    bool emplace_back(uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count);

    inline void insert_data(int index, uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count) {
        bars_.start_time.insert(bars_.start_time.begin() + index, time);
        bars_.open.insert(bars_.open.begin() + index, open);
        bars_.high.insert(bars_.high.begin() + index, high);
        bars_.low.insert(bars_.low.begin() + index, low);
        bars_.close.insert(bars_.close.begin() + index, close);
        bars_.wap.insert(bars_.wap.begin() + index, swap);
        bars_.volume.insert(bars_.volume.begin() + index, vol);
        bars_.count.insert(bars_.count.begin() + index, count);
    }

    inline void replace_data(int index, uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count) {
        bars_.start_time[index] = time;
        bars_.open[index] = open;
        bars_.high[index] = high;
        bars_.low[index] = low;
        bars_.close[index] = close;
        bars_.wap[index] = swap;
        bars_.volume[index] = vol;
        bars_.count[index] = count;
    }

private:
    unsigned int cur_ = 0;
    unsigned int size_ = 0;
    unsigned int capacity_ = 0;
    BarType bar_type_;
    unsigned int bar_size_ = 0;
    BarSeries bars_;
    quanttrader::log::LoggerPtr logger_ {nullptr};

    std::shared_mutex bar_mutex_;
};

}  // namespace util
}  // namespace data
}  // namespace quanttrader