#pragma once

#include "common/singleton.h"
#include "config/lua_config_loader.h"
#include "service_consts.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include <exception>

namespace quanttrader {
namespace service {

/**
 * Interface class for all services.
 * Defines the common methods that all services must implement.
 */
class ServiceInterface {
public:
    virtual ~ServiceInterface() = default;
    
    // Configuration management
    virtual bool set_config_path(const std::string_view path) = 0;
    virtual bool set_reload_config() = 0;
    virtual bool is_service_prepared() const = 0;
    
    // Service lifecycle methods
    virtual bool prepare() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
    
    // Service identification
    virtual const std::string_view get_service_name() const = 0;
    
    // Configuration access methods
    virtual int get_int_value(const std::string &key) = 0;
    virtual std::string get_string_value(const std::string &key) = 0;
    virtual int get_int_value_in_table(const std::string &table_name, const std::string &key) = 0;
    virtual std::string get_string_value_in_table(const std::string &table_name, const std::string &key) = 0;
    virtual bool get_all_values_in_table(const std::string &table_name, std::unordered_map<std::string, std::any> &values) = 0;
};

template<typename Derived>
class ServiceBase : public Singleton<Derived>, public ServiceInterface {
public:
    bool set_config_path(const std::string_view path) override {
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

    bool set_reload_config() override {
        if (!config_loaded_) {
            return false;
        }
        config_loaded_ = false;
        return true;
    }

    virtual bool is_service_prepared() const override { return config_loaded_; }

    int get_int_value(const std::string &key) override {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_int_value(service_name_, key);
    }

    std::string get_string_value(const std::string &key) override {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_string_value(service_name_, key);
    }

    int get_int_value_in_table(const std::string &table_name, const std::string &key) override {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_int_value(table_name, key);
    }

    std::string get_string_value_in_table(const std::string &table_name, const std::string &key) override {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_string_value(table_name, key);
    }

    bool get_all_values_in_table(const std::string &table_name, std::unordered_map<std::string, std::any> &values) override {
        if (!config_loaded_) {
            throw std::runtime_error("Service is not prepared. Please prepare the service first.");
        }

        return lua_config_->get_all_values(table_name, values);
    }

    // Run the service
    virtual bool prepare() override { return load_config(); }
    virtual void run() override = 0;
    virtual void stop() override = 0;

    // Get the service name
    const std::string_view get_service_name() const override { return service_name_; }

protected:
    ServiceBase(const std::string &&name) : service_name_(name) {};
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
        lua_config_ = std::make_shared<quanttrader::luascript::LuaConfigLoader>(config_path_);
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
    std::shared_ptr<quanttrader::luascript::LuaConfigLoader> lua_config_ {nullptr};
    std::string service_name_ {""};
};

}
}