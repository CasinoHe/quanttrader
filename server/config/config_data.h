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
    int get_int_value(const std::string &table_name, const std::string &key);

    // Get a string value from a Lua table
    std::string get_string_value(const std::string &table_name, const std::string &key);

private:
    bool inited_ = false;
    lua_State *luastate_ = NULL;
    std::string script_path_ = "";
    quanttrader::log::LoggerPtr logger_ = nullptr;
};

} // namespace luascript
} // namespace quanttrader