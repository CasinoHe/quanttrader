#pragma once

#include "cerebro_base.h"
#include "broker/broker_factory.h"

namespace quanttrader {
namespace cerebro {

/**
 * @brief Enhanced cerebro implementation with broker integration
 * 
 * This cerebro implementation provides comprehensive trading functionality
 * with broker integration for both backtesting and live trading.
 */
class EnhancedCerebro : public CerebroBase {
public:
    explicit EnhancedCerebro(const std::string_view name = "EnhancedCerebro", double starting_cash = 100000.0);
    ~EnhancedCerebro() override = default;

    // Override run method to provide complete trading functionality
    bool run() override;
    
    // Broker configuration
    bool set_broker_type(const std::string& broker_type, double starting_cash = 0.0);
    bool configure_backtest_broker(double commission = 0.0, double slippage = 0.0, 
                                  double initial_margin = 0.0, double maintenance_margin = 0.0);
    
    // Override prepare to set up broker integration
    bool prepare() override;
    
    // Enhanced stop method
    bool stop() override;
    
    // Get performance statistics
    void print_performance_report() const;
    
private:
    std::string broker_type_ = "backtest";
    double broker_starting_cash_ = 100000.0;
    
    // Backtesting parameters
    double commission_per_trade_ = 0.0;
    double slippage_percent_ = 0.0;
    double initial_margin_percent_ = 0.0;
    double maintenance_margin_percent_ = 0.0;
};

} // namespace cerebro
} // namespace quanttrader
