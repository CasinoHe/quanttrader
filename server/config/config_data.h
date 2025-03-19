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

    // Get all values from a Lua table and its nested tables
    // Results are stored with dot notation keys, e.g., "a.b.c.d" = 1
    bool get_all_values(const std::string &table_name, std::unordered_map<std::string, std::any> &values);

private:
    // Helper function to traverse nested tables using dot notation
    bool traverse_nested_tables(const std::string &table_name, const std::string &key_path);
    
    // Recursive helper function to extract all values from a table and its nested tables
    void extract_table_values(std::unordered_map<std::string, std::any> &values, const std::string &prefix);
    
    bool inited_ = false;
    lua_State *luastate_ = NULL;
    std::string script_path_ = "";
    quanttrader::log::LoggerPtr logger_ = nullptr;
};

} // namespace luascript
} // namespace quanttrader