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
#include <sstream>

namespace quanttrader {
namespace luascript {

LuaConfigData::LuaConfigData(const std::string &script_path): script_path_(script_path) {
    logger_ = quanttrader::log::get_common_rotation_logger("LuaConfigData", "config");
}

LuaConfigData::~LuaConfigData() {
    if (luastate_) {
        lua_close(luastate_);
    }
}

bool LuaConfigData::run_lua_script() {
    if (!inited_) {
        luastate_ = luaL_newstate();
        luaL_openlibs(luastate_); // Load Lua libraries

        logger_->info("Lua state created for script: {}", script_path_);
        inited_ = true;
    }

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

// Helper function to traverse nested tables using dot notation
bool LuaConfigData::traverse_nested_tables(const std::string &table_name, const std::string &key_path) {
    if (table_name.empty()) {
        logger_->error("Table name is empty.");
        return false;
    }
    
    // Push the base table onto the stack
    lua_getglobal(luastate_, table_name.c_str());
    if (!lua_istable(luastate_, -1)) {
        logger_->error("{} is not a table.", table_name);
        lua_pop(luastate_, 1); // Remove non-table from stack
        return false;
    }
    
    if (key_path.empty()) {
        return true; // The base table is already on the stack
    }
    
    // Split the key path by dots
    std::stringstream ss(key_path);
    std::string key;
    std::string current_path = table_name;
    
    while (std::getline(ss, key, '.')) {
        if (key.empty()) continue;
        
        current_path += "." + key;
        
        // For all but the last key, we expect tables
        lua_pushstring(luastate_, key.c_str());
        lua_gettable(luastate_, -2);
        
        // If not the last key and not a table, error
        if (ss.peek() != EOF && !lua_istable(luastate_, -1)) {
            if (lua_isnil(luastate_, -1)) {
                logger_->error("Table {} does not have a nested table at key {}", current_path, key);
            } else {
                logger_->error("Value at {} is not a table but trying to access it as one", current_path);
            }
            lua_pop(luastate_, 2); // Remove value and parent table
            return false;
        }
        
        // Replace the parent table with the nested table for the next iteration
        if (ss.peek() != EOF) {
            lua_remove(luastate_, -2); // Remove the parent table, leaving the nested one
        }
    }
    
    return true;
}

// Recursive helper function to extract all values from a table and its nested tables
void LuaConfigData::extract_table_values(std::unordered_map<std::string, std::any> &values, 
                                        const std::string &prefix) {
    // lua_gettop returns the index of the top element
    // The table to process should be at the top of the stack
    lua_pushnil(luastate_);  // Push the first key
    while (lua_next(luastate_, -2) != 0) {
        // Now key is at index -2 and value is at index -1
        std::string key;
        if (lua_isstring(luastate_, -2)) {
            key = lua_tostring(luastate_, -2);
        } else if (lua_isnumber(luastate_, -2)) {
            // Convert numeric keys to string
            key = std::to_string(lua_tonumber(luastate_, -2));
        } else {
            // Skip keys that are not strings or numbers
            lua_pop(luastate_, 1);  // Pop value, keep key for next iteration
            continue;
        }

        // Create the full key path
        std::string fullKey = prefix.empty() ? key : prefix + "." + key;

        if (lua_istable(luastate_, -1)) {
            // Recursively process nested table
            extract_table_values(values, fullKey);
        } else if (lua_isinteger(luastate_, -1)) {
            int value = static_cast<int>(lua_tointeger(luastate_, -1));
            values[fullKey] = value;
        } else if (lua_isnumber(luastate_, -1)) {
            double value = lua_tonumber(luastate_, -1);
            values[fullKey] = value;
        } else if (lua_isstring(luastate_, -1)) {
            std::string value = lua_tostring(luastate_, -1);
            values[fullKey] = value;
        } else if (lua_isboolean(luastate_, -1)) {
            bool value = lua_toboolean(luastate_, -1);
            values[fullKey] = value;
        } else {
            logger_->debug("Skipping unsupported type for key {}", fullKey);
        }

        // Pop value, keep key for next iteration
        lua_pop(luastate_, 1);
    }
}

int LuaConfigData::get_int_value(const std::string &table_name, const std::string &key_path) {
    if (table_name.empty() || key_path.empty()) {
        logger_->error("Table name *{}* or key path *{}* is empty.", table_name, key_path);
        return 0;
    }

    // Split the key_path to get the last key
    size_t last_dot = key_path.find_last_of('.');
    std::string parent_path = "";
    std::string last_key = key_path;
    
    if (last_dot != std::string::npos) {
        parent_path = key_path.substr(0, last_dot);
        last_key = key_path.substr(last_dot + 1);
    }
    
    // Traverse to the parent table
    if (!traverse_nested_tables(table_name, parent_path)) {
        return 0;
    }
    
    // Now get the value from the final table
    lua_pushstring(luastate_, last_key.c_str());
    lua_gettable(luastate_, -2);
    
    if (lua_isnil(luastate_, -1)) {
        lua_pop(luastate_, 2); // Remove nil value and table from stack
        return 0;
    }
    
    if (!lua_isinteger(luastate_, -1)) {
        logger_->error("Value from {}.{} is not an integer.", table_name, key_path);
        lua_pop(luastate_, 2); // Remove value and table from stack
        return 0;
    }
    
    int value = static_cast<int>(lua_tointeger(luastate_, -1));
    lua_pop(luastate_, 2); // Remove value and table from stack
    return value;
}

std::string LuaConfigData::get_string_value(const std::string &table_name, const std::string &key_path) {
    if (table_name.empty() || key_path.empty()) {
        logger_->error("Table name *{}* or key path *{}* is empty.", table_name, key_path);
        return "";
    }
    
    // Split the key_path to get the last key
    size_t last_dot = key_path.find_last_of('.');
    std::string parent_path = "";
    std::string last_key = key_path;
    
    if (last_dot != std::string::npos) {
        parent_path = key_path.substr(0, last_dot);
        last_key = key_path.substr(last_dot + 1);
    }
    
    // Traverse to the parent table
    if (!traverse_nested_tables(table_name, parent_path)) {
        return "";
    }
    
    // Now get the value from the final table
    lua_pushstring(luastate_, last_key.c_str());
    lua_gettable(luastate_, -2);
    
    if (lua_isnil(luastate_, -1)) {
        lua_pop(luastate_, 2); // Remove nil value and table from stack
        return "";
    }
    
    if (!lua_isstring(luastate_, -1)) {
        logger_->error("Value from {}.{} is not a string.", table_name, key_path);
        lua_pop(luastate_, 2); // Remove value and table from stack
        return "";
    }
    
    std::string value = lua_tostring(luastate_, -1);
    lua_pop(luastate_, 2); // Remove value and table from stack
    return value;
}

bool LuaConfigData::get_all_values(const std::string &table_name, std::unordered_map<std::string, std::any> &values) {
    if (table_name.empty()) {
        logger_->error("Table name *{}* is empty.", table_name);
        return false;
    }

    lua_getglobal(luastate_, table_name.c_str()); // Push the table onto the stack
    if (!lua_istable(luastate_, -1)) {
        logger_->error("{} is not a table.", table_name);
        lua_pop(luastate_, 1); // Remove non-table from stack
        return false;
    }

    // Process the table recursively
    extract_table_values(values, "");

    lua_pop(luastate_, 1); // Remove table from stack
    return true;
}

} // namespace luascript
} // namespace quanttrader