#pragma once

#include "data_consts.h"
#include "logger/quantlogger.h"
#include "data/lines.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <optional>
#include <atomic>

namespace quanttrader {

namespace broker {

struct ResHistoricalData;
struct ResRealtimeData;
}

namespace service {
class TwsService;
}

namespace data {

using DataParamsType = std::shared_ptr<std::unordered_map<std::string, std::any>>;

class DataProvider {
public:
    DataProvider(const std::string_view &data_prefix, DataParamsType params);
    ~DataProvider() = default;

    bool prepare_data();
    bool start_request_data();
    bool terminate_request_data();
    bool is_data_ready();

    std::string get_data_prefix() const { return data_prefix_; }

    void historical_data_response(std::shared_ptr<broker::ResHistoricalData> response);
    void realtime_data_response(std::shared_ptr<broker::ResRealtimeData> response);

protected:
    bool is_historical_completed() { return historical_fetch_completed_.load(); }
    long subscribe_realtime_data();
    long fetch_historical_data();
    std::optional<std::string> get_duration();

    std::pair<BarType, unsigned int> get_bar_type_from_string(const std::string &bar_type);

private:
    template<typename T>
    T get_data_by_prefix(const std::string &&data, std::optional<T> default_value=std::nullopt) {
        std::string key = data_prefix_ + data;
        auto iter = params_->find(key);
        if (iter == params_->end()) {
            if (default_value.has_value()) {
                return default_value.value();
            } else {
                return {};
            }
        }
        return std::any_cast<T>(iter->second);
    }
    std::shared_ptr<quanttrader::service::TwsService> broker_service_ {nullptr};
    std::shared_ptr<BarLine> bar_line_ {nullptr};

    // TODO: DataStorage service

    std::string data_prefix_ {};
    DataParamsType params_ {nullptr};
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

    quanttrader::log::LoggerPtr logger_ {nullptr};
};

}
}