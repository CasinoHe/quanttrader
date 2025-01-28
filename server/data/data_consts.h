#pragma once

namespace quanttrader {
namespace data {

constexpr const char *DATA_TICK_NAME = "_ticker";
constexpr const char *DATA_TYPE_NAME = "_type";
constexpr const char *DATA_START_DATE_NAME = "_start_date";
constexpr const char *DATA_END_DATE_NAME = "_end_date";
constexpr const char *DATA_SECURITY_TYPE_NAME = "_security_type";
constexpr const char *DATA_EXCHANGE_NAME = "_exchange";
constexpr const char *DATA_CURRENCY_NAME = "_currency";
constexpr const char *DATA_USE_RTH_NAME = "_use_rth";
constexpr const char *DATA_TIMEZONE_NAME = "_timezone";
constexpr const char *DATA_TRADE_WHAT_NAME = "_what";
constexpr const char *BAR_TYPE_NAME = "_bar_type";

constexpr const char *kDefaultSecurityType = "STK";
constexpr const char *kDefaultExchange = "SMART";
constexpr const char *kDefaultCurrency = "USD";
constexpr const char *kDefaultTimezone = "America/New_York";
constexpr const char *kDefaultWhatToShow = "TRADES";
constexpr const char *kDefaultDataType = "historical";
constexpr const bool kDefaultUseRth = false;

}
}