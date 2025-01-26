#pragma once

namespace quanttrader {
namespace data {

constexpr const char *DATA_TICK_NAME = "_tick";
constexpr const char *DATA_START_DATE_NAME = "_start_date";
constexpr const char *DATA_END_DATE_NAME = "_end_date";
constexpr const char *BAR_TYPE_NAME = "_bar_type";
constexpr const char *BAR_INTERVAL_NAME = "_bar_interval";
constexpr const char *DATA_SECURITY_TYPE_NAME = "_security_type";
constexpr const char *DATA_EXCHANGE_NAME = "_exchange";
constexpr const char *DATA_CURRENCY_NAME = "_currency";
constexpr const char *DATA_USE_RTH_NAME = "_use_rth";
constexpr const char *DATA_TIMEZONE_NAME = "_timezone";
constexpr const char *DATA_TIMEFRAME_TYPE_NAME = "_timeframe_type";
constexpr const char *DATA_TRADE_WHAT_NAME = "_what";

constexpr const char *kDefaultSecurityType = "STK";
constexpr const char *kDefaultExchange = "SMART";
constexpr const char *kDefaultCurrency = "USD";
constexpr const char *kDefaultTimezone = "America/New_York";
constexpr const char *kDefaultWhatToShoe = "TRADES";
constexpr const bool kDefaultUseRth = false;

}
}