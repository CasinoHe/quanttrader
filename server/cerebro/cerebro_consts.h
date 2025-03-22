#pragma once

namespace quanttrader {
namespace cerebro {

// Cerebro type constants
constexpr char BACKTEST_CEREBRO_TYPE[] = "backtest";
constexpr char LIVE_CEREBRO_TYPE[] = "live";

// Configuration keys
constexpr char CEREBRO_TYPE_KEY[] = "cerebro_type";
constexpr char CEREBRO_NAME_KEY[] = "cerebro_name";
constexpr char INITIAL_CASH_KEY[] = "initial_cash";
constexpr char COMMISSION_KEY[] = "commission";
constexpr char SLIPPAGE_KEY[] = "slippage";
constexpr char START_DATE_KEY[] = "start_date";
constexpr char END_DATE_KEY[] = "end_date";
constexpr char DATA_TIMEFRAME_KEY[] = "data_timeframe";
constexpr char BROKER_TYPE_KEY[] = "broker_type";

// Default values
constexpr double DEFAULT_INITIAL_CASH = 100000.0;
constexpr double DEFAULT_COMMISSION = 0.0;
constexpr double DEFAULT_SLIPPAGE = 0.0;

} // namespace cerebro
} // namespace quanttrader