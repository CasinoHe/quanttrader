#pragma once

#include "observer_base.h"
#include "logger/quantlogger.h"
#include "broker/abstract_broker.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace quanttrader {
namespace observer {

struct TradeRecord {
    uint64_t time;
    std::string symbol;
    int quantity;
    double price;
    bool is_buy;
};

struct CompletedTrade {
    uint64_t entry_time;
    uint64_t exit_time;
    std::string symbol;
    int quantity;
    bool is_long;
    double entry_price;
    double exit_price;
    double max_drawdown;
    double profit;
    uint64_t duration;
};

struct OpenTrade {
    uint64_t entry_time;
    std::string symbol;
    int quantity;
    bool is_long;
    double entry_price;
    double worst_price;
};

class PerformanceObserver : public ObserverBase {
public:
    explicit PerformanceObserver(double starting_cash = 100000.0);
    ~PerformanceObserver() override = default;

    void record_trade(uint64_t time, const std::string& symbol, int quantity, double price, bool is_buy) override;

    void update_market_value(uint64_t time, const std::map<std::string, double>& prices) override;

    // Broker integration
    void set_broker(std::shared_ptr<broker::AbstractBroker> broker) { broker_ = broker; }
    std::shared_ptr<broker::AbstractBroker> get_broker() const { return broker_; }

    double get_equity() const { return equity_; }
    double get_max_drawdown() const { return max_drawdown_; }
    double get_gross_profit() const { return gross_profit_; }
    double get_gross_loss() const { return gross_loss_; }

    void report() const override;

private:
    double starting_cash_;
    double cash_;
    double equity_;
    double peak_equity_;
    double max_drawdown_;
    double gross_profit_ = 0.0;
    double gross_loss_ = 0.0;
    std::map<std::string, int> positions_;
    std::map<std::string, double> avg_price_;
    std::vector<TradeRecord> trades_;
    std::map<std::string, OpenTrade> open_trades_;
    std::vector<CompletedTrade> completed_trades_;
    quanttrader::log::LoggerPtr logger_;
    
    // Broker integration
    std::shared_ptr<broker::AbstractBroker> broker_;
    
    // Internal methods
    void update_from_broker();
};

} // namespace observer
} // namespace quanttrader
