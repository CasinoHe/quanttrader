#pragma once

#include "data/data_provider.h"
#include "data/util/bar_line.h"
#include "data/common/data_consts.h"
#include <atomic>
#include <string>

namespace quanttrader {

namespace broker {
struct ResHistoricalData;
struct ResRealtimeData;
}

namespace service {
class TwsService;
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

    // TWS specific methods
    void historical_data_response(std::shared_ptr<broker::ResHistoricalData> response);
    void realtime_data_response(std::shared_ptr<broker::ResRealtimeData> response);
    std::string get_ticker_name() const { return tick_name_; }

protected:
    bool is_historical_completed() { return historical_fetch_completed_.load(); }
    long subscribe_realtime_data();
    long fetch_historical_data();
    std::optional<std::string> get_duration();
    std::pair<BarType, unsigned int> get_bar_type_from_string(const std::string &bar_type);

private:
    std::shared_ptr<quanttrader::service::TwsService> broker_service_ {nullptr};
    std::shared_ptr<util::BarLine> bar_line_ {nullptr};

    std::string tick_name_ {};
    std::string security_type_ {kDefaultSecurityType};
    std::string exchange_ {kDefaultExchange};
    std::string currency_ {kDefaultCurrency};
    bool use_rth_ {kDefaultUseRth};
    std::string timezone_ {kDefaultTimezone};
    std::string what_type_ {kDefaultWhatToShow};
    bool keep_up_to_date_ {false};

    std::string start_date_ {""};
    std::string end_date_ {""};
    std::string bar_type_ {""};
    bool is_realtime_ {false};
    bool is_historical_ {false};
    std::atomic<bool> historical_fetch_completed_ {false};
    long request_id_ = 0;
    int historical_data_length_ = 0;
};

} // namespace feed
} // namespace data
} // namespace quanttrader