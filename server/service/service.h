#pragma once

#include "common/singleton.h"
#include "config/config_data.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include <exception>

namespace quanttrader {
namespace service {

template<typename Derived>
class ServiceBase : public Singleton<Derived> {
public:
    bool set_config_path(const std::string_view path) {
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

    bool set_reload_config() {
        if (!config_loaded_) {
            return false;
        }
        config_loaded_ = false;
        return true;
    }

    virtual bool is_service_prepared() const { return config_loaded_; }

    int get_int_value(const std::string &key) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_int_value("service", key);
    }

    std::string get_string_value(const std::string &key) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_string_value("service", key);
    }

    // Run the service
    virtual bool prepare() { return load_config(); };
    virtual void run() = 0;

protected:
    ServiceBase() = default;
    virtual ~ServiceBase() = default;

    bool load_config() {
        if (config_loaded_) {
            return true;
        }

        if (config_path_.empty()) {
            return false;
        }

        // check the config file again
        // Check if the path exists
        if (!std::filesystem::exists(config_path_)) {
            return false;
        }

        // should be a file
        if (!std::filesystem::is_regular_file(config_path_)) {
            return false;
        }

        // check the path should be a lua file
        if (config_path_.substr(config_path_.find_last_of(".") + 1) != "lua") {
            return false;
        }

        // Load the configuration file
        lua_config_ = std::make_shared<quanttrader::luascript::LuaConfigData>(config_path_);
        if (!lua_config_->run_lua_script())
        {
            return false;
        }

        config_loaded_ = true;
        return true;
    }

private:
    bool config_loaded_ = false;
    std::string config_path_ = "";
    std::shared_ptr<quanttrader::luascript::LuaConfigData> lua_config_ = nullptr;
};

}
}