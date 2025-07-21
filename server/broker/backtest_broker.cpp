#include "backtest_broker.h"
#include <algorithm>
#include <cmath>

namespace quanttrader {
namespace broker {

BacktestBroker::BacktestBroker(double starting_cash) : AbstractBroker(starting_cash) {
    logger_ = quanttrader::log::get_common_rotation_logger("BacktestBroker", "broker");
    logger_->info("Initialized BacktestBroker with starting cash: {}", starting_cash);
}

void BacktestBroker::process_market_data(uint64_t timestamp, const std::map<std::string, double>& prices) {
    current_timestamp_ = timestamp;
    
    // Update market prices
    update_market_prices(prices);
    
    // Process pending orders
    process_pending_orders(timestamp, prices);
    
    // Check margin requirements
    check_margin_requirements();
}

long BacktestBroker::place_order(const std::string& symbol, OrderSide side, OrderType type, 
                                double quantity, double price, double stop_price) {
    // Call parent implementation to create the order
    long order_id = AbstractBroker::place_order(symbol, side, type, quantity, price, stop_price);
    
    if (order_id > 0) {
        // For market orders, try to fill immediately if we have current price
        if (type == OrderType::MARKET) {
            double current_price = get_last_price(symbol);
            if (current_price > 0) {
                auto order_opt = get_order(order_id);
                if (order_opt.has_value() && order_opt->status == OrderStatus::PENDING) {
                    double fill_price = calculate_fill_price(order_opt.value(), current_price);
                    process_order_fill(order_id, quantity, fill_price, current_timestamp_);
                }
            }
        }
    }
    
    return order_id;
}

void BacktestBroker::process_pending_orders(uint64_t timestamp, const std::map<std::string, double>& prices) {
    std::vector<long> orders_to_fill;
    
    for (const auto& [order_id, order] : orders_) {
        if (order.status != OrderStatus::PENDING && order.status != OrderStatus::PARTIALLY_FILLED) {
            continue;
        }
        
        auto price_it = prices.find(order.symbol);
        if (price_it == prices.end()) {
            continue;
        }
        
        double current_price = price_it->second;
        
        if (should_fill_order(order, current_price)) {
            double fill_price = calculate_fill_price(order, current_price);
            double fill_quantity = order.remaining_quantity;
            
            orders_to_fill.push_back(order_id);
            
            // Process the fill
            process_order_fill(order_id, fill_quantity, fill_price, timestamp);
        }
    }
}

bool BacktestBroker::should_fill_order(const Order& order, double current_price) const {
    switch (order.type) {
        case OrderType::MARKET:
            return true;
            
        case OrderType::LIMIT:
            if (order.side == OrderSide::BUY) {
                return current_price <= order.price;
            } else {
                return current_price >= order.price;
            }
            
        case OrderType::STOP:
            if (order.side == OrderSide::BUY) {
                return current_price >= order.stop_price;
            } else {
                return current_price <= order.stop_price;
            }
            
        case OrderType::STOP_LIMIT:
            // For simplicity, treat as stop order
            if (order.side == OrderSide::BUY) {
                return current_price >= order.stop_price;
            } else {
                return current_price <= order.stop_price;
            }
            
        default:
            return false;
    }
}

double BacktestBroker::calculate_fill_price(const Order& order, double current_price) const {
    double fill_price = current_price;
    
    switch (order.type) {
        case OrderType::MARKET:
            // Apply slippage
            if (slippage_percent_ > 0) {
                double slippage = current_price * slippage_percent_ / 100.0;
                if (order.side == OrderSide::BUY) {
                    fill_price = current_price + slippage;
                } else {
                    fill_price = current_price - slippage;
                }
            }
            break;
            
        case OrderType::LIMIT:
            // Fill at the limit price or better
            if (order.side == OrderSide::BUY) {
                fill_price = std::min(current_price, order.price);
            } else {
                fill_price = std::max(current_price, order.price);
            }
            break;
            
        case OrderType::STOP:
        case OrderType::STOP_LIMIT:
            // Fill at market price when stop is triggered
            fill_price = current_price;
            break;
    }
    
    return fill_price;
}

double BacktestBroker::calculate_commission(const Order& order) const {
    // Simple commission model
    return commission_per_trade_;
}

void BacktestBroker::check_margin_requirements() {
    // Simplified margin check
    if (initial_margin_percent_ > 0 || maintenance_margin_percent_ > 0) {
        double total_position_value = 0.0;
        
        for (const auto& [symbol, position] : positions_) {
            double current_price = get_last_price(symbol);
            if (current_price > 0 && position.quantity != 0) {
                total_position_value += std::abs(position.quantity) * current_price;
            }
        }
        
        double required_margin = total_position_value * (maintenance_margin_percent_ / 100.0);
        
        if (account_info_.cash < required_margin) {
            logger_->warn("Margin call: Cash {} below required margin {}", 
                         account_info_.cash, required_margin);
            
            // In a real implementation, this might force position closure
            // For now, just log the warning
        }
    }
}

} // namespace broker
} // namespace quanttrader
