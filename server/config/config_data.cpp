#include "config_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef __cplusplus
}
#endif

#include <filesystem>

namespace quanttrader {
namespace luascript {

LuaConfigData::LuaConfigData(const std::string &script_path): script_path_(script_path) {
    logger_ = quanttrader::logger::get_common_rotation_logger("LuaConfigData", "LuaConfigData");

    luastate_ = luaL_newstate();
    luaL_openlibs(luastate_); // Load Lua libraries

    logger_->info("Lua state created for script: {}", script_path_);
}

LuaConfigData::~LuaConfigData() {
    if (luastate_) {
        lua_close(luastate_);
    }
}

bool LuaConfigData::run_lua_script() {
    // check the file exists
    if (!std::filesystem::exists(script_path_)) {
        logger_->error("Lua script file does not exist: {}", script_path_);
        return false;
    }

    if (luaL_dofile(luastate_, script_path_.c_str()) != LUA_OK) {
        logger_->error("Error running Lua script: {}", lua_tostring(luastate_, -1));
        lua_pop(luastate_, 1); // Remove error message from stack
        return false;
    }
    return true;
}

int LuaConfigData::get_int_value(const std::string &table_name, const std::string &key) {
    if (table_name.empty() || key.empty()) {
        logger_->error("Table name *{}* or key *{}* is empty.", table_name, key);
        return 0;
    }

    lua_getglobal(luastate_, table_name.c_str()); // Push the table onto the stack
    if (!lua_istable(luastate_, -1)) {
        logger_->error("{} is not a table.", table_name);
        lua_pop(luastate_, 1); // Remove non-table from stack
        return 0;
    }

    lua_pushstring(luastate_, key.c_str()); // Push the key onto the stack
    lua_gettable(luastate_, -2);           // Get the value associated with the key

    if (!lua_isinteger(luastate_, -1)) {
        logger_->error("Value from {} for key {} is not an integer.", table_name, key);
        lua_pop(luastate_, 2); // Remove value and table from stack
        return 0;
    }

    int value = static_cast<int>(lua_tointeger(luastate_, -1));
    lua_pop(luastate_, 2); // Remove value and table from stack
    return value;
}

std::string LuaConfigData::get_string_value(const std::string &table_name, const std::string &key) {
    if (table_name.empty() || key.empty()) {
        logger_->error("Table name *{}* or key *{}* is empty.", table_name, key);
        return "";
    }
    lua_getglobal(luastate_, table_name.c_str()); // Push the table onto the stack
    if (!lua_istable(luastate_, -1)) {
        logger_->error("{} is not a table.", table_name);
        lua_pop(luastate_, 1); // Remove non-table from stack
        return "";
    }

    lua_pushstring(luastate_, key.c_str()); // Push the key onto the stack
    lua_gettable(luastate_, -2);           // Get the value associated with the key

    if (!lua_isstring(luastate_, -1)) {
        logger_->error("Value from {} for key {} is not a string.", table_name, key);
        lua_pop(luastate_, 2); // Remove value and table from stack
        return "";
    }

    std::string value = lua_tostring(luastate_, -1);
    lua_pop(luastate_, 2); // Remove value and table from stack
    return value;
}

}
}