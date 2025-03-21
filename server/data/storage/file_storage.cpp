#include "file_storage.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

namespace quanttrader {
namespace data {
namespace storage {

FileStorage::FileStorage() {
    logger_ = quanttrader::log::get_common_rotation_logger("FileStorage", "data");
}

FileStorage::~FileStorage() {
    // No specific cleanup needed
}

bool FileStorage::initialize(const std::string& storage_path) {
    base_path_ = storage_path;
    
    if (!fs::exists(base_path_)) {
        try {
            fs::create_directories(base_path_);
            logger_->info("Created storage directory: {}", base_path_);
        } catch (const std::exception& e) {
            logger_->error("Failed to create storage directory: {}, error: {}", base_path_, e.what());
            return false;
        }
    }
    
    logger_->info("Initialized file storage at: {}", base_path_);
    return true;
}

bool FileStorage::store_bars(const std::string& symbol, 
                            BarType bar_type, 
                            unsigned int bar_size,
                            const BarSeries& bars) {
    if (bars.start_time.empty()) {
        logger_->warn("Attempting to store empty bar series for symbol: {}", symbol);
        return false;
    }
    
    std::string file_path = get_data_file_path(symbol, bar_type, bar_size);
    
    // Ensure the directory exists
    fs::path dir_path = fs::path(file_path).parent_path();
    if (!fs::exists(dir_path)) {
        try {
            fs::create_directories(dir_path);
        } catch (const std::exception& e) {
            logger_->error("Failed to create directory: {}, error: {}", dir_path.string(), e.what());
            return false;
        }
    }
    
    // Prepare data for serialization
    // Format: size_t count, followed by the data arrays
    size_t count = bars.start_time.size();
    
    // Calculate total size of uncompressed data
    size_t total_size = sizeof(size_t);  // size of count
    total_size += count * sizeof(uint64_t);  // size of all start_time entries
    total_size += count * sizeof(double) * 4;  // size of all open, high, low, close entries
    total_size += count * sizeof(Decimal) * 2;  // size of all volume and wap entries
    total_size += count * sizeof(int);  // size of all count entries
    
    // Buffer for uncompressed data
    std::vector<char> buffer(total_size);
    char* ptr = buffer.data();
    
    // Write count
    *reinterpret_cast<size_t*>(ptr) = count;
    ptr += sizeof(size_t);
    
    // Write all arrays
    // start_time
    std::memcpy(ptr, bars.start_time.data(), count * sizeof(uint64_t));
    ptr += count * sizeof(uint64_t);
    
    // open
    std::memcpy(ptr, bars.open.data(), count * sizeof(double));
    ptr += count * sizeof(double);
    
    // high
    std::memcpy(ptr, bars.high.data(), count * sizeof(double));
    ptr += count * sizeof(double);
    
    // low
    std::memcpy(ptr, bars.low.data(), count * sizeof(double));
    ptr += count * sizeof(double);
    
    // close
    std::memcpy(ptr, bars.close.data(), count * sizeof(double));
    ptr += count * sizeof(double);
    
    // wap
    std::memcpy(ptr, bars.wap.data(), count * sizeof(Decimal));
    ptr += count * sizeof(Decimal);
    
    // volume
    std::memcpy(ptr, bars.volume.data(), count * sizeof(Decimal));
    ptr += count * sizeof(Decimal);
    
    // count
    std::memcpy(ptr, bars.count.data(), count * sizeof(int));
    
    // Compress the data
    std::vector<char> compressed_data;
    if (!compress_data(buffer.data(), buffer.size(), compressed_data)) {
        logger_->error("Failed to compress data for symbol: {}", symbol);
        return false;
    }
    
    // Store the compressed data to file
    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        logger_->error("Failed to open file for writing: {}", file_path);
        return false;
    }
    
    // Write the original size first (needed for decompression)
    file.write(reinterpret_cast<const char*>(&total_size), sizeof(size_t));
    
    // Write the compressed data
    file.write(compressed_data.data(), compressed_data.size());
    
    if (!file) {
        logger_->error("Failed to write data to file: {}", file_path);
        return false;
    }
    
