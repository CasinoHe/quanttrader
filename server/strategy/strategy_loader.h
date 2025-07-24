#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace quanttrader {
namespace strategy {

struct PluginInfo {
    void* handle;
    std::vector<std::string> registered_strategies;
    std::string file_path;
};

class StrategyLoader {
public:
    static bool load_plugins(const std::string &directory);
    static void unload_plugins();
    static void unload_plugin(const std::string& plugin_name);
    
    // Plugin-aware strategy registration tracking
    static void track_strategy_registration(const std::string& plugin_name, const std::string& strategy_name);
    static std::string get_current_loading_plugin() { return current_loading_plugin_; }
    
private:
    static std::unordered_map<std::string, PluginInfo> loaded_plugins_;
    static std::string current_loading_plugin_;  // Track which plugin is currently being loaded
};

}
}
