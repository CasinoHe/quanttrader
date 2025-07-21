#include "performance_observer.h"
#include <iostream>
#include <algorithm>

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
    
    // Reconstruct completed trades from broker trade history
    reconstruct_completed_trades_from_broker();
    
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
        
        // Report completed trades (round trips)
        logger_->info("Completed trades ({} round trips):", completed_trades_.size());
        if (completed_trades_.empty()) {
            logger_->info("  No completed trades");
        } else {
            for (const auto& trade : completed_trades_) {
                double return_pct = (trade.profit / (trade.entry_price * trade.quantity)) * 100.0;
                logger_->info("  {} {} {} shares: Entry {:.2f} -> Exit {:.2f} = {} {:.2f} ({:.2f}%) [Duration: {}ms]",
                             trade.symbol,
                             trade.is_long ? "LONG" : "SHORT",
                             trade.quantity,
                             trade.entry_price,
                             trade.exit_price,
                             trade.profit >= 0 ? "Profit" : "Loss",
                             trade.profit,
                             return_pct,
                             trade.duration);
            }
        }
        
        // Report open trades (if any)
        if (!open_trades_.empty()) {
            logger_->info("Open trades ({} positions):", open_trades_.size());
            for (const auto& [symbol, trade] : open_trades_) {
                logger_->info("  {} {} {} shares @ {:.2f} (Unrealized worst: {:.2f})",
                             symbol,
                             trade.is_long ? "LONG" : "SHORT",
                             trade.quantity,
                             trade.entry_price,
                             trade.worst_price);
            }
        }
        
    } else {
        // Legacy reporting
        logger_->info("Starting cash: {}", starting_cash_);
        logger_->info("Ending equity: {}", equity_);
        logger_->info("Total profit: {}", equity_ - starting_cash_);
        logger_->info("Gross profit: {}", gross_profit_);
        logger_->info("Gross loss: {}", gross_loss_);
        logger_->info("Max drawdown: {}", max_drawdown_);
        
        logger_->info("Completed trades ({} round trips):", completed_trades_.size());
        if (completed_trades_.empty()) {
            logger_->info("  No completed trades");
        } else {
            for (const auto& ct : completed_trades_) {
                double return_pct = (ct.profit / (ct.entry_price * ct.quantity)) * 100.0;
                logger_->info("  {} {} {} shares: Entry {:.2f} -> Exit {:.2f} = {} {:.2f} ({:.2f}%) [Duration: {}ms, Max DD: {:.2f}]",
                             ct.symbol,
                             ct.is_long ? "LONG" : "SHORT",
                             ct.quantity,
                             ct.entry_price,
                             ct.exit_price,
                             ct.profit >= 0 ? "Profit" : "Loss",
                             ct.profit,
                             return_pct,
                             ct.duration,
                             ct.max_drawdown);
            }
        }
        
        // Report open trades (if any)
        if (!open_trades_.empty()) {
            logger_->info("Open trades ({} positions):", open_trades_.size());
            for (const auto& [symbol, trade] : open_trades_) {
                logger_->info("  {} {} {} shares @ {:.2f} (Unrealized worst: {:.2f})",
                             symbol,
                             trade.is_long ? "LONG" : "SHORT",
                             trade.quantity,
                             trade.entry_price,
                             trade.worst_price);
            }
        }
    }
}

