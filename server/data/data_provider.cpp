#include "data_provider.h"

namespace quanttrader {
namespace data {

DataProvider::DataProvider(const std::string_view &data_prefix, DataParamsType params) {
    tick_name_ = get_data_by_prefix(data_prefix, params);
}

std::string DataProvider::get_data_by_prefix(const std::string_view prefix, DataParamsType params) {
    return {};
}

}
}