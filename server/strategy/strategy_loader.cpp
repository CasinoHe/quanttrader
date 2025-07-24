#include "strategy_loader.h"
#include "strategy_factory.h"
#include "logger/quantlogger.h"

#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

namespace quanttrader {
namespace strategy {

namespace qlog = quanttrader::log;

std::unordered_map<std::string, PluginInfo> StrategyLoader::loaded_plugins_;
std::string StrategyLoader::current_loading_plugin_;

bool StrategyLoader::load_plugins(const std::string &directory) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        qlog::Warn("Strategy plugin directory {} does not exist", directory);
        return false;
    }

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            std::string plugin_name = entry.path().stem().string();
            
            // Check if plugin is already loaded
            if (loaded_plugins_.find(plugin_name) != loaded_plugins_.end()) {
                qlog::Info("Strategy plugin {} is already loaded", plugin_name);
                continue;
            }
            
            // Set current loading plugin for tracking registrations
            current_loading_plugin_ = plugin_name;
            
            void *handle = dlopen(entry.path().c_str(), RTLD_NOW);
            if (!handle) {
                qlog::Error("Failed to load strategy plugin {}: {}", entry.path().string(), dlerror());
                current_loading_plugin_.clear();
            } else {
                PluginInfo plugin_info;
                plugin_info.handle = handle;
                plugin_info.file_path = entry.path().string();
                
                // Store plugin info (strategies are tracked during loading via track_strategy_registration)
                loaded_plugins_[plugin_name] = plugin_info;
                qlog::Info("Loaded strategy plugin {} with {} strategies", 
                          entry.path().string(), loaded_plugins_[plugin_name].registered_strategies.size());
                
                current_loading_plugin_.clear();
            }
        }
    }
    return !loaded_plugins_.empty();
}

void StrategyLoader::track_strategy_registration(const std::string& plugin_name, const std::string& strategy_name) {
    auto it = loaded_plugins_.find(plugin_name);
    if (it != loaded_plugins_.end()) {
        it->second.registered_strategies.push_back(strategy_name);
        qlog::Info("Tracked strategy {} registration for plugin {}", strategy_name, plugin_name);
    }
}

void StrategyLoader::unload_plugins() {
    for (auto& [plugin_name, plugin_info] : loaded_plugins_) {
        // Unregister all strategies registered by this plugin
        for (const std::string& strategy_name : plugin_info.registered_strategies) {
            if (StrategyFactory::instance()->unregister_strategy(strategy_name)) {
                qlog::Info("Unregistered strategy {} from plugin {}", strategy_name, plugin_name);
            } else {
                qlog::Warn("Failed to unregister strategy {} from plugin {}", strategy_name, plugin_name);
            }
        }
        
        // Close the plugin handle
        if (plugin_info.handle) {
            dlclose(plugin_info.handle);
            qlog::Info("Unloaded strategy plugin {}", plugin_name);
        }
    }
    loaded_plugins_.clear();
}

void StrategyLoader::unload_plugin(const std::string& plugin_name) {
    auto it = loaded_plugins_.find(plugin_name);
    if (it != loaded_plugins_.end()) {
        PluginInfo& plugin_info = it->second;
        
        // Unregister all strategies registered by this plugin
        for (const std::string& strategy_name : plugin_info.registered_strategies) {
            if (StrategyFactory::instance()->unregister_strategy(strategy_name)) {
                qlog::Info("Unregistered strategy {} from plugin {}", strategy_name, plugin_name);
            } else {
                qlog::Warn("Failed to unregister strategy {} from plugin {}", strategy_name, plugin_name);
            }
        }
        
        // Close the plugin handle
        if (plugin_info.handle) {
            dlclose(plugin_info.handle);
            qlog::Info("Unloaded strategy plugin {}", plugin_name);
        }
        
        loaded_plugins_.erase(it);
    } else {
        qlog::Warn("Plugin {} not found for unloading", plugin_name);
    }
}

} // namespace strategy
} // namespace quanttrader
