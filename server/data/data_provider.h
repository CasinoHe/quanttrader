#pragma once

#include "data_consts.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <any>

namespace quanttrader {

namespace service {
class TwsService;
}

namespace data {

using DataParamsType = std::shared_ptr<std::unordered_map<std::string, std::any>>;

class DataProvider {
public:
    DataProvider(const std::string_view &data_prefix, DataParamsType params);
    ~DataProvider() = default;

private:
    std::string get_data_by_prefix(const std::string_view prefix, DataParamsType params);
    std::shared_ptr<quanttrader::service::TwsService> broker_service_ {nullptr};

    // TODO: DataStorage service

    std::string tick_name_ {""};
    std::string security_type_ {kDefaultSecurityType};
    std::string exchange_ {kDefaultExchange};
    std::string currency_ {kDefaultCurrency};
    bool use_rth_ {kDefaultUseRth};

    std::string start_date_ {""};
    std::string end_date_ {""};
    std::string bar_type_ {""};
};

}
}