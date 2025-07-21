#include "abstract_broker.h"
#include "broker_provider.h"
#include <algorithm>
#include <cmath>
#include <chrono>

namespace quanttrader {
namespace broker {

AbstractBroker::AbstractBroker(double starting_cash) : account_info_(starting_cash) {
    logger_ = quanttrader::log::get_common_rotation_logger("AbstractBroker", "broker");
    logger_->info("Initialized AbstractBroker with starting cash: {}", starting_cash);
}

long AbstractBroker::place_order(const std::string& symbol, OrderSide side, OrderType type, 
                                double quantity, double price, double stop_price) {
    if (quantity <= 0) {
        logger_->error("Invalid order quantity: {}", quantity);
        return -1;
    }
    
    long order_id = get_next_order_id();
    Order order(order_id, symbol, side, type, quantity, price, stop_price);
    order.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    // Check if we have enough buying power for buy orders
    if (side == OrderSide::BUY) {
        double required_cash = quantity * (price > 0 ? price : get_last_price(symbol));
        if (required_cash > account_info_.cash) {
            logger_->error("Insufficient cash for order. Required: {}, Available: {}", 
                         required_cash, account_info_.cash);
            order.status = OrderStatus::REJECTED;
            order.error_message = "Insufficient cash";
            orders_[order_id] = order;
            if (order_status_callback_) order_status_callback_(order);
            return order_id;
        }
    }
    
    // For backtesting or simulation, we might fill the order immediately
    // For live trading, we would send to broker provider
    orders_[order_id] = order;
    
    if (broker_provider_) {
        // Send order to actual broker
        std::string action = (side == OrderSide::BUY) ? "BUY" : "SELL";
        std::string order_type_str = order_type_to_string(type);
        
        try {
            broker_provider_->placeOrder(symbol, "STK", "SMART", "USD", 
                                       action, quantity, price, order_type_str);
        } catch (const std::exception& e) {
            logger_->error("Failed to place order with broker provider: {}", e.what());
            order.status = OrderStatus::REJECTED;
            order.error_message = e.what();
            orders_[order_id] = order;
            if (order_status_callback_) order_status_callback_(order);
            return order_id;
        }
    } else {
        // For simulation, fill market orders immediately
        if (type == OrderType::MARKET) {
            double fill_price = price > 0 ? price : get_last_price(symbol);
            if (fill_price > 0) {
                process_order_fill(order_id, quantity, fill_price, order.timestamp);
            }
        }
    }
    
    logger_->info("Placed order: {} {} {} {} @ {}", 
                 order_id, order_side_to_string(side), quantity, symbol, price);
    
    if (order_status_callback_) order_status_callback_(order);
    return order_id;
}

bool AbstractBroker::cancel_order(long order_id) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        logger_->error("Order not found: {}", order_id);
        return false;
    }
    
    Order& order = it->second;
    if (order.status != OrderStatus::PENDING && order.status != OrderStatus::PARTIALLY_FILLED) {
        logger_->error("Cannot cancel order {} with status: {}", 
                      order_id, order_status_to_string(order.status));
        return false;
    }
    
    if (broker_provider_) {
        try {
            broker_provider_->cancelOrder(order_id);
        } catch (const std::exception& e) {
            logger_->error("Failed to cancel order with broker provider: {}", e.what());
            return false;
        }
    }
    
    process_order_cancel(order_id);
    return true;
}

bool AbstractBroker::modify_order(long order_id, double new_quantity, double new_price) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        logger_->error("Order not found: {}", order_id);
        return false;
    }
    
    Order& order = it->second;
    if (order.status != OrderStatus::PENDING && order.status != OrderStatus::PARTIALLY_FILLED) {
        logger_->error("Cannot modify order {} with status: {}", 
                      order_id, order_status_to_string(order.status));
        return false;
    }
    
    // For simplicity, we'll cancel and replace with a new order
    // In a real implementation, this would use broker-specific modify functionality
    if (new_quantity > 0) {
        order.quantity = new_quantity;
        order.remaining_quantity = new_quantity - order.filled_quantity;
    }
    if (new_price > 0) {
        order.price = new_price;
    }
    
    logger_->info("Modified order: {} new qty: {} new price: {}", 
                 order_id, new_quantity, new_price);
    
    if (order_status_callback_) order_status_callback_(order);
    return true;
}

