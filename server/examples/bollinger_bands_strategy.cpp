#include "strategy/strategy_base.h"
#include "strategy/custom_strategy_helper.h"
#include "data/bar_struct.h"
#include <deque>
#include <iostream>

namespace quanttrader {
namespace examples {

/**
 * @brief Example custom strategy implementing a Bollinger Bands trading system
 * 
 * This strategy buys when price dips below the lower band and sells
 * when price rises above the upper band.
 */
class BollingerBandsStrategy : public strategy::StrategyBase {
public:
    BollingerBandsStrategy(strategy::StrategyCreateFuncParemType params)
        : StrategyBase(params) {
        
        // Get strategy parameters with defaults
        period_ = get_param<int>("period", 20);
        num_std_dev_ = get_param<double>("num_std_dev", 2.0);
        symbol_ = get_param<std::string>("symbol", "");
        
        strategy_name_ = "BollingerBands";
        logger_->info("Created BollingerBandsStrategy with period={}, std_dev={}, symbol={}",
                    period_, num_std_dev_, symbol_);
    }

    bool initialize() override {
        StrategyBase::initialize();
        
        // Make sure we have a symbol to trade
        if (symbol_.empty()) {
            logger_->error("No symbol specified for BollingerBandsStrategy");
            return false;
        }
        
        logger_->info("Initialized BollingerBandsStrategy");
        return true;
    }

    void on_bar(const std::string& data_name, const data::BarStruct& bar) override {
        // Only process data for our symbol
        if (data_name != symbol_) {
            return;
        }
        
        // Store the closing price
        prices_.push_back(bar.close);
        
        // Keep only enough data for our calculations
        if (prices_.size() > period_) {
            prices_.pop_front();
        }
        
        // Update the bands if we have enough data
        if (prices_.size() == period_) {
            update_bands();
        }
    }

protected:
    void next() override {
        // Wait until we have enough data
        if (prices_.size() < period_) {
            return;
        }
        
        // Get the most recent price
        double current_price = prices_.back();
        
        // Check if price is outside the bands
        if (current_price < lower_band_ && !position_open_) {
            // Buy signal
            buy(symbol_, 100);
            position_open_ = true;
            logger_->info("Buy signal: price {} below lower band {}", current_price, lower_band_);
        }
        else if (current_price > upper_band_ && position_open_) {
            // Sell signal
            sell(symbol_, 100);
            position_open_ = false;
            logger_->info("Sell signal: price {} above upper band {}", current_price, upper_band_);
        }
    }

private:
    // Strategy parameters
    int period_ = 20;
    double num_std_dev_ = 2.0;
    std::string symbol_;
    
    // Data storage
    std::deque<double> prices_;
    double sma_ = 0.0;
    double std_dev_ = 0.0;
    double upper_band_ = 0.0;
    double lower_band_ = 0.0;
    bool position_open_ = false;
    
    // Calculate Simple Moving Average
    double calculate_sma() const {
        double sum = 0.0;
        for (const auto& price : prices_) {
            sum += price;
        }
        return sum / prices_.size();
    }
    
    // Calculate Standard Deviation
    double calculate_std_dev(double mean) const {
        double sum_sq_diff = 0.0;
        for (const auto& price : prices_) {
            double diff = price - mean;
            sum_sq_diff += diff * diff;
        }
        return std::sqrt(sum_sq_diff / prices_.size());
    }
    
    // Update the Bollinger Bands
    void update_bands() {
        sma_ = calculate_sma();
        std_dev_ = calculate_std_dev(sma_);
        
        upper_band_ = sma_ + (num_std_dev_ * std_dev_);
        lower_band_ = sma_ - (num_std_dev_ * std_dev_);
    }
};

// Register the strategy with the factory
bool register_bollinger_strategy() {
    return strategy::CustomStrategyHelper::register_strategy<BollingerBandsStrategy>("bollinger_bands");
}

} // namespace examples
} // namespace quanttrader