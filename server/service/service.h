#pragma once

#include "common/singleton.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>

namespace quanttrader {

namespace luascript {
// forward declaration
class LuaConfigData;
}

namespace service {

template<typename Derived>
class ServiceBase : public Singleton<Derived> {
public:
    bool set_config_path(const std::string_view path);
    bool reload_config();

    // Run the service
    virtual void run() = 0;

protected:
    ServiceBase() = default;
    virtual ~ServiceBase() = default;

private:
    bool config_loaded_ = false;
    std::string config_path_ = "";
    std::shared_ptr<quanttrader::luascript::LuaConfigData> lua_config_ = nullptr;
};

}
}