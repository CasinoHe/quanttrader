#include "service.h"
#include <filesystem>


namespace quanttrader {
namespace service {

template<typename Derived>
bool ServiceBase<Derived>::set_config_path(const std::string_view path) {
    // Check if the path exists
    if (!std::filesystem::exists(path)) {
        return false;
    }

    // should be a file
    if (!std::filesystem::is_regular_file(path)) {
        return false;
    }

    // check the path should be a lua file
    if (path.substr(path.find_last_of(".") + 1) != "lua") {
        return false;
    }

    config_path_ = path;
    return true;
}

}
}