std::optional<Order> AbstractBroker::get_order(long order_id) const {
    auto it = orders_.find(order_id);
    if (it != orders_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<Order> AbstractBroker::get_orders(const std::string& symbol) const {
    std::vector<Order> result;
    for (const auto& [id, order] : orders_) {
        if (symbol.empty() || order.symbol == symbol) {
            result.push_back(order);
        }
    }
    return result;
}

std::vector<Order> AbstractBroker::get_open_orders(const std::string& symbol) const {
    std::vector<Order> result;
    for (const auto& [id, order] : orders_) {
        if ((order.status == OrderStatus::PENDING || order.status == OrderStatus::PARTIALLY_FILLED) &&
            (symbol.empty() || order.symbol == symbol)) {
            result.push_back(order);
        }
    }
    return result;
}

Position AbstractBroker::get_position(const std::string& symbol) const {
    auto it = positions_.find(symbol);
    if (it != positions_.end()) {
        return it->second;
    }
    return Position(symbol, 0.0, 0.0);
}

std::map<std::string, Position> AbstractBroker::get_all_positions() const {
    return positions_;
}

bool AbstractBroker::close_position(const std::string& symbol, double quantity) {
    auto it = positions_.find(symbol);
    if (it == positions_.end() || it->second.quantity == 0) {
        logger_->warn("No position to close for symbol: {}", symbol);
        return false;
    }
    
    const Position& pos = it->second;
    double qty_to_close = (quantity > 0) ? std::min(quantity, std::abs(pos.quantity)) : std::abs(pos.quantity);
    
    OrderSide side = pos.is_long() ? OrderSide::SELL : OrderSide::BUY;
    
    long order_id = place_order(symbol, side, OrderType::MARKET, qty_to_close);
    
    logger_->info("Closing position: {} {} shares of {}", 
                 qty_to_close, pos.is_long() ? "long" : "short", symbol);
    
    return order_id > 0;
}

std::vector<Trade> AbstractBroker::get_trades(const std::string& symbol) const {
    if (symbol.empty()) {
        return trades_;
    }
    
    std::vector<Trade> result;
    for (const auto& trade : trades_) {
        if (trade.symbol == symbol) {
            result.push_back(trade);
        }
    }
    return result;
}

std::vector<Trade> AbstractBroker::get_trades_by_date(uint64_t start_time, uint64_t end_time, const std::string& symbol) const {
    std::vector<Trade> result;
    for (const auto& trade : trades_) {
        if (trade.timestamp >= start_time && trade.timestamp <= end_time &&
            (symbol.empty() || trade.symbol == symbol)) {
            result.push_back(trade);
        }
    }
    return result;
}

void AbstractBroker::update_market_prices(const std::map<std::string, double>& prices) {
    for (const auto& [symbol, price] : prices) {
        last_prices_[symbol] = price;
    }
    
    // Update unrealized P&L for all positions
    for (auto& [symbol, position] : positions_) {
        auto price_it = prices.find(symbol);
        if (price_it != prices.end() && position.quantity != 0) {
            double current_price = price_it->second;
            position.unrealized_pnl = (current_price - position.avg_price) * position.quantity;
        }
    }
    
    update_account_info();
}

double AbstractBroker::get_last_price(const std::string& symbol) const {
    auto it = last_prices_.find(symbol);
    return (it != last_prices_.end()) ? it->second : 0.0;
}

void AbstractBroker::reset_account(double starting_cash) {
    account_info_ = AccountInfo(starting_cash);
    orders_.clear();
    positions_.clear();
    trades_.clear();
    last_prices_.clear();
    next_order_id_ = 1;
    
    logger_->info("Reset account with starting cash: {}", starting_cash);
}

void AbstractBroker::process_order_fill(long order_id, double fill_quantity, double fill_price, uint64_t timestamp) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        logger_->error("Order not found for fill: {}", order_id);
        return;
    }
    
    Order& order = it->second;
    
    // Update order status
    order.filled_quantity += fill_quantity;
    order.remaining_quantity -= fill_quantity;
    
    if (order.remaining_quantity <= 0) {
        order.status = OrderStatus::FILLED;
        order.remaining_quantity = 0;
    } else {
        order.status = OrderStatus::PARTIALLY_FILLED;
    }
    
    // Create trade record
    Trade trade(order_id, order.symbol, order.side, fill_quantity, fill_price, timestamp);
    trades_.push_back(trade);
    
    // Update position
    update_position(order.symbol, order.side, fill_quantity, fill_price);
    
    // Update account
    update_account_info();
    
    logger_->info("Order fill: {} {} {} @ {}", 
                 order_id, fill_quantity, order.symbol, fill_price);
    
    // Notify callbacks
    if (order_status_callback_) order_status_callback_(order);
    if (trade_callback_) trade_callback_(trade);
}

