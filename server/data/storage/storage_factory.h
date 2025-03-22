#pragma once

#include "data_storage.h"
#include "file_storage.h"
#include "db_storage.h"
#include <memory>
#include <string>

namespace quanttrader {
namespace data {
namespace storage {

/**
 * @brief Factory class for creating storage providers
 * 
 * This class simplifies creating storage instances of different types.
 */
class StorageFactory {
public:
    /**
     * @brief Create a storage provider based on the specified type
     * 
     * @param storage_type Type of storage ("file" or "db")
     * @return std::shared_ptr<DataStorage> Storage provider instance
     */
    static std::shared_ptr<DataStorage> create(const std::string& storage_type) {
        if (storage_type == "db" || storage_type == "database" || storage_type == "sql") {
            return std::make_shared<DbStorage>();
        } else {
            // Default to file storage for any other type
            return std::make_shared<FileStorage>();
        }
    }
};

} // namespace storage
} // namespace data
} // namespace quanttrader