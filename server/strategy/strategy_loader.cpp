#include "strategy_loader.h"
#include "logger/quantlogger.h"

#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

namespace quanttrader {
namespace strategy {

namespace qlog = quanttrader::log;

static std::vector<void*> loaded_handles;

bool StrategyLoader::load_plugins(const std::string &directory) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        qlog::Warn("Strategy plugin directory {} does not exist", directory);
        return false;
    }

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            void *handle = dlopen(entry.path().c_str(), RTLD_NOW);
            if (!handle) {
                qlog::Error("Failed to load strategy plugin {}: {}", entry.path().string(), dlerror());
            } else {
                loaded_handles.push_back(handle);
                qlog::Info("Loaded strategy plugin {}", entry.path().string());
            }
        }
    }
    return !loaded_handles.empty();
}

void StrategyLoader::unload_plugins() {
    for (void *handle : loaded_handles) {
        if (handle) {
            dlclose(handle);
        }
    }
    loaded_handles.clear();
}

} // namespace strategy
} // namespace quanttrader
