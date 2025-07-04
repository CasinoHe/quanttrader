#pragma once

namespace quanttrader {
namespace data {

constexpr const char *DATA_TICK_NAME = "ticker";
constexpr const char *DATA_TYPE_NAME = "type";
constexpr const char *DATA_START_DATE_NAME = "start_date";
constexpr const char *DATA_END_DATE_NAME = "end_date";
constexpr const char *DATA_SECURITY_TYPE_NAME = "security_type";
constexpr const char *DATA_EXCHANGE_NAME = "exchange";
constexpr const char *DATA_CURRENCY_NAME = "currency";
constexpr const char *DATA_USE_RTH_NAME = "use_rth";
constexpr const char *DATA_TIMEZONE_NAME = "timezone";
constexpr const char *DATA_TRADE_WHAT_NAME = "what";
constexpr const char *BAR_TYPE_NAME = "bar_type";
constexpr const char *KEEP_UP_TO_DATE_NAME = "up_to_date";
constexpr const char *NEED_RESAMPLE = "resample";
constexpr const char *RESAMPLE_BAR_SIZE = "resample_bar_size";

constexpr const char *kDefaultSecurityType = "STK";
constexpr const char *kDefaultExchange = "SMART";
constexpr const char *kDefaultCurrency = "USD";
constexpr const char *kDefaultTimezone = "America/New_York";
constexpr const char *kDefaultWhatToShow = "TRADES";
constexpr const char *kDefaultDataType = "historical";
constexpr const bool kDefaultUseRth = false;
constexpr const char *kDefaultSessionStart = "09:30:00";
constexpr const char *kDefaultSessionEnd = "16:00:00";

constexpr const char *DATA_SESSION_START_NAME = "session_start";
constexpr const char *DATA_SESSION_END_NAME = "session_end";
constexpr const char *DATA_SESSION_TIMEZONE_NAME = "session_timezone";

constexpr const char *kSecondsBarType[] = {"1 secs", "5 secs", "10 secs", "15 secs", "30 secs"};
constexpr const char *kMinutesBarType[] = {"1 min", "2 mins", "3 mins", "5 mins", "10 mins", "15 mins", "20 mins", "30 mins"};
constexpr const char *kHoursBarType[] = {"1 hour", "2 hours", "3 hours", "4 hours", "8 hours"};
constexpr const char *kDaysBarType[] = {"1 day"};
constexpr const char *kWeekBarType[] = {"1 W"};
constexpr const char *kMonthBarType[] = {"1 M"};

constexpr unsigned long kSecondsToNano = 1000000000;

constexpr const int kLastBarIndex = -1;

}
}