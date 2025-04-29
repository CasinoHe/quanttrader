#pragma once

#include "data/common/data_provider.h"
#include <memory>
#include <vector>
#include <deque>

namespace quanttrader {
namespace data {
namespace resampler {

/**
 * @brief Check if data can be resampled from one bar type to another
 * 
 * @param source_type The source bar type
 * @param target_type The target bar type
 * @return true if resampling is possible, false otherwise
 */
bool can_resample(BarType source_type, BarType target_type);

/**
 * @brief Data resampler class that converts data from one timeframe to another
 * 
 * This class wraps an existing data provider and resamples its data to a new timeframe.
 * It acts as a data provider itself, allowing transparent usage in the framework.
 */
class DataResampler : public provider::DataProvider {
public:
    /**
     * @brief Construct a new Data Resampler
     * 
     * @param source_provider The source data provider to resample from
     * @param target_type The target bar type
     * @param target_size The target bar size
     */
    DataResampler(std::shared_ptr<provider::DataProvider> source_provider, 
                 BarType target_type, unsigned int target_size);
    
    ~DataResampler() = default;

    // Implementation of DataProvider interface
    bool prepare_data() override;
    bool start_request_data() override;
    bool terminate_request_data() override;
    bool is_data_ready() override;
    std::optional<BarStruct> next() override;
    bool rewind() override;
    
private:
    /**
     * @brief Add a new bar to the current aggregation
     * 
     * @param bar The bar to add
     * @return true if a new resampled bar is completed and ready
     */
    bool add_to_aggregation(const BarStruct& bar);
    
    /**
     * @brief Finalize the current aggregation into a complete bar
     * 
     * @return The completed resampled bar
     */
    BarStruct finalize_aggregation();
    
    /**
     * @brief Check if the current bar belongs to the current aggregation period
     * 
     * @param bar_time The timestamp of the bar to check
     * @return true if the bar belongs to the current period, false otherwise
     */
    bool is_same_period(uint64_t bar_time) const;
    
    /**
     * @brief Align a timestamp to the start of its period based on target timeframe
     * 
     * @param time_ns The timestamp to align
     * @return The aligned timestamp
     */
    uint64_t align_to_period_start(uint64_t time_ns) const;

    std::shared_ptr<provider::DataProvider> source_provider_;
    std::deque<BarStruct> resampled_bars_;
    bool current_aggregation_started_ = false;
    uint64_t current_period_start_ = 0;
    
    // Aggregation state
    double agg_open_ = 0.0;
    double agg_high_ = 0.0;
    double agg_low_ = 0.0;
    double agg_close_ = 0.0;
    Decimal agg_volume_ = Decimal(0);
    Decimal agg_wap_ = Decimal(0);
    int agg_count_ = 0;
    int bar_count_ = 0;
};

} // namespace resampler
} // namespace data
} // namespace quanttrader