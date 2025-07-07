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

void PerformanceObserver::report() const {
    logger_->info("===== Performance Report =====");
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

} // namespace observer
} // namespace quanttrader
