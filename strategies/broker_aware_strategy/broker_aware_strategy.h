#pragma once

#include "strategy/strategy_base.h"
#include "broker/abstract_broker.h"

namespace quanttrader {
namespace strategy {

/**
 * @brief Example strategy demonstrating broker integration
 * 
 * This strategy shows how to use the new broker-integrated trading system
 * with proper long and short position management.
 */
class BrokerAwareStrategy : public StrategyBase {
public:
    explicit BrokerAwareStrategy(StrategyCreateFuncParemType params);
    ~BrokerAwareStrategy() override = default;

    // Override broker event handlers
    void on_order_status(const broker::Order& order) override;
    void on_trade_execution(const broker::Trade& trade) override;
    void on_position_update(const broker::Position& position) override;
    void on_account_update(const broker::AccountInfo& account) override;

protected:
    void next() override;

private:
    // Strategy parameters
    double entry_threshold_ = 0.02;  // 2% move to enter
    double exit_threshold_ = 0.01;   // 1% move to exit
    double stop_loss_ = 0.05;        // 5% stop loss
    double position_size_ = 100.0;   // Base position size
    bool allow_short_ = true;        // Allow short positions
    
    // Strategy state
    bool in_position_ = false;
    double last_price_ = 0.0;
    double entry_price_ = 0.0;
    bool is_long_position_ = false;
    
    // Helper methods
    void check_entry_signals();
    void check_exit_signals();
    void manage_risk();
    double calculate_position_size();
};

} // namespace strategy
} // namespace quanttrader