void PerformanceObserver::reconstruct_completed_trades_from_broker() {
    // Clear existing completed trades when reconstructing from broker data
    completed_trades_.clear();
    open_trades_.clear();
    
    // Sort trades by timestamp to process them in chronological order
    auto sorted_trades = trades_;
    std::sort(sorted_trades.begin(), sorted_trades.end(), 
              [](const TradeRecord& a, const TradeRecord& b) {
                  return a.time < b.time;
              });
    
    // Track positions per symbol to match buy/sell pairs
    std::map<std::string, std::vector<TradeRecord>> symbol_trades;
    
    // Group trades by symbol
    for (const auto& trade : sorted_trades) {
        symbol_trades[trade.symbol].push_back(trade);
    }
    
    // Process each symbol's trades to create completed round trips
    for (auto& [symbol, symbol_trade_list] : symbol_trades) {
        std::vector<TradeRecord> long_entries;  // Buy orders waiting for sell
        std::vector<TradeRecord> short_entries; // Sell orders waiting for buy
        
        for (const auto& trade : symbol_trade_list) {
            if (trade.is_buy) {
                // This is a buy order
                if (!short_entries.empty()) {
                    // Match with existing short position (cover short)
                    auto short_entry = short_entries.front();
                    short_entries.erase(short_entries.begin());
                    
                    // Create completed trade for short position
                    CompletedTrade ct;
                    ct.entry_time = short_entry.time;
                    ct.exit_time = trade.time;
                    ct.symbol = symbol;
                    ct.quantity = std::min(trade.quantity, short_entry.quantity);
                    ct.is_long = false;  // This was a short trade
                    ct.entry_price = short_entry.price;
                    ct.exit_price = trade.price;
                    ct.profit = (short_entry.price - trade.price) * ct.quantity;
                    ct.duration = trade.time - short_entry.time;
                    ct.max_drawdown = 0.0; // We don't have enough data to calculate this from broker
                    
                    completed_trades_.push_back(ct);
                    
                    // Handle partial fills
                    if (trade.quantity > short_entry.quantity) {
                        TradeRecord remaining_buy = trade;
                        remaining_buy.quantity = trade.quantity - short_entry.quantity;
                        long_entries.push_back(remaining_buy);
                    } else if (short_entry.quantity > trade.quantity) {
                        TradeRecord remaining_short = short_entry;
                        remaining_short.quantity = short_entry.quantity - trade.quantity;
                        short_entries.insert(short_entries.begin(), remaining_short);
                    }
                } else {
                    // Open new long position
                    long_entries.push_back(trade);
                }
            } else {
                // This is a sell order
                if (!long_entries.empty()) {
                    // Match with existing long position (close long)
                    auto long_entry = long_entries.front();
                    long_entries.erase(long_entries.begin());
                    
                    // Create completed trade for long position
                    CompletedTrade ct;
                    ct.entry_time = long_entry.time;
                    ct.exit_time = trade.time;
                    ct.symbol = symbol;
                    ct.quantity = std::min(trade.quantity, long_entry.quantity);
                    ct.is_long = true;  // This was a long trade
                    ct.entry_price = long_entry.price;
                    ct.exit_price = trade.price;
                    ct.profit = (trade.price - long_entry.price) * ct.quantity;
                    ct.duration = trade.time - long_entry.time;
                    ct.max_drawdown = 0.0; // We don't have enough data to calculate this from broker
                    
                    completed_trades_.push_back(ct);
                    
                    // Handle partial fills
                    if (trade.quantity > long_entry.quantity) {
                        TradeRecord remaining_sell = trade;
                        remaining_sell.quantity = trade.quantity - long_entry.quantity;
                        short_entries.push_back(remaining_sell);
                    } else if (long_entry.quantity > trade.quantity) {
                        TradeRecord remaining_long = long_entry;
                        remaining_long.quantity = long_entry.quantity - trade.quantity;
                        long_entries.insert(long_entries.begin(), remaining_long);
                    }
                } else {
                    // Open new short position
                    short_entries.push_back(trade);
                }
            }
        }
        
        // Any remaining entries are still open positions
        for (const auto& long_entry : long_entries) {
            OpenTrade ot;
            ot.entry_time = long_entry.time;
            ot.symbol = symbol;
            ot.quantity = long_entry.quantity;
            ot.is_long = true;
            ot.entry_price = long_entry.price;
            ot.worst_price = long_entry.price; // We don't have historical worst price data
            open_trades_[symbol] = ot;
        }
        
        for (const auto& short_entry : short_entries) {
            OpenTrade ot;
            ot.entry_time = short_entry.time;
            ot.symbol = symbol;
            ot.quantity = short_entry.quantity;
            ot.is_long = false;
            ot.entry_price = short_entry.price;
            ot.worst_price = short_entry.price; // We don't have historical worst price data
            open_trades_[symbol] = ot;
        }
    }
}

} // namespace observer
} // namespace quanttrader
