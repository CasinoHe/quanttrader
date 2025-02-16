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
        // Get the absolute path
        std::filesystem::path abs_path = std::filesystem::absolute(path);

        // Check if the path exists
        if (!std::filesystem::exists(abs_path)) {
            return false;
        }

        // should be a file
        if (!std::filesystem::is_regular_file(abs_path)) {
            return false;
        }

        config_path_ = abs_path.string();
        // check the path should be a lua file
        if (config_path_.substr(config_path_.find_last_of(".") + 1) != "lua") {
            config_path_ = "";
            return false;
        }

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

        return lua_config_->get_int_value(service_name_, key);
    }

    std::string get_string_value(const std::string &key) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_string_value(service_name_, key);
    }

    int get_int_value_in_table(const std::string &table_name, const std::string &key) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_int_value(table_name, key);
    }

    std::string get_string_value_in_table(const std::string &table_name, const std::string &key) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_string_value(table_name, key);
    }

    bool get_all_values_in_table(const std::string &table_name, std::unordered_map<std::string, std::any> &values) {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_all_values(table_name, values);
    }

    // Run the service
    virtual bool prepare() { return load_config(); }
    virtual void run() = 0;
    virtual void stop() = 0;

protected:
    ServiceBase(const std::string &&name) : service_name_(name) {};
    virtual ~ServiceBase() = default;
    void set_service_name(const std::string_view name) { service_name_ = name; }
    const std::string_view get_service_name() const { return service_name_; }

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

    const std::string_view get_config_path() const { return config_path_; }

private:
    bool config_loaded_ {false};
    std::string config_path_ {""};
    std::shared_ptr<quanttrader::luascript::LuaConfigData> lua_config_ {nullptr};
    std::string service_name_ {""};
};

}
}