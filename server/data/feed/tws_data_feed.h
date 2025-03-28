#pragma once

#include "data/data_provider.h"
#include "data/common/data_consts.h"
#include <atomic>
#include <string>

namespace quanttrader {

namespace broker {
struct ResHistoricalData;
struct ResRealtimeData;
class TwsBrokerAdapter;
class BrokerProvider;
}

namespace data {
namespace feed {

/**
 * @brief TWS Broker data feed implementation
 * 
 * This class implements a data feed using the TWS (Interactive Brokers Trader Workstation) API
 * to request historical and real-time market data.
 */
class TwsDataFeed : public provider::DataProvider {
public:
    TwsDataFeed(const std::string_view &data_prefix, provider::DataParamsType params);
    ~TwsDataFeed() = default;

    // Implementation of DataProvider interface
    bool prepare_data() override;
    bool start_request_data() override;
    bool terminate_request_data() override;
    bool is_data_ready() override;
    std::optional<BarStruct> next() override;
    inline void set_broker(std::shared_ptr<broker::BrokerProvider> broker_adapter) override;

protected:
    bool is_historical_completed() const { return historical_fetch_completed_.load(); }
    long subscribe_realtime_data();
    long fetch_historical_data();
    std::optional<std::string> get_duration();

private:
    std::shared_ptr<broker::TwsBrokerAdapter> broker_adapter_{nullptr};

    // TWS specific configuration
    std::string security_type_ {kDefaultSecurityType};
    std::string exchange_ {kDefaultExchange};
    std::string currency_ {kDefaultCurrency};
    bool use_rth_ {kDefaultUseRth};
    std::string timezone_ {kDefaultTimezone};
    std::string what_type_ {kDefaultWhatToShow};
    bool keep_up_to_date_ {false};

    // Historical data parameters
    std::string start_date_ {""};
    std::string end_date_ {""};
    std::string bar_type_str_ {""};
    bool is_realtime_ {false};
    bool is_historical_ {false};
    std::atomic<bool> historical_fetch_completed_ {false};
    long request_id_ = 0;
    int historical_data_length_ = 0;
};

} // namespace feed
} // namespace data
} // namespace quanttrader