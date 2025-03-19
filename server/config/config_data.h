#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "lstate.h"

#ifdef __cplusplus
}
#endif

#include "common/consts.h"
#include "logger/quantlogger.h"

#include <string>
#include <iostream>
#include <unordered_map>
#include <any>

namespace quanttrader {
namespace luascript {

class LuaConfigData {

public:
    // Constructor: initializes the Lua state
    LuaConfigData(const std::string &script_path);
    // Destructor: closes the Lua state
    ~LuaConfigData();

    // Run a Lua script file
    bool run_lua_script();

    // Get an integer value from a Lua table
    // Support nested table access with dot notation, e.g., "a.b.c.d"
    int get_int_value(const std::string &table_name, const std::string &key_path);

    // Get a string value from a Lua table
    // Support nested table access with dot notation, e.g., "a.b.c.d"
    std::string get_string_value(const std::string &table_name, const std::string &key_path);

    // Get all double values(treat int as double) and string values from a Lua table
    bool get_all_values(const std::string &table_name, std::unordered_map<std::string, std::any> &values);

private:
    // Helper function to traverse nested tables using dot notation
    bool traverse_nested_tables(const std::string &table_name, const std::string &key_path);
    
    bool inited_ = false;
    lua_State *luastate_ = NULL;
    std::string script_path_ = "";
    quanttrader::log::LoggerPtr logger_ = nullptr;
};

} // namespace luascript
} // namespace quanttrader