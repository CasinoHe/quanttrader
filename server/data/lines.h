#pragma once

#include "data_struct.h"
#include "logger/quantlogger.h"

namespace quanttrader {
namespace data {

class BarLine {
public:
    BarLine(unsigned int capacity, BarType bar_type, unsigned int bar_size) : bars_() {
        logger_ = quanttrader::log::get_common_rotation_logger("BarLine", "data", false);

        bar_type_ = bar_type;
        bar_size_ = bar_size;
        // TODO: optimize the capacity to make calculation effectively
        // if (!set_capacity(capacity)) {
        //     logger_->error("{} Cannot set capacity to {} on init", static_cast<int>(bar_type), capacity);
        // }
    }

    void emplace_back(const BarStruct &bar) {
        bars_.start_time.emplace_back(bar.time);
        bars_.open.emplace_back(bar.open);
        bars_.high.emplace_back(bar.high);
        bars_.low.emplace_back(bar.low);
        bars_.close.emplace_back(bar.close);
        bars_.wap.emplace_back(bar.swap);
        bars_.volume.emplace_back(bar.volume);
        bars_.count.emplace_back(bar.count);
    }

    void emplace_back(uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count) {
        bars_.start_time.emplace_back(time);
        bars_.open.emplace_back(open);
        bars_.high.emplace_back(high);
        bars_.low.emplace_back(low);
        bars_.close.emplace_back(close);
        bars_.wap.emplace_back(swap);
        bars_.volume.emplace_back(vol);
        bars_.count.emplace_back(count);
    }

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

private:
    unsigned int cur_ = 0;
    unsigned int size_ = 0;
    unsigned int capacity_ = 0;
    BarType bar_type_;
    unsigned int bar_size_ = 0;
    BarSeries bars_;
    quanttrader::log::LoggerPtr logger_ {nullptr};
};

}
}