#include "lines.h"
#include "data_consts.h"

namespace quanttrader {
namespace data {

std::optional<BarStruct> BarLine::next() {
    std::shared_lock lock(bar_mutex_);
    if (cur_ > bars_.start_time.size()) {
        return std::nullopt;
    }

    BarStruct bar;
    bar.time = bars_.start_time[cur_];
    bar.open = bars_.open[cur_];
    bar.high = bars_.high[cur_];
    bar.low = bars_.low[cur_];
    bar.close = bars_.close[cur_];
    bar.volume = bars_.volume[cur_];
    bar.swap = bars_.wap[cur_];
    bar.count = bars_.count[cur_];
    cur_++;
    return bar;
}

std::optional<int> BarLine::find_bar_position(uint64_t time) {
    if (bars_.start_time.size() == 0) {
        return kLastBarIndex;
    }

    int last_index = static_cast<int>(bars_.start_time.size() - 1);
    if (time == bars_.start_time[last_index]) {
        // replace the last bar
        return last_index;
    } else if (time < bars_.start_time[last_index]) {
        // find the proper position to inset the bar, if the time is dunplicated, return nullopt
        auto iter = std::lower_bound(bars_.start_time.begin(), bars_.start_time.end(), time);
        if (iter != bars_.start_time.end() && *iter == time) {
            return std::nullopt;
        } else {
            return static_cast<int>(std::distance(bars_.start_time.begin(), iter)) + 1;
        }
    } else {
        return kLastBarIndex;
    }
}

bool BarLine::emplace_back(uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count) {
    bars_.start_time.emplace_back(time);
    bars_.open.emplace_back(open);
    bars_.high.emplace_back(high);
    bars_.low.emplace_back(low);
    bars_.close.emplace_back(close);
    bars_.wap.emplace_back(swap);
    bars_.volume.emplace_back(vol);
    bars_.count.emplace_back(count);
    return true;
}

bool BarLine::emplace_back(const BarStruct &bar) {
    bars_.start_time.emplace_back(bar.time);
    bars_.open.emplace_back(bar.open);
    bars_.high.emplace_back(bar.high);
    bars_.low.emplace_back(bar.low);
    bars_.close.emplace_back(bar.close);
    bars_.wap.emplace_back(bar.swap);
    bars_.volume.emplace_back(bar.volume);
    bars_.count.emplace_back(bar.count);
    return true;
}

bool BarLine::push_data(const BarStruct &bar) {
    return push_data(bar.time, bar.open, bar.high, bar.low, bar.close, bar.volume, bar.swap, bar.count);
}

bool BarLine::push_data(uint64_t time, double open, double high, double low, double close, Decimal vol, Decimal swap, int count) {
    // lock the mutex first
    std::unique_lock lock(bar_mutex_);

    auto position_opt = find_bar_position(time);
    if (!position_opt.has_value()) {
        return false;
    }
    int position = position_opt.value();

    if (position == kLastBarIndex) {
        return emplace_back(time, open, high, low, close, vol, swap, count);
    } else if (position == bars_.start_time.size() - 1) {
        // replace the last data, maybe it is updated on real-time
        replace_data(position, time, open, high, low, close, vol, swap, count);
        return true;
    } else {
        // insert the data
        insert_data(position, time, open, high, low, close, vol, swap, count);
        return true;
    }
}

}
}