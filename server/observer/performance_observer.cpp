#include "performance_observer.h"
#include <iostream>

namespace quanttrader {
namespace observer {

PerformanceObserver::PerformanceObserver(double starting_cash)
    : starting_cash_(starting_cash), cash_(starting_cash), equity_(starting_cash),
      peak_equity_(starting_cash), max_drawdown_(0.0) {
    logger_ = quanttrader::log::get_common_rotation_logger("PerformanceObserver", "observer");
}

void PerformanceObserver::record_trade(uint64_t time, const std::string& symbol, int quantity, double price, bool is_buy) {
    if (quantity <= 0)
        return;

    trades_.push_back({time, symbol, quantity, price, is_buy});

    int signed_qty = is_buy ? quantity : -quantity;
    int prev_qty = positions_[symbol];
    int new_qty = prev_qty + signed_qty;

    cash_ += is_buy ? -price * quantity : price * quantity;

    if (prev_qty == 0 && new_qty != 0) {
        // Opening a new position
        avg_price_[symbol] = price;
        open_trades_[symbol] = {time, symbol, quantity, new_qty > 0, price, price};
    } else if ((prev_qty > 0 && new_qty == 0) || (prev_qty < 0 && new_qty == 0)) {
        // Closing a position completely
        bool was_long = prev_qty > 0;
        double entry_price = avg_price_[symbol];
        double profit = was_long ? (price - entry_price) * std::abs(prev_qty)
                                : (entry_price - price) * std::abs(prev_qty);
        if (profit >= 0)
            gross_profit_ += profit;
        else
            gross_loss_ += -profit;

        auto it_trade = open_trades_.find(symbol);
        if (it_trade != open_trades_.end()) {
            double dd = was_long ? (it_trade->second.entry_price - it_trade->second.worst_price)
                                 : (it_trade->second.worst_price - it_trade->second.entry_price);
            CompletedTrade ct{it_trade->second.entry_time, time, symbol, std::abs(prev_qty), was_long,
                              it_trade->second.entry_price, price, dd * std::abs(prev_qty), profit,
                              time - it_trade->second.entry_time};
            completed_trades_.push_back(ct);
            open_trades_.erase(it_trade);
        }

        avg_price_.erase(symbol);
    } else if (prev_qty != 0 && ((prev_qty > 0) == (new_qty > 0))) {
        // Increasing existing position in same direction
        double prev_cost = avg_price_[symbol] * std::abs(prev_qty);
        avg_price_[symbol] = (prev_cost + price * quantity) / std::abs(new_qty);
    }

    positions_[symbol] = new_qty;
}

void PerformanceObserver::update_market_value(uint64_t /*time*/, const std::map<std::string, double>& prices) {
    // Update broker data if available
    if (broker_) {
        update_from_broker();
    } else {
        // Legacy behavior for backward compatibility
        double pos_value = 0.0;
        for (const auto& [symbol, qty] : positions_) {
            auto it = prices.find(symbol);
            if (it != prices.end()) {
                pos_value += qty * it->second;
                auto it_trade = open_trades_.find(symbol);
                if (it_trade != open_trades_.end()) {
                    if (it_trade->second.is_long) {
                        if (it->second < it_trade->second.worst_price) {
                            it_trade->second.worst_price = it->second;
                        }
                    } else {
                        if (it->second > it_trade->second.worst_price) {
                            it_trade->second.worst_price = it->second;
                        }
                    }
                }
            }
        }

        equity_ = cash_ + pos_value;
        if (equity_ > peak_equity_) {
            peak_equity_ = equity_;
        } else {
            double dd = peak_equity_ - equity_;
            if (dd > max_drawdown_) {
                max_drawdown_ = dd;
            }
        }
    }
}

void PerformanceObserver::update_from_broker() {
    if (!broker_) {
        return;
    }
    
    // Get current account information from broker
    auto account_info = broker_->get_account_info();
    cash_ = account_info.cash;
    equity_ = account_info.equity;
    
    // Update peak equity and max drawdown
    if (equity_ > peak_equity_) {
        peak_equity_ = equity_;
    } else {
        double dd = peak_equity_ - equity_;
        if (dd > max_drawdown_) {
            max_drawdown_ = dd;
        }
    }
    
    // Update positions from broker
    auto broker_positions = broker_->get_all_positions();
    positions_.clear();
    avg_price_.clear();
    
    for (const auto& [symbol, position] : broker_positions) {
        if (position.quantity != 0) {
            positions_[symbol] = static_cast<int>(position.quantity);
            avg_price_[symbol] = position.avg_price;
        }
    }
    
    // Update trades from broker
    auto broker_trades = broker_->get_trades();
    trades_.clear();
    
    for (const auto& trade : broker_trades) {
        TradeRecord record;
        record.time = trade.timestamp;
        record.symbol = trade.symbol;
        record.quantity = static_cast<int>(trade.quantity);
        record.price = trade.price;
        record.is_buy = (trade.side == broker::OrderSide::BUY);
        trades_.push_back(record);
    }
    
    // Update gross profit/loss from broker positions
    gross_profit_ = 0.0;
    gross_loss_ = 0.0;
    
    for (const auto& [symbol, position] : broker_positions) {
        if (position.realized_pnl > 0) {
            gross_profit_ += position.realized_pnl;
        } else if (position.realized_pnl < 0) {
            gross_loss_ += -position.realized_pnl;
        }
    }
}

void PerformanceObserver::report() const {
    logger_->info("===== Performance Report =====");
    
    if (broker_) {
        // Use broker data for reporting
        auto account_info = broker_->get_account_info();
        logger_->info("Starting cash: {}", starting_cash_);
        logger_->info("Ending equity: {}", account_info.equity);
        logger_->info("Cash: {}", account_info.cash);
        logger_->info("Total profit: {}", account_info.equity - starting_cash_);
        logger_->info("Realized P&L: {}", account_info.realized_pnl);
        logger_->info("Unrealized P&L: {}", account_info.unrealized_pnl);
        logger_->info("Max drawdown: {}", max_drawdown_);
        
        // Report positions
        auto positions = broker_->get_all_positions();
        logger_->info("Current positions:");
        for (const auto& [symbol, position] : positions) {
            if (position.quantity != 0) {
                logger_->info("  {} {} @ {} (P&L: {:.2f})", 
                             symbol, position.quantity, position.avg_price, 
                             position.realized_pnl + position.unrealized_pnl);
            }
        }
        
        // Report recent trades
        auto trades = broker_->get_trades();
        logger_->info("Recent trades:");
        for (const auto& trade : trades) {
            logger_->info("  {} {} {} @ {} (Order: {})", 
                         trade.symbol, 
                         (trade.side == broker::OrderSide::BUY) ? "BUY" : "SELL",
                         trade.quantity, trade.price, trade.order_id);
        }
        
    } else {
        // Legacy reporting
        logger_->info("Starting cash: {}", starting_cash_);
        logger_->info("Ending equity: {}", equity_);
        logger_->info("Total profit: {}", equity_ - starting_cash_);
        logger_->info("Gross profit: {}", gross_profit_);
        logger_->info("Gross loss: {}", gross_loss_);
        logger_->info("Max drawdown: {}", max_drawdown_);
        
        for (const auto& ct : completed_trades_) {
            logger_->info("Trade {} {} {}->{} qty {} profit {:.2f} dd {:.2f} duration {}",
                          ct.symbol, ct.is_long ? "LONG" : "SHORT", ct.entry_price, ct.exit_price,
                          ct.quantity, ct.profit, ct.max_drawdown, ct.duration);
        }
    }
}

} // namespace observer
} // namespace quanttrader
