#include "performance_observer.h"
#include "basic/time/time_with_zone.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <limits>

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
        open_trades_[symbol] = {time, symbol, quantity, new_qty > 0, price, price, time};
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
                              it_trade->second.entry_price, price, dd * std::abs(prev_qty), 
                              it_trade->second.worst_price_time, profit, time - it_trade->second.entry_time};
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

void PerformanceObserver::update_market_value(uint64_t time, const std::map<std::string, double>& prices) {
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
                            it_trade->second.worst_price_time = time;
                        }
                    } else {
                        if (it->second > it_trade->second.worst_price) {
                            it_trade->second.worst_price = it->second;
                            it_trade->second.worst_price_time = time;
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
        double total_profit = account_info.equity - starting_cash_;
        double profit_percentage = (total_profit / starting_cash_) * 100.0;
        double realized_percentage = (account_info.realized_pnl / starting_cash_) * 100.0;
        
        // Calculate trade statistics
        int total_trades = completed_trades_.size();
        int winning_trades = 0;
        int losing_trades = 0;
        double gross_profit = 0.0;
        double gross_loss = 0.0;
        double total_duration = 0.0;
        uint64_t max_dd_time = 0;
        
        for (const auto& trade : completed_trades_) {
            if (trade.profit > 0) {
                winning_trades++;
                gross_profit += trade.profit;
            } else if (trade.profit < 0) {
                losing_trades++;
                gross_loss += std::abs(trade.profit);
            }
            total_duration += trade.duration;
        }
        
        double win_rate = total_trades > 0 ? (static_cast<double>(winning_trades) / total_trades) * 100.0 : 0.0;
        double avg_trade_profit = total_trades > 0 ? (gross_profit - gross_loss) / total_trades : 0.0;
        double profit_factor = gross_loss > 0 ? gross_profit / gross_loss : (gross_profit > 0 ? std::numeric_limits<double>::infinity() : 0.0);
        double avg_duration = total_trades > 0 ? total_duration / total_trades : 0.0;
        double return_dd_ratio = max_drawdown_ > 0 ? total_profit / max_drawdown_ : std::numeric_limits<double>::infinity();
        
        // TODO: Find actual max drawdown time from historical equity data
        // For now, we'll indicate it's not available from broker data alone
        
        logger_->info("===== PORTFOLIO SUMMARY =====");
        logger_->info("Starting cash: ${:.2f}", starting_cash_);
        logger_->info("Ending equity: ${:.2f}", account_info.equity);
        logger_->info("Cash: ${:.2f}", account_info.cash);
        logger_->info("Total profit: ${:.2f} ({:.2f}%)", total_profit, profit_percentage);
        logger_->info("Realized P&L: ${:.2f} ({:.2f}%)", account_info.realized_pnl, realized_percentage);
        logger_->info("Unrealized P&L: ${:.2f}", account_info.unrealized_pnl);
        logger_->info("Max drawdown: ${:.2f} ({:.2f}%)", max_drawdown_, (max_drawdown_ / starting_cash_) * 100.0);
        logger_->info("Max DD time: N/A (requires historical equity tracking)");
        
        logger_->info("===== TRADE STATISTICS =====");
        logger_->info("Total trades: {}", total_trades);
        logger_->info("Winning trades: {} ({:.1f}%)", winning_trades, win_rate);
        logger_->info("Losing trades: {} ({:.1f}%)", losing_trades, 100.0 - win_rate);
        logger_->info("Gross profit: ${:.2f}", gross_profit);
        logger_->info("Gross loss: ${:.2f}", gross_loss);
        logger_->info("Average trade P&L: ${:.2f}", avg_trade_profit);
        if (std::isfinite(profit_factor)) {
            logger_->info("Profit factor: {:.2f}", profit_factor);
        } else {
            logger_->info("Profit factor: ∞ (no losing trades)");
        }
        if (std::isfinite(return_dd_ratio)) {
            logger_->info("Return/DD ratio: {:.2f}", return_dd_ratio);
        } else {
            logger_->info("Return/DD ratio: ∞ (no drawdown)");
        }
        logger_->info("Average trade duration: {}", format_duration(static_cast<uint64_t>(avg_duration)));
        logger_->info("");
        
        // Report positions
        auto positions = broker_->get_all_positions();
        logger_->info("===== CURRENT POSITIONS =====");
        if (positions.empty() || std::all_of(positions.begin(), positions.end(), 
                                            [](const auto& p) { return p.second.quantity == 0; })) {
            logger_->info("No open positions");
        } else {
            for (const auto& [symbol, position] : positions) {
                if (position.quantity != 0) {
                    logger_->info("  {} {} @ ${:.2f} (P&L: ${:.2f})", 
                                 symbol, position.quantity, position.avg_price, 
                                 position.realized_pnl + position.unrealized_pnl);
                }
            }
        }
        logger_->info("");
        
        // Report completed trades (round trips)
        logger_->info("===== COMPLETED TRADES =====");
        logger_->info("Total completed trades: {} round trips", completed_trades_.size());
        if (completed_trades_.empty()) {
            logger_->info("  No completed trades");
        } else {
            for (size_t i = 0; i < completed_trades_.size(); ++i) {
                const auto& trade = completed_trades_[i];
                double return_pct = (trade.profit / (trade.entry_price * trade.quantity)) * 100.0;
                
                logger_->info("  === Trade #{} ({}) ===", i + 1, trade.symbol);
                logger_->info("    Direction: {} {} shares", trade.is_long ? "LONG" : "SHORT", trade.quantity);
                logger_->info("    Entry:     {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(trade.entry_time), trade.entry_price, 
                             trade.entry_price * trade.quantity);
                logger_->info("    Exit:      {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(trade.exit_time), trade.exit_price, 
                             trade.exit_price * trade.quantity);
                logger_->info("    Profit:    {} ${:.2f} ({:.2f}%)", 
                             trade.profit >= 0 ? "+" : "", trade.profit, return_pct);
                logger_->info("    Duration:  {}", format_duration(trade.duration));
                if (trade.max_drawdown > 0) {
                    logger_->info("    Max DD:    ${:.2f} on {}", trade.max_drawdown, 
                                 format_timestamp(trade.max_drawdown_time));
                } else if (trade.max_drawdown_time == 0) {
                    logger_->info("    Max DD:    N/A (profitable trade, no intra-trade data)");
                } else {
                    logger_->info("    Max DD:    $0.00 (no drawdown detected)");
                }
                logger_->info("");
            }
        }
        
        // Report open trades (if any)
        if (!open_trades_.empty()) {
            logger_->info("===== OPEN TRADES =====");
            logger_->info("Total open trades: {} positions", open_trades_.size());
            size_t trade_num = completed_trades_.size() + 1;
            for (const auto& [symbol, trade] : open_trades_) {
                auto broker_positions = broker_->get_all_positions();
                auto pos_it = broker_positions.find(symbol);
                double current_value = pos_it != broker_positions.end() ? 
                    pos_it->second.quantity * pos_it->second.avg_price : 0.0;
                double unrealized_pnl = pos_it != broker_positions.end() ? 
                    pos_it->second.unrealized_pnl : 0.0;
                double current_price = pos_it != broker_positions.end() ? 
                    pos_it->second.avg_price : trade.entry_price;
                
                logger_->info("  === Open Trade #{} ({}) ===", trade_num, symbol);
                logger_->info("    Direction: {} {} shares", trade.is_long ? "LONG" : "SHORT", trade.quantity);
                logger_->info("    Entry:     {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(trade.entry_time), trade.entry_price, 
                             trade.entry_price * trade.quantity);
                logger_->info("    Current:   ${:.2f} (Amount: ${:.2f})", 
                             current_price, current_value);
                logger_->info("    Unrealized P&L: {} ${:.2f}", 
                             unrealized_pnl >= 0 ? "+" : "", unrealized_pnl);
                logger_->info("    Worst Price: ${:.2f} on {}", trade.worst_price, 
                             format_timestamp(trade.worst_price_time));
                logger_->info("");
                trade_num++;
            }
        }
        
    } else {
        // Legacy reporting
        double total_profit = equity_ - starting_cash_;
        double profit_percentage = (total_profit / starting_cash_) * 100.0;
        
        // Calculate trade statistics
        int total_trades = completed_trades_.size();
        int winning_trades = 0;
        int losing_trades = 0;
        double total_duration = 0.0;
        
        for (const auto& trade : completed_trades_) {
            if (trade.profit > 0) {
                winning_trades++;
            } else if (trade.profit < 0) {
                losing_trades++;
            }
            total_duration += trade.duration;
        }
        
        double win_rate = total_trades > 0 ? (static_cast<double>(winning_trades) / total_trades) * 100.0 : 0.0;
        double avg_trade_profit = total_trades > 0 ? (gross_profit_ - gross_loss_) / total_trades : 0.0;
        double profit_factor = gross_loss_ > 0 ? gross_profit_ / gross_loss_ : (gross_profit_ > 0 ? std::numeric_limits<double>::infinity() : 0.0);
        double avg_duration = total_trades > 0 ? total_duration / total_trades : 0.0;
        double return_dd_ratio = max_drawdown_ > 0 ? total_profit / max_drawdown_ : std::numeric_limits<double>::infinity();
        
        logger_->info("===== PORTFOLIO SUMMARY =====");
        logger_->info("Starting cash: ${:.2f}", starting_cash_);
        logger_->info("Ending equity: ${:.2f}", equity_);
        logger_->info("Total profit: ${:.2f} ({:.2f}%)", total_profit, profit_percentage);
        logger_->info("Gross profit: ${:.2f}", gross_profit_);
        logger_->info("Gross loss: ${:.2f}", gross_loss_);
        logger_->info("Max drawdown: ${:.2f} ({:.2f}%)", max_drawdown_, (max_drawdown_ / starting_cash_) * 100.0);
        logger_->info("Max DD time: N/A (requires historical equity tracking)");
        
        logger_->info("===== TRADE STATISTICS =====");
        logger_->info("Total trades: {}", total_trades);
        logger_->info("Winning trades: {} ({:.1f}%)", winning_trades, win_rate);
        logger_->info("Losing trades: {} ({:.1f}%)", losing_trades, 100.0 - win_rate);
        if (std::isfinite(profit_factor)) {
            logger_->info("Profit factor: {:.2f}", profit_factor);
        } else {
            logger_->info("Profit factor: ∞ (no losing trades)");
        }
        logger_->info("Average trade P&L: ${:.2f}", avg_trade_profit);
        if (std::isfinite(return_dd_ratio)) {
            logger_->info("Return/DD ratio: {:.2f}", return_dd_ratio);
        } else {
            logger_->info("Return/DD ratio: ∞ (no drawdown)");
        }
        logger_->info("Average trade duration: {}", format_duration(static_cast<uint64_t>(avg_duration)));
        logger_->info("");
        
        logger_->info("===== COMPLETED TRADES =====");
        logger_->info("Total completed trades: {} round trips", completed_trades_.size());
        if (completed_trades_.empty()) {
            logger_->info("  No completed trades");
        } else {
            for (size_t i = 0; i < completed_trades_.size(); ++i) {
                const auto& ct = completed_trades_[i];
                double return_pct = (ct.profit / (ct.entry_price * ct.quantity)) * 100.0;
                
                logger_->info("  === Trade #{} ({}) ===", i + 1, ct.symbol);
                logger_->info("    Direction: {} {} shares", ct.is_long ? "LONG" : "SHORT", ct.quantity);
                logger_->info("    Entry:     {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(ct.entry_time), ct.entry_price, 
                             ct.entry_price * ct.quantity);
                logger_->info("    Exit:      {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(ct.exit_time), ct.exit_price, 
                             ct.exit_price * ct.quantity);
                logger_->info("    Profit:    {} ${:.2f} ({:.2f}%)", 
                             ct.profit >= 0 ? "+" : "", ct.profit, return_pct);
                logger_->info("    Duration:  {}", format_duration(ct.duration));
                if (ct.max_drawdown > 0) {
                    logger_->info("    Max DD:    ${:.2f} on {}", ct.max_drawdown, 
                                 format_timestamp(ct.max_drawdown_time));
                } else if (ct.max_drawdown_time == 0) {
                    logger_->info("    Max DD:    N/A (profitable trade, no intra-trade data)");
                } else {
                    logger_->info("    Max DD:    $0.00 (no drawdown detected)");
                }
                logger_->info("");
            }
        }
        
        // Report open trades (if any)
        if (!open_trades_.empty()) {
            logger_->info("===== OPEN TRADES =====");
            logger_->info("Total open trades: {} positions", open_trades_.size());
            size_t trade_num = completed_trades_.size() + 1;
            for (const auto& [symbol, trade] : open_trades_) {
                // For legacy mode, we don't have current market prices, so use entry price
                double unrealized_pnl = 0.0; // We can't calculate this without current market data
                
                logger_->info("  === Open Trade #{} ({}) ===", trade_num, symbol);
                logger_->info("    Direction: {} {} shares", trade.is_long ? "LONG" : "SHORT", trade.quantity);
                logger_->info("    Entry:     {} @ ${:.2f} (Amount: ${:.2f})", 
                             format_timestamp(trade.entry_time), trade.entry_price, 
                             trade.entry_price * trade.quantity);
                logger_->info("    Current:   No current market data available");
                logger_->info("    Unrealized P&L: Not available (no current price)");
                logger_->info("    Worst Price: ${:.2f} on {}", trade.worst_price, 
                             format_timestamp(trade.worst_price_time));
                logger_->info("");
                trade_num++;
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
                    
                    // For broker-reconstructed trades, calculate basic max DD from entry/exit
                    if (ct.profit < 0) {
                        // If the trade was unprofitable, the max DD is at least the final loss
                        ct.max_drawdown = std::abs(ct.profit);
                        ct.max_drawdown_time = ct.exit_time; // Worst case at exit
                    } else {
                        // Profitable trade - no drawdown information available from just entry/exit
                        ct.max_drawdown = 0.0;
                        ct.max_drawdown_time = 0;
                    }
                    
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
                    
                    // For broker-reconstructed trades, calculate basic max DD from entry/exit
                    if (ct.profit < 0) {
                        // If the trade was unprofitable, the max DD is at least the final loss
                        ct.max_drawdown = std::abs(ct.profit);
                        ct.max_drawdown_time = ct.exit_time; // Worst case at exit
                    } else {
                        // Profitable trade - no drawdown information available from just entry/exit
                        ct.max_drawdown = 0.0;
                        ct.max_drawdown_time = 0;
                    }
                    
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
            ot.worst_price_time = long_entry.time; // Set to entry time as default
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
            ot.worst_price_time = short_entry.time; // Set to entry time as default
            open_trades_[symbol] = ot;
        }
    }
}

std::string PerformanceObserver::format_duration(uint64_t duration_ms) const {
    if (duration_ms == 0) {
        return "0ms";
    }
    
    // Use std::chrono for more precise duration handling
    auto duration = std::chrono::milliseconds(duration_ms);
    auto days = std::chrono::duration_cast<std::chrono::hours>(duration) / 24;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration) % 24;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) % 60;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) % 60;
    auto ms = duration % std::chrono::seconds(1);
    
    std::ostringstream oss;
    
    if (days.count() > 0) {
        oss << days.count() << "d";
        if (hours.count() > 0) {
            oss << " " << hours.count() << "h";
        }
    } else if (hours.count() > 0) {
        oss << hours.count() << "h";
        if (minutes.count() > 0) {
            oss << " " << minutes.count() << "m";
        }
    } else if (minutes.count() > 0) {
        oss << minutes.count() << "m";
        if (seconds.count() > 0) {
            oss << " " << seconds.count() << "s";
        }
    } else if (seconds.count() > 0) {
        oss << seconds.count() << "s";
        if (ms.count() > 0) {
            oss << " " << ms.count() << "ms";
        }
    } else {
        oss << ms.count() << "ms";
    }
    
    return oss.str();
}

std::string PerformanceObserver::format_timestamp(uint64_t timestamp_ms) const {
    if (timestamp_ms == 0) {
        return "N/A";
    }
    
    // Convert milliseconds to nanoseconds for TimeWithZone
    uint64_t timestamp_ns = timestamp_ms * 1000000ULL;
    
    try {
        // Use the current system timezone or fall back to UTC
        auto time_with_zone = quanttrader::time::TimeWithZone(timestamp_ns, "");
        return time_with_zone.to_string();
    } catch (const std::exception& e) {
        // Fallback to simple formatting if TimeWithZone fails
        std::time_t timestamp_sec = timestamp_ms / 1000;
        std::tm tm_local;
        
#ifdef _WIN32
        if (localtime_s(&tm_local, &timestamp_sec) != 0) {
            return "Invalid time";
        }
#else
        if (localtime_r(&timestamp_sec, &tm_local) == nullptr) {
            return "Invalid time";
        }
#endif
        
        std::ostringstream oss;
        oss << std::put_time(&tm_local, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}

} // namespace observer
} // namespace quanttrader