    logger_->info("Successfully stored {} bars for symbol: {} bar_type: {} bar_size: {}", 
                 count, symbol, get_bar_type_string(bar_type, bar_size), bar_size);
    return true;
}

std::optional<BarSeries> FileStorage::load_bars(const std::string& symbol, 
                                               BarType bar_type, 
                                               unsigned int bar_size,
                                               std::optional<uint64_t> start_time,
                                               std::optional<uint64_t> end_time) {
    std::string file_path = get_data_file_path(symbol, bar_type, bar_size);
    
    if (!fs::exists(file_path)) {
        logger_->warn("Data file not found: {}", file_path);
        return std::nullopt;
    }
    
    // Open the file
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        logger_->error("Failed to open file for reading: {}", file_path);
        return std::nullopt;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read the original uncompressed size
    size_t original_size;
    file.read(reinterpret_cast<char*>(&original_size), sizeof(size_t));
    
    // Read the compressed data
    std::vector<char> compressed_data(file_size - sizeof(size_t));
    file.read(compressed_data.data(), compressed_data.size());
    
    if (!file) {
        logger_->error("Failed to read data from file: {}", file_path);
        return std::nullopt;
    }
    
    // Decompress the data
    std::vector<char> uncompressed_data;
    if (!decompress_data(compressed_data.data(), compressed_data.size(), uncompressed_data, original_size)) {
        logger_->error("Failed to decompress data for symbol: {}", symbol);
        return std::nullopt;
    }
    
    // Parse the data
    const char* ptr = uncompressed_data.data();
    
    // Read count
    size_t count = *reinterpret_cast<const size_t*>(ptr);
    ptr += sizeof(size_t);
    
    BarSeries result;
    
    // Resize all vectors
    result.start_time.resize(count);
    result.open.resize(count);
    result.high.resize(count);
    result.low.resize(count);
    result.close.resize(count);
    result.wap.resize(count);
    result.volume.resize(count);
    result.count.resize(count);
    
    // Read all arrays
    // start_time
    std::memcpy(result.start_time.data(), ptr, count * sizeof(uint64_t));
    ptr += count * sizeof(uint64_t);
    
    // open
    std::memcpy(result.open.data(), ptr, count * sizeof(double));
    ptr += count * sizeof(double);
    
    // high
    std::memcpy(result.high.data(), ptr, count * sizeof(double));
    ptr += count * sizeof(double);
    
    // low
    std::memcpy(result.low.data(), ptr, count * sizeof(double));
    ptr += count * sizeof(double);
    
    // close
    std::memcpy(result.close.data(), ptr, count * sizeof(double));
    ptr += count * sizeof(double);
    
    // wap
    std::memcpy(result.wap.data(), ptr, count * sizeof(Decimal));
    ptr += count * sizeof(Decimal);
    
    // volume
    std::memcpy(result.volume.data(), ptr, count * sizeof(Decimal));
    ptr += count * sizeof(Decimal);
    
    // count
    std::memcpy(result.count.data(), ptr, count * sizeof(int));
    
    // Apply time filtering if required
    if (start_time.has_value() || end_time.has_value()) {
        size_t start_idx = 0;
        size_t end_idx = count;
        
        if (start_time.has_value()) {
            auto it = std::lower_bound(result.start_time.begin(), result.start_time.end(), start_time.value());
            if (it != result.start_time.end()) {
                start_idx = std::distance(result.start_time.begin(), it);
            }
        }
        
        if (end_time.has_value()) {
            auto it = std::upper_bound(result.start_time.begin(), result.start_time.end(), end_time.value());
            if (it != result.start_time.begin()) {
                end_idx = std::distance(result.start_time.begin(), it);
            }
        }
        
        // No data in the specified range
        if (start_idx >= end_idx) {
            logger_->warn("No data in the specified time range for symbol: {}", symbol);
            return std::nullopt;
        }
        
        // Apply the filtering
        size_t new_count = end_idx - start_idx;
        
        BarSeries filtered_result;
        filtered_result.start_time.assign(result.start_time.begin() + start_idx, result.start_time.begin() + end_idx);
        filtered_result.open.assign(result.open.begin() + start_idx, result.open.begin() + end_idx);
        filtered_result.high.assign(result.high.begin() + start_idx, result.high.begin() + end_idx);
        filtered_result.low.assign(result.low.begin() + start_idx, result.low.begin() + end_idx);
        filtered_result.close.assign(result.close.begin() + start_idx, result.close.begin() + end_idx);
        filtered_result.wap.assign(result.wap.begin() + start_idx, result.wap.begin() + end_idx);
        filtered_result.volume.assign(result.volume.begin() + start_idx, result.volume.begin() + end_idx);
        filtered_result.count.assign(result.count.begin() + start_idx, result.count.begin() + end_idx);
        
        result = std::move(filtered_result);
    }
    
    logger_->info("Successfully loaded {} bars for symbol: {} bar_type: {} bar_size: {}", 
                 result.start_time.size(), symbol, get_bar_type_string(bar_type, bar_size), bar_size);
    return result;
}

bool FileStorage::has_data(const std::string& symbol, 
                          BarType bar_type, 
                          unsigned int bar_size) {
    std::string file_path = get_data_file_path(symbol, bar_type, bar_size);
    return fs::exists(file_path);
}

std::vector<std::string> FileStorage::get_available_symbols() {
    std::vector<std::string> symbols;
    
    try {
        for (const auto& entry : fs::directory_iterator(base_path_)) {
            if (entry.is_directory()) {
                symbols.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::exception& e) {
        logger_->error("Failed to list directories in {}, error: {}", base_path_, e.what());
    }
    
    return symbols;
}

std::string FileStorage::get_bar_type_string(BarType bar_type, unsigned int bar_size) const {
    switch (bar_type) {
        case BarType::Tick:
            return "tick";
        case BarType::Second:
            return std::to_string(bar_size) + "sec";
        case BarType::Minute:
            return std::to_string(bar_size) + "min";
        case BarType::Hour:
            return std::to_string(bar_size) + "hour";
        case BarType::Day:
            return std::to_string(bar_size) + "day";
        case BarType::Week:
            return std::to_string(bar_size) + "week";
        case BarType::Month:
            return std::to_string(bar_size) + "month";
        case BarType::Year:
            return std::to_string(bar_size) + "year";
        default:
            return "unknown";
    }
}

std::string FileStorage::get_data_file_path(const std::string& symbol, 
                                           BarType bar_type, 
                                           unsigned int bar_size) const {
    std::string bar_type_str = get_bar_type_string(bar_type, bar_size);
    return base_path_ + "/" + symbol + "/" + bar_type_str + ".dat";
}

bool FileStorage::compress_data(const char* source, size_t source_size, std::vector<char>& dest) {
    // Estimate the buffer size needed for compression
    uLongf dest_size = compressBound(source_size);
    dest.resize(dest_size);
    
    // Compress the data
    int result = compress(reinterpret_cast<Bytef*>(dest.data()), &dest_size, 
                          reinterpret_cast<const Bytef*>(source), source_size);
    
    if (result != Z_OK) {
        logger_->error("Compression failed with error code: {}", result);
        return false;
    }
    
    // Resize the vector to the actual compressed size
    dest.resize(dest_size);
    return true;
}

bool FileStorage::decompress_data(const char* source, size_t source_size, 
                                 std::vector<char>& dest, size_t expected_size) {
    // Allocate buffer for decompressed data
    dest.resize(expected_size);
    
    // Decompress the data
    uLongf dest_size = expected_size;
    int result = uncompress(reinterpret_cast<Bytef*>(dest.data()), &dest_size, 
                           reinterpret_cast<const Bytef*>(source), source_size);
    
    if (result != Z_OK) {
        logger_->error("Decompression failed with error code: {}", result);
        return false;
    }
    
    // Ensure we got the expected size
    if (dest_size != expected_size) {
        logger_->error("Decompressed size ({}) does not match expected size ({})", dest_size, expected_size);
        return false;
    }
    
    return true;
}

} // namespace storage
} // namespace data
} // namespace quanttrader