void AbstractBroker::process_order_cancel(long order_id) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        logger_->error("Order not found for cancel: {}", order_id);
        return;
    }
    
    Order& order = it->second;
    order.status = OrderStatus::CANCELLED;
    order.remaining_quantity = 0;
    
    logger_->info("Order cancelled: {}", order_id);
    
    if (order_status_callback_) order_status_callback_(order);
}

void AbstractBroker::process_order_reject(long order_id, const std::string& reason) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        logger_->error("Order not found for reject: {}", order_id);
        return;
    }
    
    Order& order = it->second;
    order.status = OrderStatus::REJECTED;
    order.error_message = reason;
    order.remaining_quantity = 0;
    
    logger_->info("Order rejected: {} - {}", order_id, reason);
    
    if (order_status_callback_) order_status_callback_(order);
}

void AbstractBroker::update_position(const std::string& symbol, OrderSide side, double quantity, double price) {
    auto it = positions_.find(symbol);
    Position* position = nullptr;
    
    if (it == positions_.end()) {
        positions_[symbol] = Position(symbol, 0.0, 0.0);
        position = &positions_[symbol];
    } else {
        position = &it->second;
    }
    
    double signed_quantity = (side == OrderSide::BUY) ? quantity : -quantity;
    double old_quantity = position->quantity;
    double new_quantity = old_quantity + signed_quantity;
    
    // Calculate realized P&L if closing or reducing position
    if ((old_quantity > 0 && signed_quantity < 0) || (old_quantity < 0 && signed_quantity > 0)) {
        double closed_quantity = std::min(std::abs(signed_quantity), std::abs(old_quantity));
        double pnl_per_share = (old_quantity > 0) ? (price - position->avg_price) : (position->avg_price - price);
        position->realized_pnl += pnl_per_share * closed_quantity;
        
        // Update cash
        account_info_.cash += (side == OrderSide::SELL) ? quantity * price : -quantity * price;
    }
    
    // Update position
    if (new_quantity == 0) {
        position->quantity = 0;
        position->avg_price = 0;
        position->unrealized_pnl = 0;
    } else if ((old_quantity >= 0 && new_quantity > 0) || (old_quantity <= 0 && new_quantity < 0)) {
        // Same direction or opening new position
        double total_cost = position->avg_price * std::abs(old_quantity) + price * quantity;
        position->avg_price = total_cost / std::abs(new_quantity);
        position->quantity = new_quantity;
        
        // Update cash
        account_info_.cash += (side == OrderSide::SELL) ? quantity * price : -quantity * price;
    } else {
        // Changing direction
        position->quantity = new_quantity;
        position->avg_price = price;
        position->unrealized_pnl = 0;
        
        // Update cash
        account_info_.cash += (side == OrderSide::SELL) ? quantity * price : -quantity * price;
    }
    
    if (position_callback_) position_callback_(*position);
}

void AbstractBroker::update_account_info() {
    // Calculate total unrealized P&L
    double total_unrealized_pnl = 0;
    double total_realized_pnl = 0;
    double position_value = 0;
    
    for (const auto& [symbol, position] : positions_) {
        total_unrealized_pnl += position.unrealized_pnl;
        total_realized_pnl += position.realized_pnl;
        
        double last_price = get_last_price(symbol);
        if (last_price > 0) {
            position_value += position.quantity * last_price;
        }
    }
    
    account_info_.unrealized_pnl = total_unrealized_pnl;
    account_info_.realized_pnl = total_realized_pnl;
    account_info_.equity = account_info_.cash + position_value;
    account_info_.buying_power = account_info_.cash; // Simplified
    
    if (account_callback_) account_callback_(account_info_);
}

std::string AbstractBroker::order_side_to_string(OrderSide side) const {
    switch (side) {
        case OrderSide::BUY: return "BUY";
        case OrderSide::SELL: return "SELL";
        default: return "UNKNOWN";
    }
}

std::string AbstractBroker::order_type_to_string(OrderType type) const {
    switch (type) {
        case OrderType::MARKET: return "MKT";
        case OrderType::LIMIT: return "LMT";
        case OrderType::STOP: return "STP";
        case OrderType::STOP_LIMIT: return "STP LMT";
        default: return "UNKNOWN";
    }
}

std::string AbstractBroker::order_status_to_string(OrderStatus status) const {
    switch (status) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::FILLED: return "FILLED";
        case OrderStatus::PARTIALLY_FILLED: return "PARTIALLY_FILLED";
        case OrderStatus::CANCELLED: return "CANCELLED";
        case OrderStatus::REJECTED: return "REJECTED";
        default: return "UNKNOWN";
    }
}

} // namespace broker
} // namespace quanttrader
