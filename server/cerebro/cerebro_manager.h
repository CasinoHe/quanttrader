#pragma once

#include "cerebro_base.h"
#include "cerebro_factory.h"
#include "cerebro_consts.h"
#include "common/singleton.h"
#include "logger/quantlogger.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <mutex>

namespace quanttrader {
namespace cerebro {

/**
 * @brief Manager class for Cerebro instances
 * 
 * This class manages the lifecycle of Cerebro instances throughout the application.
 * It allows creating, retrieving, and destroying cerebro instances by name.
 */
class CerebroManager : public Singleton<CerebroManager> {
public:
    /**
     * @brief Create a new Cerebro instance
     * 
     * @param cerebroType Type of cerebro to create (e.g., "backtest" or "live")
     * @param name Unique name to identify this cerebro instance
     * @param configPath Path to the configuration file for the cerebro
     * @return std::shared_ptr<Cerebro> The created cerebro instance or nullptr if creation failed
     */
    std::shared_ptr<Cerebro> create_cerebro(
        const std::string& cerebroType,
        const std::string& name,
        const std::string& configPath) {
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if a cerebro with the same name already exists
        if (active_cerebros_.find(name) != active_cerebros_.end()) {
            logger_->error("Cerebro with name '{}' already exists", name);
            return nullptr;
        }
        
        // Create the cerebro instance
        auto cerebro = CerebroFactory::instance().create_cerebro(cerebroType, name, configPath);
        if (!cerebro) {
            logger_->error("Failed to create cerebro of type '{}' with name '{}'", cerebroType, name);
            return nullptr;
        }
        
        // Store the cerebro instance
        active_cerebros_[name] = cerebro;
        logger_->info("Created cerebro of type '{}' with name '{}'", cerebroType, name);
        
        return cerebro;
    }
    
    /**
     * @brief Get an existing Cerebro instance by name
     * 
     * @param name Name of the cerebro instance to retrieve
     * @return std::shared_ptr<Cerebro> The cerebro instance or nullptr if not found
     */
    std::shared_ptr<Cerebro> get_cerebro(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = active_cerebros_.find(name);
        if (it == active_cerebros_.end()) {
            logger_->warn("Cerebro with name '{}' not found", name);
            return nullptr;
        }
        
        return it->second;
    }
    
    /**
     * @brief Check if a Cerebro instance with the given name exists
     * 
     * @param name Name to check
     * @return true if a cerebro with the name exists
     * @return false if no cerebro with the name exists
     */
    bool has_cerebro(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        return active_cerebros_.find(name) != active_cerebros_.end();
    }
    
    /**
     * @brief Destroy a Cerebro instance by name
     * 
     * @param name Name of the cerebro instance to destroy
     * @return true if the cerebro was destroyed
     * @return false if no cerebro with the name was found
     */
    bool destroy_cerebro(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = active_cerebros_.find(name);
        if (it == active_cerebros_.end()) {
            logger_->warn("Cannot destroy cerebro with name '{}': not found", name);
            return false;
        }
        
        // Stop the cerebro before removing it
        it->second->stop();
        
        // Remove the cerebro instance
        active_cerebros_.erase(it);
        logger_->info("Destroyed cerebro with name '{}'", name);
        
        return true;
    }
    
    /**
     * @brief Get the number of active Cerebro instances
     * 
     * @return size_t The number of active cerebro instances
     */
    size_t get_cerebro_count() {
        std::lock_guard<std::mutex> lock(mutex_);
        return active_cerebros_.size();
    }
    
    /**
     * @brief Destroy all active Cerebro instances
     */
    void destroy_all_cerebros() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        logger_->info("Destroying all cerebro instances (count: {})", active_cerebros_.size());
        
        // Stop and destroy all cerebro instances
        for (auto& pair : active_cerebros_) {
            pair.second->stop();
        }
        
        active_cerebros_.clear();
    }

private:
    friend class Singleton<CerebroManager>;
    
    CerebroManager() {
        logger_ = quanttrader::log::get_common_rotation_logger("CerebroManager", "cerebro");
        logger_->info("CerebroManager initialized");
    }
    
    ~CerebroManager() {
        destroy_all_cerebros();
        logger_->info("CerebroManager destroyed");
    }
    
    std::unordered_map<std::string, std::shared_ptr<Cerebro>> active_cerebros_;
    std::mutex mutex_;
    quanttrader::log::LoggerPtr logger_;
};

} // namespace cerebro
} // namespace quanttrader