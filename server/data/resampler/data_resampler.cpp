#include "data_resampler.h"
#include "time/time_util.h"
#include <limits>
#include <algorithm>

namespace quanttrader {
namespace data {
namespace resampler {

bool can_resample(BarType source_type, BarType target_type) {
    // Define the ordering of bar types from smallest to largest
    static const std::vector<BarType> bar_order = {
        BarType::Tick,
        BarType::Second,
        BarType::Minute,
        BarType::Hour,
        BarType::Day,
        BarType::Week,
        BarType::Month
    };
    
    // Get the position of each type in the order
    auto source_pos = std::find(bar_order.begin(), bar_order.end(), source_type);
    auto target_pos = std::find(bar_order.begin(), bar_order.end(), target_type);
    
    // Make sure both types are valid
    if (source_pos == bar_order.end() || target_pos == bar_order.end()) {
        return false;
    }
    
    // We can only resample from smaller to larger timeframes
    return source_pos <= target_pos;
}

DataResampler::DataResampler(std::shared_ptr<provider::DataProvider> source_provider, 
                           BarType target_type, unsigned int target_size)
    : DataProvider(source_provider->get_data_prefix() + "_resampled", 
                  std::make_shared<std::unordered_map<std::string, std::any>>()),
      source_provider_(source_provider) {
    
    symbol_ = source_provider->get_symbol();
    bar_type_ = target_type;
    bar_size_ = target_size;
    
    logger_->info("Creating resampler from {} to {} for symbol {}", 
        get_bar_type_string(source_provider->get_bar_type(), source_provider->get_bar_size()),
        get_bar_type_string(target_type, target_size),
        symbol_);
}

bool DataResampler::prepare_data() {
    if (!source_provider_) {
        logger_->error("Source provider is null");
        return false;
    }
    
    // Initialize the bar line with appropriate type and size
    bar_line_ = std::make_shared<util::BarLine>(0, bar_type_, bar_size_);
    
    // Reset aggregate values
    current_aggregation_started_ = false;
    current_period_start_ = 0;
    agg_open_ = 0.0;
    agg_high_ = 0.0;
    agg_low_ = 0.0;
    agg_close_ = 0.0;
    agg_volume_ = Decimal(0);
    agg_wap_ = Decimal(0);
    agg_count_ = 0;
    bar_count_ = 0;
    
    // Clear any existing resampled bars
    resampled_bars_.clear();
    
    return true;
}

bool DataResampler::start_request_data() {
    if (!source_provider_) {
        logger_->error("Source provider is null");
        return false;
    }
    
    if (!source_provider_->is_data_ready()) {
        logger_->error("Source provider is not ready");
        return false;
    }
    
    // Preload data from source provider
    source_provider_->rewind();
    
    // Process all bars from source provider
    std::optional<BarStruct> bar;
    while ((bar = source_provider_->next()).has_value()) {
        if (add_to_aggregation(bar.value()) && bar_line_) {
            // A new resampled bar is ready, add it to our bar line
            BarStruct resampled_bar = finalize_aggregation();
            bar_line_->push_data(
                resampled_bar.time,
                resampled_bar.open,
                resampled_bar.high,
                resampled_bar.low,
                resampled_bar.close,
                resampled_bar.volume,
                resampled_bar.swap,
                resampled_bar.count
            );
        }
    }
    
    // If there's an incomplete bar at the end, finalize it too
    if (current_aggregation_started_ && bar_line_) {
        BarStruct resampled_bar = finalize_aggregation();
        bar_line_->push_data(
            resampled_bar.time,
            resampled_bar.open,
            resampled_bar.high,
            resampled_bar.low,
            resampled_bar.close,
            resampled_bar.volume,
            resampled_bar.swap,
            resampled_bar.count
        );
    }
    
    // Reset bar line to beginning
    if (bar_line_) {
        bar_line_->reset();
    }
    
    data_ready_ = true;
    return true;
}

bool DataResampler::terminate_request_data() {
    // Nothing to do for resampler
    return true;
}

bool DataResampler::is_data_ready() {
    return data_ready_;
}

std::optional<BarStruct> DataResampler::next() {
    if (!data_ready_ || !bar_line_) {
        return std::nullopt;
    }
    
    auto bar_opt = bar_line_->next();
    if (!bar_opt.has_value()) {
        return std::nullopt;
    }
    
    // Handle replay modes
    if (bar_opt.has_value()) {
        BarStruct current_bar = bar_opt.value();
        
        // For realtime mode, wait appropriately
        wait_for_realtime(current_bar, last_bar_);
        
        // For stepped mode, wait for step signal
        wait_for_step();
        
        // Update last bar for next time
        last_bar_ = current_bar;
    }
    
    return bar_opt;
}

bool DataResampler::rewind() {
    if (!bar_line_) {
        logger_->error("Cannot rewind resampler: bar line is not initialized");
        return false;
    }
    
    return bar_line_->reset();
}

bool DataResampler::add_to_aggregation(const BarStruct& bar) {
    uint64_t bar_time = bar.time;
    
    // If this is the first bar, start a new aggregation
    if (!current_aggregation_started_) {
        current_period_start_ = align_to_period_start(bar_time);
        agg_open_ = bar.open;
        agg_high_ = bar.high;
        agg_low_ = bar.low;
        agg_close_ = bar.close;
        agg_volume_ = bar.volume;
        agg_wap_ = bar.swap;
        agg_count_ = bar.count;
        bar_count_ = 1;
        current_aggregation_started_ = true;
        return false;
    }
    
    // Check if this bar belongs to the current period
    if (is_same_period(bar_time)) {
        // Update aggregation values
        agg_high_ = std::max(agg_high_, bar.high);
        agg_low_ = std::min(agg_low_, bar.low);
        agg_close_ = bar.close;
        agg_volume_ = agg_volume_ + bar.volume;
        // Update weighted average price if volume is available
        if (bar.volume > Decimal(0)) {
            Decimal bar_volume_weight = bar.volume / (agg_volume_);
            agg_wap_ = agg_wap_ + (bar.swap - agg_wap_) * bar_volume_weight;
        }
        agg_count_ += bar.count;
        bar_count_++;
        return false;
    } else {
        // The bar belongs to a new period, so the current aggregation is complete
        return true;
    }
}

BarStruct DataResampler::finalize_aggregation() {
    BarStruct resampled_bar;
    resampled_bar.time = current_period_start_;
    resampled_bar.open = agg_open_;
    resampled_bar.high = agg_high_;
    resampled_bar.low = agg_low_;
    resampled_bar.close = agg_close_;
    resampled_bar.volume = agg_volume_;
    resampled_bar.swap = agg_wap_;
    resampled_bar.count = agg_count_;
    
    // Reset aggregation
    current_aggregation_started_ = false;
    
    return resampled_bar;
}

bool DataResampler::is_same_period(uint64_t bar_time) const {
    uint64_t period_start = align_to_period_start(bar_time);
    return period_start == current_period_start_;
}

uint64_t DataResampler::align_to_period_start(uint64_t time_ns) const {
    using quanttrader::time::TimeUtil;
    
    switch (bar_type_) {
        case BarType::Second:
            return TimeUtil::align_to_second(time_ns, bar_size_);
        case BarType::Minute:
            return TimeUtil::align_to_minute(time_ns, bar_size_);
        case BarType::Hour:
            return TimeUtil::align_to_hour(time_ns, bar_size_);
        case BarType::Day:
            return TimeUtil::align_to_day(time_ns, bar_size_);
        case BarType::Week:
            return TimeUtil::align_to_week(time_ns);
        case BarType::Month:
            return TimeUtil::align_to_month(time_ns);
        default:
            return time_ns;
    }
}

} // namespace resampler
} // namespace data
} // namespace quanttrader