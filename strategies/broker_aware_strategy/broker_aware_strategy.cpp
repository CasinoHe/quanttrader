#include "broker_aware_strategy.h"
#include "data/common/data_struct.h"

namespace quanttrader {
namespace strategy {

BrokerAwareStrategy::BrokerAwareStrategy(StrategyCreateFuncParemType params) 
    : StrategyBase(params) {
    strategy_name_ = "BrokerAwareStrategy";
    
    // Extract strategy parameters
    entry_threshold_ = get_param<double>("entry_threshold", 0.02);
    exit_threshold_ = get_param<double>("exit_threshold", 0.01);
    stop_loss_ = get_param<double>("stop_loss", 0.05);
    position_size_ = get_param<double>("position_size", 100.0);
    allow_short_ = get_param<bool>("allow_short", true);
    
    logger_->info("Created BrokerAwareStrategy with parameters: entry={}, exit={}, stop_loss={}, size={}, allow_short={}",
                 entry_threshold_, exit_threshold_, stop_loss_, position_size_, allow_short_);
}

void BrokerAwareStrategy::on_order_status(const broker::Order& order) {
    logger_->info("Order Status Update: {} {} {} {} @ {} - Status: {}",
                 order.order_id, order.symbol,
                 (order.side == broker::OrderSide::BUY) ? "BUY" : "SELL",
                 order.quantity, order.price,
                 static_cast<int>(order.status));
}

void BrokerAwareStrategy::on_trade_execution(const broker::Trade& trade) {
    logger_->info("Trade Executed: {} {} {} @ {} (Order: {})",
                 trade.symbol,
                 (trade.side == broker::OrderSide::BUY) ? "BUY" : "SELL",
                 trade.quantity, trade.price, trade.order_id);
    
    // Update strategy state based on trade
    if (trade.symbol == symbol_) {
        entry_price_ = trade.price;
        is_long_position_ = (trade.side == broker::OrderSide::BUY);
        in_position_ = true;
    }
}

void BrokerAwareStrategy::on_position_update(const broker::Position& position) {
    if (position.symbol == symbol_) {
        logger_->info("Position Update: {} {} shares @ {} (P&L: {:.2f})",
                     position.symbol, position.quantity, position.avg_price,
                     position.realized_pnl + position.unrealized_pnl);
        
        // Update strategy state
        in_position_ = !position.is_flat();
        if (in_position_) {
            is_long_position_ = position.is_long();
            entry_price_ = position.avg_price;
        }
    }
}

void BrokerAwareStrategy::on_account_update(const broker::AccountInfo& account) {
    logger_->info("Account Update: Cash={:.2f}, Equity={:.2f}, P&L={:.2f}",
                 account.cash, account.equity, account.realized_pnl + account.unrealized_pnl);
}

void BrokerAwareStrategy::next() {
    if (symbol_.empty()) {
        logger_->warn("No symbol specified for strategy");
        return;
    }
    
    // Get current price
    if (last_prices_.find(symbol_) != last_prices_.end()) {
        last_price_ = last_prices_[symbol_];
    } else {
        logger_->warn("No price data available for {}", symbol_);
        return;
    }
    
    // Update position status from broker
    auto position = get_position(symbol_);
    in_position_ = !position.is_flat();
    
    if (in_position_) {
        is_long_position_ = position.is_long();
        entry_price_ = position.avg_price;
    }
    
    // Risk management first
    manage_risk();
    
    // Check signals
    if (!in_position_) {
        check_entry_signals();
    } else {
        check_exit_signals();
    }
}

void BrokerAwareStrategy::check_entry_signals() {
    if (last_price_ <= 0) {
        return;
    }
    
    // Simple momentum-based entry
    // This is a placeholder - real strategies would have more sophisticated logic
    static double previous_price = 0.0;
    
    if (previous_price > 0) {
        double price_change = (last_price_ - previous_price) / previous_price;
        
        if (price_change > entry_threshold_) {
            // Strong upward movement - go long
            double qty = calculate_position_size();
            long order_id = buy(symbol_, qty);
            logger_->info("LONG ENTRY SIGNAL: {} shares of {} at {:.2f} (Order: {})",
                         qty, symbol_, last_price_, order_id);
        } else if (allow_short_ && price_change < -entry_threshold_) {
            // Strong downward movement - go short
            double qty = calculate_position_size();
            long order_id = sell(symbol_, qty);
            logger_->info("SHORT ENTRY SIGNAL: {} shares of {} at {:.2f} (Order: {})",
                         qty, symbol_, last_price_, order_id);
        }
    }
    
    previous_price = last_price_;
}

void BrokerAwareStrategy::check_exit_signals() {
    if (entry_price_ <= 0 || last_price_ <= 0) {
        return;
    }
    
    double price_change = (last_price_ - entry_price_) / entry_price_;
    
    bool should_exit = false;
    std::string reason;
    
    if (is_long_position_) {
        // Long position exit conditions
        if (price_change > exit_threshold_) {
            should_exit = true;
            reason = "profit target";
        } else if (price_change < -stop_loss_) {
            should_exit = true;
            reason = "stop loss";
        }
    } else {
        // Short position exit conditions
        if (price_change < -exit_threshold_) {
            should_exit = true;
            reason = "profit target";
        } else if (price_change > stop_loss_) {
            should_exit = true;
            reason = "stop loss";
        }
    }
    
    if (should_exit) {
        bool success = close_position(symbol_);
        logger_->info("EXIT SIGNAL ({}): Closing {} position in {} at {:.2f} - {}",
                     reason, is_long_position_ ? "LONG" : "SHORT", 
                     symbol_, last_price_, success ? "SUCCESS" : "FAILED");
    }
}

void BrokerAwareStrategy::manage_risk() {
    // Check account health
    auto account = get_account_info();
    
    // Simple risk management: close all positions if losses exceed threshold
    double total_pnl = account.realized_pnl + account.unrealized_pnl;
    double max_loss_threshold = account.cash * 0.10; // 10% of starting cash
    
    if (total_pnl < -max_loss_threshold) {
        logger_->warn("Risk management: Total P&L {:.2f} exceeds loss threshold {:.2f}",
                     total_pnl, -max_loss_threshold);
        
        // Close all positions
        auto positions = get_all_positions();
        for (const auto& [symbol, position] : positions) {
            if (!position.is_flat()) {
                close_position(symbol);
                logger_->info("Risk management: Closing position in {}", symbol);
            }
        }
    }
}

double BrokerAwareStrategy::calculate_position_size() {
    // Simple position sizing based on available cash
    auto account = get_account_info();
    double available_cash = account.cash * 0.95; // Use 95% of available cash
    
    if (last_price_ > 0) {
        double max_shares = available_cash / last_price_;
        return std::min(position_size_, max_shares);
    }
    
    return position_size_;
}

} // namespace strategy
} // namespace quanttrader
