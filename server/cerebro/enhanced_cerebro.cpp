#include "enhanced_cerebro.h"
#include "broker/backtest_broker.h"
#include <chrono>
#include <thread>

namespace quanttrader {
namespace cerebro {

EnhancedCerebro::EnhancedCerebro(const std::string_view name, double starting_cash) 
    : CerebroBase(name), broker_starting_cash_(starting_cash) {
    logger_->info("Created EnhancedCerebro: {} with starting cash: {}", name, starting_cash);
}

bool EnhancedCerebro::run() {
    if (!prepare()) {
        logger_->error("{} Failed to prepare for execution", name_);
        return false;
    }
    
    is_running_ = true;
    logger_->info("{} Starting execution...", name_);
    
    // Initialize all strategies
    for (auto& strategy : strategies_) {
        if (!strategy->on_start()) {
            logger_->error("{} Failed to start strategy: {}", name_, strategy->get_name());
            return false;
        }
    }
    
    // Main execution loop
    while (is_running_ && !stop_flag_.load()) {
        if (!process_next()) {
            logger_->info("{} No more data to process, execution complete", name_);
            break;
        }
        
        // Small delay to prevent tight loop in real-time scenarios
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    // Stop all strategies
    for (auto& strategy : strategies_) {
        strategy->on_stop();
    }
    
    // Generate performance report
    print_performance_report();
    
    is_running_ = false;
    logger_->info("{} Execution completed", name_);
    return true;
}

bool EnhancedCerebro::set_broker_type(const std::string& broker_type, double starting_cash) {
    if (is_running_) {
        logger_->error("{} Cannot change broker type while running", name_);
        return false;
    }
    
    broker_type_ = broker_type;
    if (starting_cash > 0) {
        broker_starting_cash_ = starting_cash;
    }
    
    logger_->info("{} Set broker type to: {} with starting cash: {}", 
                 name_, broker_type_, broker_starting_cash_);
    return true;
}

bool EnhancedCerebro::configure_backtest_broker(double commission, double slippage, 
                                               double initial_margin, double maintenance_margin) {
    commission_per_trade_ = commission;
    slippage_percent_ = slippage;
    initial_margin_percent_ = initial_margin;
    maintenance_margin_percent_ = maintenance_margin;
    
    logger_->info("{} Configured backtest broker: commission={}, slippage={}%, initial_margin={}%, maintenance_margin={}%",
                 name_, commission, slippage, initial_margin, maintenance_margin);
    return true;
}

bool EnhancedCerebro::prepare() {
    // Call parent prepare first
    if (!CerebroBase::prepare()) {
        return false;
    }
    
    // Create and configure broker
    auto broker_factory = broker::BrokerFactory::instance();
    auto broker = broker_factory->create_broker(broker_type_, broker_starting_cash_);
    
    if (!broker) {
        logger_->error("{} Failed to create broker of type: {}", name_, broker_type_);
        return false;
    }
    
    // Configure backtest broker if applicable
    if (broker_type_ == "backtest") {
        auto backtest_broker = std::dynamic_pointer_cast<broker::BacktestBroker>(broker);
        if (backtest_broker) {
            backtest_broker->set_commission(commission_per_trade_);
            backtest_broker->set_slippage(slippage_percent_);
            backtest_broker->set_margin_requirements(initial_margin_percent_, maintenance_margin_percent_);
        }
    }
    
    // Set the broker
    set_broker(broker);
    
    // Connect observers to broker
    for (auto& observer : observers_) {
        auto performance_observer = std::dynamic_pointer_cast<observer::PerformanceObserver>(observer);
        if (performance_observer) {
            performance_observer->set_broker(broker);
        }
    }
    
    logger_->info("{} Successfully prepared with broker integration", name_);
    return true;
}

bool EnhancedCerebro::stop() {
    if (!is_running_) {
        return true;
    }
    
    stop_flag_.store(true);
    
    // Generate final performance report
    print_performance_report();
    
    // Call parent stop
    return CerebroBase::stop();
}

void EnhancedCerebro::print_performance_report() const {
    logger_->info("========== {} Performance Report ==========", name_);
    
    if (broker_) {
        auto account_info = broker_->get_account_info();
        logger_->info("Account Summary:");
        logger_->info("  Starting Cash: {:.2f}", broker_starting_cash_);
        logger_->info("  Current Cash: {:.2f}", account_info.cash);
        logger_->info("  Current Equity: {:.2f}", account_info.equity);
        logger_->info("  Total Return: {:.2f} ({:.2f}%)", 
                     account_info.equity - broker_starting_cash_,
                     ((account_info.equity - broker_starting_cash_) / broker_starting_cash_) * 100.0);
        logger_->info("  Realized P&L: {:.2f}", account_info.realized_pnl);
        logger_->info("  Unrealized P&L: {:.2f}", account_info.unrealized_pnl);
        logger_->info("  Buying Power: {:.2f}", account_info.buying_power);
        
        // Current positions
        auto positions = broker_->get_all_positions();
        logger_->info("Current Positions:");
        if (positions.empty()) {
            logger_->info("  No open positions");
        } else {
            for (const auto& [symbol, position] : positions) {
                if (position.quantity != 0) {
                    logger_->info("  {}: {} shares @ {:.2f} (P&L: {:.2f})",
                                 symbol, position.quantity, position.avg_price,
                                 position.realized_pnl + position.unrealized_pnl);
                }
            }
        }
        
        // Recent trades
        auto trades = broker_->get_trades();
        logger_->info("Trade History ({} trades):", trades.size());
        for (const auto& trade : trades) {
            logger_->info("  {} {} {:.0f} {} @ {:.2f} (Order: {})",
                         trade.symbol,
                         (trade.side == broker::OrderSide::BUY) ? "BUY" : "SELL",
                         trade.quantity,
                         trade.symbol,
                         trade.price,
                         trade.order_id);
        }
    }
    
    // Observer reports
    for (const auto& observer : observers_) {
        observer->report();
    }
    
    logger_->info("========== End Performance Report ==========");
}

} // namespace cerebro
} // namespace quanttrader
